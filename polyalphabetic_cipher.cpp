#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<vector>
#include<random>

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


string encrypt(vector<int> key,string plaintext)
{
    string cipher = "";
    for(int i=0; i<plaintext.size();i++)
    {
        int k = key[i%key.size()];
        char temp = ((plaintext[i] - 'A' + k) % 26 + 'A');
        cout<<"Orig char: "<<plaintext[i]<<" Key: "<<k<<" New char: "<<temp<<"\n";
        cipher = cipher + temp;
    }

    return cipher;
}

string decrypt(vector<int> key,string ciphertext)
{
    string plaintext = "";
    for(int i=0; i<ciphertext.size();i++)
    {
        int k = key[i%key.size()];
        char temp = (((ciphertext[i] - 'A' - k + 26) % 26) + 'A');
        cout<<"Orig char: "<<ciphertext[i]<<" Key: "<<k<<" New char: "<<temp<<"\n";
        plaintext = plaintext + temp;
    }

    return plaintext;
}

vector<int> prepare_key(string key)
{
    vector<int> res;
    key = toUpperCase(key);
    for(char c : key)
        res.push_back(c - 'A');
    return res;
}

int main()
{
    string key,text;
    cout<<"Enter the key:-\n";
    cin>>key;
    cout<<"Enter the plaintext:-\n";
    cin.ignore();
    getline(cin,text);

    string processed_text = remove_spaces(text);
    cout<<"Processed text:"<<processed_text<<"\n";

    vector<int> processed_key = prepare_key(key);

    string cipher = encrypt(processed_key,processed_text);

    cout<<"The cipher is:-"<<cipher<<"\n";

    string plain = decrypt(processed_key,cipher);

    cout<<"The plain text obtained back after decryption:"<<plain<<"\n";

    return 0;


}