#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <cstdint>
using namespace std;


#define F(b,c,d) (b & c) | (!b & d) 
#define G(b,c,d) (b & d) | (c & !d)
#define H(b,c,d) (b ^ c ^ c)
#define I(b,c,d) (c ^ (b | !d))

uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;

string to_binary_string(unsigned int value) {
    bitset<32> bits(value);
    return bits.to_string();  // returns std::string of length 32
}

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

uint32_t bits_to_uint32(vector<char>& bits) {
    if (bits.size() != 32) {
        throw std::invalid_argument("Expected exactly 32 bits");
    }
    
    uint32_t result = 0;
    for (size_t i = 0; i < 32; ++i) {
        if (bits[i] == '1') {
            result |= (1u << (31 - i)); // MSB first
        } 
        else if (bits[i] != '0') {
            throw invalid_argument("Bits must be '0' or '1'");
        }
    }
    return result;
}

vector<uint32_t> first_round(vector<vector<char>> padded,uint32_t A, uint32_t B, uint32_t C, uint32_t D)
{
    vector<uint32_t> ks = {0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501, 0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD9886BA, 0xAEF127FA, 0x4292244};
    // 1,5,9,13 = 7;
    // 2,6,10,14 = 12;
    // 3,7,11,15 = 17;
    // 4,8,12,16 = 22;
    for(int i = 0; i<padded.size();i++)
    {
        uint32_t f_bcd = F(B,C,D);
        uint32_t f_plus_a = f_bcd + A;
        uint32_t m_con = bits_to_uint32(padded[i]);
        uint32_t after_m = f_plus_a + m_con;
        uint32_t after_k =  after_m + ks[i];
        uint32_t after_shift;
        if((i+1) % 4 == 1)
        {
            after_shift = 7 << after_k;
        }
        else if((i+1) % 4 == 2)
        {
            after_shift = 12 << after_k;
        }
        else if((i+1) % 4 == 3)
        {
            after_shift = 17 << after_k;
        }
        else
        {
            after_shift = 22 << after_k;
        }
        
        uint32_t after_b = after_shift + B;
        uint32_t A_temp = A;
        uint32_t B_temp = B;
        uint32_t C_temp = C;
        uint32_t D_temp = D;

        B = after_b;
        A = D_temp;
        C = B_temp;
        D = C_temp;

        
    }

    vector<uint32_t> res;

    res.push_back(A);
    res.push_back(B);
    res.push_back(C);
    res.push_back(D);

    return res;

}


vector<uint32_t> second_round(vector<vector<char>> padded,uint32_t A, uint32_t B, uint32_t C, uint32_t D)
{
    vector<uint32_t> ks = {0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8, 0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A};
    // 1,5,9,13 = 5;
    // 2,6,10,14 = 9;
    // 3,7,11,15 = 14;
    // 4,8,12,16 = 20;
    for(int i = 0; i<padded.size();i++)
    {
        uint32_t g_bcd = G(B,C,D);
        uint32_t g_plus_a = g_bcd + A;
        uint32_t m_con = bits_to_uint32(padded[i]);
        uint32_t after_m = g_plus_a + m_con;
        uint32_t after_k =  after_m + ks[i];
        uint32_t after_shift;
        if((i+1) % 4 == 1)
        {
            after_shift = 5 << after_k;
        }
        else if((i+1) % 4 == 2)
        {
            after_shift = 9 << after_k;
        }
        else if((i+1) % 4 == 3)
        {
            after_shift = 14 << after_k;
        }
        else
        {
            after_shift = 20 << after_k;
        }
        
        uint32_t after_b = after_shift + B;
        uint32_t A_temp = A;
        uint32_t B_temp = B;
        uint32_t C_temp = C;
        uint32_t D_temp = D;

        B = after_b;
        A = D_temp;
        C = B_temp;
        D = C_temp;

        
    }

    vector<uint32_t> res;

    res.push_back(A);
    res.push_back(B);
    res.push_back(C);
    res.push_back(D);

    return res;

}


vector<uint32_t> third_round(vector<vector<char>> padded,uint32_t A, uint32_t B, uint32_t C, uint32_t D)
{
    vector<uint32_t> ks = {0x85845700, 0x676F02D9, 0x8D2A4C8A, 0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70, 0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x0654D202, 0x2B8699E5};
    // 1,5,9,13 = 4;
    // 2,6,10,14 =1;
    // 3,7,11,15 = 16;
    // 4,8,12,16 = 13;
    for(int i = 0; i<padded.size();i++)
    {
        uint32_t h_bcd = H(B,C,D);
        uint32_t h_plus_a = h_bcd + A;
        uint32_t m_con = bits_to_uint32(padded[i]);
        uint32_t after_m = h_plus_a + m_con;
        uint32_t after_k =  after_m + ks[i];
        uint32_t after_shift;
        if((i+1) % 4 == 1)
        {
            after_shift = 4 << after_k;
        }
        else if((i+1) % 4 == 2)
        {
            after_shift = 11 << after_k;
        }
        else if((i+1) % 4 == 3)
        {
            after_shift = 16 << after_k;
        }
        else
        {
            after_shift = 13 << after_k;
        }
        
        uint32_t after_b = after_shift + B;
        uint32_t A_temp = A;
        uint32_t B_temp = B;
        uint32_t C_temp = C;
        uint32_t D_temp = D;

        B = after_b;
        A = D_temp;
        C = B_temp;
        D = C_temp;

        
    }

    vector<uint32_t> res;

    res.push_back(A);
    res.push_back(B);
    res.push_back(C);
    res.push_back(D);

    return res;

}



vector<uint32_t> fourth_round(vector<vector<char>> padded,uint32_t A, uint32_t B, uint32_t C, uint32_t D)
{
    vector<uint32_t> ks = {0xFFFAF361, 0xBEFA4FA4, 0x6B3F8785, 0x895CD7BE, 0x6B901122, 0xFD9886BA, 0xAEF127FA, 0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845700, 0x676F02D9};
    // 1,5,9,13 = 6;
    // 2,6,10,14 =10;
    // 3,7,11,15 = 15;
    // 4,8,12,16 = 21;
    for(int i = 0; i<padded.size();i++)
    {
        uint32_t i_bcd = I(B,C,D);
        uint32_t i_plus_a = i_bcd + A;
        uint32_t m_con = bits_to_uint32(padded[i]);
        uint32_t after_m = i_plus_a + m_con;
        uint32_t after_k =  after_m + ks[i];
        uint32_t after_shift;
        if((i+1) % 4 == 1)
        {
            after_shift = 6 << after_k;
        }
        else if((i+1) % 4 == 2)
        {
            after_shift = 10 << after_k;
        }
        else if((i+1) % 4 == 3)
        {
            after_shift = 115 << after_k;
        }
        else
        {
            after_shift = 21 << after_k;
        }
        
        uint32_t after_b = after_shift + B;
        uint32_t A_temp = A;
        uint32_t B_temp = B;
        uint32_t C_temp = C;
        uint32_t D_temp = D;

        B = after_b;
        A = D_temp;
        C = B_temp;
        D = C_temp;

        
    }

    vector<uint32_t> res;

    res.push_back(A);
    res.push_back(B);
    res.push_back(C);
    res.push_back(D);

    return res;

}







int main()
{
    string plaintext,A_fin,B_fin,C_fin,D_fin;
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
    cout<<"Size of padded_to_hex:- " << padded_to_hex.size() << " || " << padded_to_hex[0].size() << "\n";

    vector<uint32_t> first_round_res = first_round(padding_str,A,B,C,D);
    cout<<"After first round:- \n";
    uint32_t after_first_A = first_round_res[0];
    uint32_t after_first_B = first_round_res[1];
    uint32_t after_first_C = first_round_res[2];
    uint32_t after_first_D = first_round_res[3];
    cout<<"A:- " << after_first_A << "\n";
    cout<<"B:- " << after_first_B << "\n";
    cout<<"C:- " << after_first_C << "\n";
    cout<<"D:- " << after_first_D << "\n";



    vector<uint32_t> second_round_res = second_round(padding_str,after_first_A,after_first_B,after_first_C,after_first_D);
    cout<<"After second round:- \n";
    uint32_t after_second_A = second_round_res[0];
    uint32_t after_second_B = second_round_res[1];
    uint32_t after_second_C = second_round_res[2];
    uint32_t after_second_D = second_round_res[3];
    cout<<"A:- " << after_second_A << "\n";
    cout<<"B:- " << after_second_B << "\n";
    cout<<"C:- " << after_second_C << "\n";
    cout<<"D:- " << after_second_D << "\n";


    vector<uint32_t> third_round_res = third_round(padding_str,after_second_A,after_second_B,after_second_C,after_second_D);
    cout<<"After third round:- \n";
    uint32_t after_third_A = third_round_res[0];
    uint32_t after_third_B = third_round_res[1];
    uint32_t after_third_C = third_round_res[2];
    uint32_t after_third_D = third_round_res[3];
    cout<<"A:- " << after_third_A << "\n";
    cout<<"B:- " << after_third_B << "\n";
    cout<<"C:- " << after_third_C << "\n";
    cout<<"D:- " << after_third_D << "\n";


    vector<uint32_t> fourth_round_res = fourth_round(padding_str,after_third_A,after_third_B,after_third_C,after_third_D);
    cout<<"After fourth round:- \n";
    uint32_t after_fourth_A = fourth_round_res[0];
    uint32_t after_fourth_B = fourth_round_res[1];
    uint32_t after_fourth_C = fourth_round_res[2];
    uint32_t after_fourth_D = fourth_round_res[3];
    cout<<"A:- " << after_fourth_A << "\n";
    cout<<"B:- " << after_fourth_B << "\n";
    cout<<"C:- " << after_fourth_C << "\n";
    cout<<"D:- " << after_fourth_D << "\n";


    A_fin = to_binary_string(after_fourth_A);
    B_fin = to_binary_string(after_fourth_B);
    C_fin = to_binary_string(after_fourth_C);
    D_fin = to_binary_string(after_fourth_D);

    string final_md5_hash = A_fin + B_fin + C_fin + D_fin;

    cout<<"Final 128 bit hash value is:- " << final_md5_hash << "\n";






    return 0;





}