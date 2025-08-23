#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<cctype>
#include<math.h>
using namespace std;



string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c){ return toupper(c); });
    return str;
}

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c){ return tolower(c); });
    return str;
}

string remove_spaces(string text)
{
    string text2 = toLowerCase(text);
    string res = "";
    for(int i=0; i<text2.size();i++)
    {
        if(text2[i]!= ' ')
        {
            res = res + text2[i];
        }
        else{
            continue;
        }
    }

    return res;
}

string encrypt(string text, string key)
{
    int n = key.size();
    int nrows = (text.size() + n - 1) / n; // ceiling division

    // Fill matrix row by row
    vector<vector<char>> matrix(nrows, vector<char>(n, '_')); // '_' as padding
    for (int i = 0; i < text.size(); i++) {
        matrix[i / n][i % n] = text[i];
    }

    // Create index mapping of sorted key
    vector<pair<char,int>> key_order;
    for (int i = 0; i < n; i++) key_order.push_back({key[i], i});
    sort(key_order.begin(), key_order.end());

    // Print matrix (optional for debugging)
    cout << "Encryption matrix:\n";
    for (int i = 0; i < n; i++) cout << key[i] << " ";
    cout << "\n";
    for (int r = 0; r < nrows; r++) {
        for (int c = 0; c < n; c++) cout << matrix[r][c] << " ";
        cout << "\n";
    }

    // Read column by sorted key order
    string enc = "";
    for (auto [ch, col] : key_order) {
        for (int r = 0; r < nrows; r++) {
            if (matrix[r][col] != '_') enc += matrix[r][col];
        }
    }
    return enc;
}

vector<int> getpermutationorder(string key)
{
    vector<pair<char,int>> keymap;
    for(int i=0; i<key.size();i++)
    {
        keymap.push_back({key[i],i});
    }

    sort(keymap.begin(),keymap.end());
    vector<int> order(key.size());
    for(int i=0; i<keymap.size();i++)
    {
        order[i] = keymap[i].second;
    }

    return order;
}

string inversecolumnartranspose(string text,string key)
{
    int ncols = key.size();
    int nrows = ceil((double)text.size() / ncols);

    vector<int> order = getpermutationorder(key);
    string result(text.size(), ' ');

    int idx = 0;
    for(int col = 0; col < ncols;col++)
    {
        int actcol = order[col];
        for(int row = 0; row<nrows;row++)
        {
            int pos = row * ncols + actcol;
            if(pos < text.size())
            {
                result[pos] = text[idx++];
            }

        }
    }

    return result;
}

string decrypt(string text,string key)
{
    return inversecolumnartranspose(text,key);
}


int main()
{
    string text, result;
    string key1, key2;

    cout<<"Enter the 1st key:-\n";
    cin>>key1;
    cout<<"Enter the 2nd key:-\n";
    cin>>key2;
    cout<<"Enter the text:-\n";
    cin.ignore();
    getline(cin, text);

    cout<<"Text befire processing is:-"<<text<<"\n";
    string text_processed = remove_spaces(text);
    cout<<"Text processed is:-"<<text_processed<<"\n";

    key1 = toUpperCase(key1);
    key2 = toUpperCase(key2);

    string enc_string = encrypt(text_processed,key1);

    cout<<"Encrypted string after 1st stage:"<<enc_string<<"\n";

    string enc_string2 = encrypt(enc_string,key2);

    cout<<"Encrypted string after 2nd stage:-"<<enc_string2<<"\n";


    string dec_string1 = decrypt(enc_string2,key2);

    cout<<"Decrypted string after reverse pass1:-"<<dec_string1<<"\n";

    string dec_string2 = decrypt(dec_string1,key1);

    cout<<"Decrypted string after reverse pass2:-"<<dec_string2<<"\n";



    return 0;




}