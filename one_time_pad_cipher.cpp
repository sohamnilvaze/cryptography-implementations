#include<iostream>
#include<random>
#include<string>
#include<algorithm>
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

string generate_key(int n)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);  // full byte range
    string key = "";
    for(int i = 0; i < n; i++)
        key += static_cast<char>(distrib(gen));
    return key;
}

string encrypt(string plaintext, string key)
{
    string cipher = "";
    for(size_t i = 0; i < plaintext.size(); i++)
        cipher += static_cast<char>(plaintext[i] ^ key[i]);  // XOR only
    return cipher;
}

string decrypt(string cipher, string key)
{
    string orig = "";
    for(size_t i = 0; i < cipher.size(); i++)
        orig += static_cast<char>(cipher[i] ^ key[i]);  // XOR again
    return orig;
}

int main()
{
    string plaintext;
    cout<<"Enter the text:-\n";
    getline(cin, plaintext);

    string plaintext_processed = remove_spaces(plaintext);
    int n = plaintext_processed.length();

    cout<<"Plaintext after processing: "<<plaintext_processed<<"\n";

    string key = generate_key(n);
    cout<<"Generated key (in hex): ";
    for(unsigned char c : key) cout << hex << (int)c << " ";
    cout<<dec<<"\n";

    string cipher = encrypt(plaintext_processed, key);
    cout<<"Cipher (in hex): ";
    for(unsigned char c : cipher) cout << hex << (int)c << " ";
    cout<<dec<<"\n";

    string orig = decrypt(cipher, key);
    cout<<"Decrypted text: "<<orig<<"\n";

    return 0;
}
