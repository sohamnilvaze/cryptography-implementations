#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<vector>
#include<random>

using namespace std;

vector<char> prepare_key()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 25);  // full byte range
    vector<char> key;
    while(key.size()<26)
    {
        char temp = static_cast<char>(distrib(gen) + 65);
        if(find(key.begin(),key.end(),temp) == key.end())
        {
            key.push_back(temp);
        }
    }
    cout<<"Key is:\n";
    for(int i=0; i<26;i++)
    {
        cout<<key[i];
    }
    return key;
}

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


string encrypt(vector<char> key, string plaintext)
{
    string cipher = "";
    for(int i=0; i<plaintext.size();i++)
    {
        char temp = key[plaintext[i] - 'A'];
        cipher = cipher + temp;
    }

    return cipher;
}

int determine_pos(vector<char> key, char t)
{
    for(int i=0; i<key.size();i++)
    {
        if(key[i] == t)
        {
            return i;

        }
    }

    return 0;
}

string decrypt(vector<char> key, string cipher)
{
    string plain = "";
    for(int i=0; i<cipher.size();i++)
    {
        int pos = determine_pos(key,cipher[i]);
        char temp = static_cast<char>(65 + pos);
        plain = plain + temp;
    }

    return plain;

}


int main()
{
    vector<char> key = prepare_key();
    string text;
    cout<<"Enter the text:\n";
    getline(cin,text);

    string processed_text = remove_spaces(text);
    cout<<"Processed text:"<<processed_text<<"\n";

    string cipher = encrypt(key,processed_text);
    cout<<"Ciphertext:"<<cipher<<"\n";

    string plain = decrypt(key,cipher);
    cout<<"Plaintext obtained again:"<<plain<<"\n";

    return 0;
}