#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;


#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

string dec_to_binary(int num)
{
    vector<char> bits;
    while(num > 0)
    {
        if(num%2 == 0)
        {
            bits.push_back('0');
        }
        else{
            bits.push_back('1');
        }
        num = num / 2;
    }

    reverse(bits.begin(),bits.end());

    string bin_no = "";
    for(char c : bits)
    {
        bin_no = bin_no + c;
    }

    return bin_no;
}

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

vector<int> chartobits(char c)
{
    vector<int> bits(8);
    unsigned char uc = static_cast<unsigned char>(c);
    for(int i = 7;i>=0;i--)
    {
        bits[7-i] = (uc >> i) & 1;
    }
    return bits;
}

vector<vector<int>> whole_string_to_bits(string text)
{
    vector<vector<int>> res;
    for(char c: text)
    {
        vector<int> cbits = chartobits(c);
        res.push_back(cbits);
    }
    cout<<"String in bits:-\n";
    for(int i=0; i<res.size()-1;i++)
    {
        for(int j = 0; j<res[0].size();j++)
        {
            cout<<res[i][j];
        }
        cout<<" ";
    }

    return res;

}

vector<vector<char>> padding(vector<vector<int>> string_bits, string len_str_in_bin)
{
    int n1 = 8 * string_bits.size();
    int n2 = 512 - (n1 % 512) - 64;
    int n_zeros = n2 - 1;
    
    string entire = "";
    for(int i=0; i<string_bits.size();i++)
    {
        for(int j = 0; j<string_bits[0].size();j++)
        {
            if(string_bits[i][j] == 0)
            {
                entire = entire + '0';
            }
            else{
                entire = entire + '1';
            }
        }
    }

    entire = entire + '1';
    for(int k= 0; k<n_zeros;k++)
    {
        entire = entire + '0';
    }

    cout<<"length of entire string before adding length:- " << entire.length() << "\n";

    if(len_str_in_bin.length() <= 64)
    {
        int n3 = 64 - len_str_in_bin.length();

        for(int k= 0; k<n3;k++)
        {
            entire = entire + '0';
        }

        for(int k = 0; k<len_str_in_bin.size();k++)
        {
            entire = entire + len_str_in_bin[k];
        }
    }

    else{
        string req = len_str_in_bin.substr(len_str_in_bin.size()-64,64);
        for(int k = 0; k<req.size();k++)
        {
            entire = entire + req[k];
        }
    }

    

    cout<<"length of entire string after adding length:- " << entire.length() << "\n";


    cout<<"String after padding:-\n";
    vector<vector<char>> res;
    int  k= 0;
    vector<char> temp;
    while(k < entire.length())
    {
        if(k % 512 != 0)
        {
            cout<<entire[k];
            temp.push_back(entire[k]);
        }
        else{
            cout<<" ";
            res.push_back(temp);
            temp.clear();
        }

        k++;


    }

    cout<<"\n";
    return res;







}



string padding_message(string message);

int main()
{
    string plaintext;
    cout<<"Enter the plaintext:\n";
    getline(cin,plaintext);

    string processed_plaintext = remove_spaces_and_upper(plaintext);

    vector<vector<int>> string_to_bits = whole_string_to_bits(processed_plaintext);

    int len_str_in_bits = 8 * string_to_bits.size();
    
    cout<<"\n The length of the string is:- " << len_str_in_bits << "\n";

    string len_str_in_bin = dec_to_binary(processed_plaintext.length());

    vector<vector<char>> padding_str = padding(string_to_bits,len_str_in_bin);

    return 0;





}