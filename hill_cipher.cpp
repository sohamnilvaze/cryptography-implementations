#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<vector>
#include<random>
#include<math.h>
#include <iomanip>
#include <cmath>

using namespace std;

string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c){ return toupper(c); });
    return str;
}

string remove_spaces(string text)
{
    string text2 = toUpperCase(text);
    string res = "";
    for(char c : text2)
        if(c != ' ') res += c;
    return res;
}

vector<vector<int>> prepare_key_mat(int key_size)
{
    string key = "";
    int mat_size = (int)pow((double)key_size,0.50);
    vector<vector<int>> key_mat(mat_size,vector<int>(mat_size,-1));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 25);
    for(int i = 0; i<mat_size;i++)
    {
        for(int j = 0; j<mat_size;j++)
        {
            int temp = distrib(gen);
            key_mat[j][i] = temp;
        }
    }

    return key_mat;


}

int find_pos(vector<char> alphas, char s)
{
    for(int i = 0; i<alphas.size();i++)
    {
        if(alphas[i] == s)
        {
            return i;
        }
    }
    return -1;
}

vector<vector<int>> prepare_plaintext(vector<char> alphas,string text, int key_mat_size)
{
    int n = text.size();
    int rem = key_mat_size - (n % key_mat_size);
    int r = key_mat_size;
    for(int i = 0; i< rem; i++)
    {
        text = text + "Z";
    }

    int c = text.size() / key_mat_size;

    vector<vector<int>> prep_text(r,vector<int>(c,-1));
    int k = 0;
    for(int j = 0;j<c;j++)
    {
        for(int i = 0; i<r;i++)
        {
            prep_text[i][j] = find_pos(alphas,text[k++]); 
        }
    }

    return prep_text;


}

string encrypt(vector<char> alphas, vector<vector<int>> key_mat, vector<vector<int>> prep_text_mat)
{
    int r1 = key_mat.size();
    int c2 = prep_text_mat[0].size();
    if(key_mat[0].size() != prep_text_mat.size())
    {
        return "Decryption not possible as matrix size not matching";
    } 
    vector<vector<int>> mat_mul(r1,vector<int>(c2,-1));
    for(int i = 0; i<r1;i++)
    {
        for(int j = 0; j<c2;j++)
        {
            for(int k = 0; k<r1;k++)
            {
                int temp = (key_mat[i][k] * prep_text_mat[k][j]) % 26;
                cout<<temp<<" ";
                mat_mul[i][j] = temp;
            }
        }
        cout<<"\n";
    }

    string cipher = "";
    for(int i = 0; i<mat_mul.size();i++)
    {
        for(int j = 0; j< mat_mul[0].size();j++)
        {
            cipher = cipher + alphas[mat_mul[i][j]];
        }
    }

    return cipher;
}

void getCofactor(const vector<vector<int>> &A, vector<vector<int>> &temp, int p, int q, int n) {
    int i = 0, j = 0;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = A[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int determinant(const vector<vector<int>> &A, int n) {
    if (n == 1) return A[0][0];

    int det = 0;
    int sign = 1; 

    vector<vector<int>> temp(n, vector<int>(n));

    for (int f = 0; f < n; f++) {
        getCofactor(A, temp, 0, f, n);
        det += sign * A[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

vector<vector<int>> adjoint(const vector<vector<int>> &A) {
    int n = A.size();
    vector<vector<int>> adj(n, vector<int>(n));

    if (n == 1) {
        adj[0][0] = 1;
        return adj;
    }

    int sign = 1;
    vector<vector<int>> temp(n, vector<int>(n));

    // Cofactor matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            getCofactor(A, temp, i, j, n);
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            // Note: adj[j][i] = cofactor(i,j)
            adj[j][i] = sign * determinant(temp, n - 1);
        }
    }

    return adj;
}



int extended_euclidean_gcd(int a, int b, int &x, int &y)
{
    // goal is to find ax + by = gcd(a,b)
    if(b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    int x1,y1;
    int g = extended_euclidean_gcd(b, a%b, x1,y1);
    x = y1;
    y = x1 - (a / b)*y1;
    return g;
}

int mod_inverse(int a,int m)
{
    //goal is to find x and then adjust x such that a.x = 1 (mod m); 
    int x,y;
    int g = extended_euclidean_gcd(a,m,x,y);
    if(g!= 1)
    {
        cout<<"Modular inverse does not exists.(numbers are not coprime) for"<<a<<" and "<<m<<"\n";
        return -1;
    }

    return (x % m + m) % m;
}

string decrypt(string cipher, vector<char> alphas, vector<vector<int>>key_mat)
{
    int n = (int)pow((double)cipher.length(),0.50);
    int k = 0;
    vector<vector<int>> cipher_mat = prepare_plaintext(alphas,cipher,n);
    int key_mat_deter = determinant(key_mat,key_mat.size());
    cout<<"Determinant: "<<key_mat_deter<<"\n";
    vector<vector<int>> key_mat_adj = adjoint(key_mat);
    for(int i =0; i<key_mat_adj.size();i++)
    {
        for(int j = 0; j<key_mat_adj[0].size();j++)
        {
            if(key_mat_adj[i][j] < 0)
            {
                key_mat_adj[i][j] = -1 * ((-1 * key_mat_adj[i][j]) % alphas.size());
            }
            else{
                key_mat_adj[i][j] = key_mat_adj[i][j] % alphas.size();
            }
            
        }
    }

    int detmod = key_mat_deter % alphas.size();
    int mul_inv = mod_inverse(detmod,alphas.size());

    for(int i =0; i<key_mat_adj.size();i++)
    {
        for(int j = 0; j<key_mat_adj[0].size();j++)
        {
            key_mat_adj[i][j] = mul_inv * key_mat_adj[i][j];
            
        }
    }

    for(int i =0; i<key_mat_adj.size();i++)
    {
        for(int j = 0; j<key_mat_adj[0].size();j++)
        {
            if(key_mat_adj[i][j] < 0)
            {
                key_mat_adj[i][j] = -1 * ((-1 * key_mat_adj[i][j]) % alphas.size());
            }
            else{
                key_mat_adj[i][j] = key_mat_adj[i][j] % alphas.size();
            }
            
        }
    }

    for(int i =0; i<key_mat_adj.size();i++)
    {
        for(int j = 0; j<key_mat_adj[0].size();j++)
        {
            key_mat_adj[i][j] = key_mat_adj[i][j] % alphas.size();
            
        }
    }

    int r1 = key_mat_adj.size();
    int c2 = cipher_mat[0].size();
    if(key_mat[0].size() != cipher_mat.size())
    {
        return "Decryption not possible as matrix size not matching" ;
    } 
    vector<vector<int>> mat_mul(r1,vector<int>(c2,-1));
    for(int i = 0; i<r1;i++)
    {
        for(int j = 0; j<c2;j++)
        {
            for(int k = 0; k<r1;k++)
            {
                int temp = (key_mat_adj[i][k] * cipher_mat[k][j]) % 26;
                cout<<temp<<" ";
                mat_mul[i][j] = temp;
            }
        }
        cout<<"\n";
    }

    string res = "";
    for(int i = 0; i<mat_mul.size();i++)
    {
        for(int j = 0; j< mat_mul[0].size();j++)
        {
            res = res + alphas[mat_mul[i][j]];
        }
    }


    return res;

}

int main()
{
    vector<char> alphas = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    int key_size;
    cout<<"Enter the key size(should be proper square):\n";
    cin>>key_size;

    string text;
    cout<<"Enter the plaintext:\n";
    cin.ignore();
    getline(cin,text);

    string processed_plaintext = remove_spaces(text);

    vector<vector<int>> key_mat = prepare_key_mat(key_size);
    vector<vector<int>> prep_text = prepare_plaintext(alphas,processed_plaintext,(int)pow((double)key_size,0.50));

    cout<<"Key matrix prepared:-\n";

    for(int i = 0; i<key_mat.size();i++)
    {
        for(int j = 0; j< key_mat.size();j++)
        {
            cout<<key_mat[i][j]<<" ";         
        }
        cout<<"\n";
    }

    cout<<"Prepared plaintext matrix:-\n";

    for(int i = 0; i<prep_text.size();i++)
    {
        for(int j = 0; j<prep_text[0].size();j++)
        {
            cout<<prep_text[i][j]<<" ";
        }
        cout<<"\n";
    }

    string cipher = encrypt(alphas,key_mat,prep_text);

    cout<<"Cipher text: "<< cipher<<"\n";


    string res = decrypt(cipher,alphas,key_mat);

    cout<<"Plaintext after decrypting: "<<res<<"\n";

    

    

    return 0;


}