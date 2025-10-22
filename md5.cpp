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

#define F(b,c,d) (b & c) | (!b & d) 
#define G(b,c,d) (b & d) | (c & !d)
#define H(b,c,d) (b ^ c ^ c)
#define I(b,c,d) (c ^ (b | !d))

string dec_to_binary(uint64_t num)
{
    string bits(64,'0');

    for(int i=63;i>=0;i--)
    {
        if(num & 1)
        {
            bits[i] = '1';
        }
        else{
            bits[i] = '0';
        }
        num = num >> 1;
    }

    return bits;
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

string bin_to_hex(string bin_str)
{
    //cout<<"Inside 2\n";
    if(bin_str == "0000")
    {
        //cout<<"Equal to 0\n";
        return "0";
    }
    else if(bin_str == "0001")
    {
        //cout<<"Equal to 1\n";
        return "1";
    }
    else if(bin_str == "0010")
    {
        //cout<<"Equal to 2\n";
        return "2";
    }
    else if(bin_str == "0011")
    {
        //cout<<"Equal to 3\n";
        return "3";
    }
    else if(bin_str == "0100")
    {
        //cout<<"Equal to 4\n";
        return "4";
    }
    else if(bin_str == "0101")
    {
        //cout<<"Equal to 5\n";
        return "5";
    }
    else if(bin_str == "0110")
    {
        //cout<<"Equal to 6\n";
        return "6";
    }
    else if(bin_str == "0111")
    {
        //cout<<"Equal to 7\n";
        return "7";
    }
    else if(bin_str == "1000")
    {
        //cout<<"Equal to 8\n";
        return "8";
    }
    else if(bin_str == "1001")
    {
        //cout<<"Equal to 9\n";
        return "9";
    }
    else if(bin_str == "1010")
    {
        //cout<<"Equal to A\n";
        return "A";
    }
    else if(bin_str == "1011")
    {
        //cout<<"Equal to B\n";
        return "B";
    }
    else if(bin_str == "1100")
    {
        //cout<<"Equal to C\n";
        return "C";
    }
    else if(bin_str == "1101")
    {
        //cout<<"Equal to D\n";
        return "D";
    }
    else if(bin_str == "1110")
    {
        //cout<<"Equal to E\n";
        return "E";
    }
    else if(bin_str == "1111")
    {
        //cout<<"Equal to F\n";
        return "F";
    }

    return " ";


}

vector<int> chartobits(char c)
{
    vector<int> bits(8);
    unsigned char uc = static_cast<unsigned char>(c);
    for(int i = 0;i<8;i++)
    {
        bits[i] = (uc >> (7-i)) & 1;
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
    for(int i=0; i<res.size();i++)
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
    int mod512 = n1 % 512;
    int n2 = 448 - mod512;
    if(mod512 > 448)
    {
        n2 = (512 - (mod512 - 448));
    }
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

    if( len_str_in_bin.size() < 64)
    {
        int diff = 64 - len_str_in_bin.size();
        entire = entire + string(diff,'0');
        entire = entire + len_str_in_bin;
    }
    else if(len_str_in_bin.size() > 64)
    {
        entire = entire + len_str_in_bin.substr(len_str_in_bin.size() - 64);
    }
    else{
        entire = entire + len_str_in_bin;
    }
    

    cout<<"length of entire string after adding length:- " << entire.length() << "\n";


    
    vector<vector<char>> res;
    for(size_t k = 0; k<entire.length();k = k + 32)
    {
        vector<char> word;
        for(size_t j = k; j< k + 32 && j < entire.length();j++)
        {
            word.push_back(entire[j]);
        }
        res.push_back(word);
    }
    return res;
}

vector<vector<string>> bin_str_to_hex(vector<vector<char>> bin_str)
{
    //cout<<"Inside\n";
    vector<vector<string>> res;
    for(int i=0; i<bin_str.size();i++)
    {
        //cout<<bin_str[i].size();
        vector<string> t;
        for(int j=0; j<32;j = j+4)
        {
            string temp = "";
            temp = temp + bin_str[i][j];
            temp = temp + bin_str[i][j+1];
            temp = temp + bin_str[i][j+2];
            temp = temp + bin_str[i][j+3];
            string t2 = bin_to_hex(temp);
            //cout<<t2;
            t.push_back(t2);
        }
        res.push_back(t);
        cout<<" ";
    }

    return res;
}


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

    vector<vector<char>> padding_str = padding(string_to_bits,len_str_in_bin); //split into 32-bits "words" ready for processing

    cout<<"Padding string size in blocks:-" << padding_str.size() << "\n";
    cout<<"String after padding:-\n";
    for(int i=0; i<padding_str.size();i++)
    {
        for(int j = 0; j<padding_str[i].size();j++)
        {
            cout<<padding_str[i][j];
        }
        cout<<" ";
    }
    
    vector<vector<string>> padded_to_hex = bin_str_to_hex(padding_str);

    return 0;





}