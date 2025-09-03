#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

string toUpperCase(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return toupper(c); });
    return s;
}

string remove_spaces_and_upper(string text) {
    text = toUpperCase(text);
    string out;
    for (char c : text) if (c != ' ') out.push_back(c);
    return out;
}

int find_pos(const vector<char>& alphas, char s) {
    for (int i = 0; i < (int)alphas.size(); ++i) if (alphas[i] == s) return i;
    return -1;
}


vector<vector<int>> prepare_plaintext(const vector<char>& alphas, const string& text, int block_size) {
    string t = text;
    int n = (int)t.size();
    int rem = (block_size - (n % block_size)) % block_size;
    for (int i = 0; i < rem; ++i) t.push_back('Z');
    int cols = (int)t.size() / block_size;

    vector<vector<int>> mat(block_size, vector<int>(cols, 0));
    int k = 0;
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < block_size; ++i) {
            mat[i][j] = find_pos(alphas, t[k++]);
        }
    }
    return mat;
}


vector<vector<int>> matmul_mod26(const vector<vector<int>>& A, const vector<vector<int>>& B, int mod = 26) {
    int n = A.size();
    int cols = B[0].size();
    vector<vector<int>> C(n, vector<int>(cols, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < cols; ++j) {
            long long sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += 1LL * A[i][k] * B[k][j];
            }
            C[i][j] = (int)((sum % mod + mod) % mod);
        }
    }
    return C;
}


void getCofactor(const vector<vector<int>> &A, vector<vector<int>> &temp, int p, int q, int n) {
    int i = 0, j = 0;
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if (row != p && col != q) {
                temp[i][j++] = A[row][col];
                if (j == n - 1) { j = 0; ++i; }
            }
        }
    }
}

int determinant(const vector<vector<int>> &A, int n) {
    if (n == 1) return A[0][0];
    int det = 0;
    int sign = 1;
    vector<vector<int>> temp(n, vector<int>(n));
    for (int f = 0; f < n; ++f) {
        getCofactor(A, temp, 0, f, n);
        det += sign * A[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

vector<vector<int>> adjoint(const vector<vector<int>> &A) {
    int n = A.size();
    vector<vector<int>> adj(n, vector<int>(n, 0));
    if (n == 1) {
        adj[0][0] = 1;
        return adj;
    }
    int sign;
    vector<vector<int>> temp(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            getCofactor(A, temp, i, j, n);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = sign * determinant(temp, n - 1);
        }
    }
    return adj;
}


int extended_euclidean_gcd(int a, int b, int &x, int &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int x1, y1;
    int g = extended_euclidean_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int mod_inverse(int a, int m) {
    a = ((a % m) + m) % m;
    int x, y;
    int g = extended_euclidean_gcd(a, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}


string encrypt(const vector<char>& alphas, const vector<vector<int>>& key_mat, const vector<vector<int>>& prep_text) {
    int n = key_mat.size();
    int cols = prep_text[0].size();
    if ((int)key_mat[0].size() != (int)prep_text.size()) return "Encryption not possible: dimensions mismatch";

    vector<vector<int>> product = matmul_mod26(key_mat, prep_text, 26);

    
    string cipher;
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < n; ++i) {
            cipher.push_back(alphas[(product[i][j] % 26 + 26) % 26]);
        }
    }
    return cipher;
}

string decrypt(const vector<char>& alphas, const vector<vector<int>>& key_mat, const string& cipher) {
    int n = key_mat.size();
    
    vector<vector<int>> cipher_mat = prepare_plaintext(alphas, cipher, n);

    
    int det = determinant(key_mat, n);
    int mod = (int)alphas.size();
    int detmod = ((det % mod) + mod) % mod;
    int detInv = mod_inverse(detmod, mod);
    if (detInv == -1) {
        return "Decryption not possible: key matrix not invertible modulo 26 (det not coprime with 26).";
    }

    vector<vector<int>> adj = adjoint(key_mat);
    vector<vector<int>> key_inv(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int a = ((adj[i][j] % mod) + mod) % mod;
            key_inv[i][j] = (int)((1LL * detInv * a) % mod);
        }
    }

    
    vector<vector<int>> plain_num = matmul_mod26(key_inv, cipher_mat, mod);

    
    string res;
    int cols = plain_num[0].size();
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < n; ++i) {
            res.push_back(alphas[(plain_num[i][j] % mod + mod) % mod]);
        }
    }
    return res;
}


int main() {
    vector<char> alphas(26);
    for (int i = 0; i < 26; ++i) alphas[i] = 'A' + i;

    int key_size;
    cout << "Enter key matrix size (n) (e.g. 2 for 2x2, 3 for 3x3): ";
    cin >> key_size;
    cin.ignore();

    string text;
    cout << "Enter the plaintext:\n";
    getline(cin, text);

    string processed_plaintext = remove_spaces_and_upper(text);
    cout << "Processed plaintext: " << processed_plaintext << "\n";

    cout << "Do you want to enter the key matrix manually? (y/n): ";
    char choice; cin >> choice;

    vector<vector<int>> key_mat(key_size, vector<int>(key_size));
    if (choice == 'y' || choice == 'Y') {
        cout << "Enter key matrix (row by row), entries 0..25:\n";
        for (int i = 0; i < key_size; ++i) {
            for (int j = 0; j < key_size; ++j) {
                cin >> key_mat[i][j];
                key_mat[i][j] = ((key_mat[i][j] % 26) + 26) % 26;
            }
        }
    } else {
        random_device rd; mt19937 gen(rd()); uniform_int_distribution<> d(0, 25);
        for (int i = 0; i < key_size; ++i)
            for (int j = 0; j < key_size; ++j)
                key_mat[i][j] = d(gen);
    }

    cout << "Key matrix (" << key_size << "x" << key_size << "):\n";
    for (int i = 0; i < key_size; ++i) {
        for (int j = 0; j < key_size; ++j) cout << setw(4) << key_mat[i][j] << " ";
        cout << "\n";
    }


    vector<vector<int>> prep_text = prepare_plaintext(alphas, processed_plaintext, key_size);
    cout << "Prepared plaintext matrix (numeric):\n";
    for (int i = 0; i < key_size; ++i) {
        for (int j = 0; j < (int)prep_text[0].size(); ++j) cout << setw(4) << prep_text[i][j] << " ";
        cout << "\n";
    }


    string cipher = encrypt(alphas, key_mat, prep_text);
    cout << "Cipher text: " << cipher << "\n";

    string recovered = decrypt(alphas, key_mat, cipher);
    cout << "Recovered plaintext (after decrypt): " << recovered << "\n";

    return 0;
}
