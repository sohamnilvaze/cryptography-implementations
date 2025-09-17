// Simplified DES: 
// Plaintext block size: 8 bits
// Key size: 10 bits
// Number of Rounds : 2


#include<iostream>
#include<vector>
#include<random>
#include<algorithm>
#include<sstream>
#include<iomanip>

using namespace std;

static vector<vector<int>> S_box_0 = {{1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2}};
static vector<vector<int>> S_box_1 = {{0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3}};

vector<int> initial_permutation(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1,10);

    vector<int> ip;
    while(ip.size() < 10)
    {
        int ri = distrib(gen);
        if(find(ip.begin(),ip.end(),ri) == ip.end())
        {
            ip.push_back(ri);
        }
    }

    return ip;

}

vector<int> compression_permutation(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1,10);

    vector<int> ip;
    while(ip.size() < 8)
    {
        int ri = distrib(gen);
        if(find(ip.begin(),ip.end(),ri) == ip.end())
        {
            ip.push_back(ri);
        }
    }

    return ip;
}

vector<int> expansion_permutation(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1,4);

    vector<int> ep;
    while(ep.size() < 8)
    {
        int ri = distrib(gen);
        ep.push_back(ri);
    }

    return ep;
}

vector<int> compression_permutation_4(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1,4);

    vector<int> ip;
    while(ip.size() < 4)
    {
        int ri = distrib(gen);
        if(find(ip.begin(),ip.end(),ri) == ip.end())
        {
            ip.push_back(ri);
        }
    }

    return ip;
}

vector<int> compression_permutation_8(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1,8);

    vector<int> ip;
    while(ip.size() < 8)
    {
        int ri = distrib(gen);
        if(find(ip.begin(),ip.end(),ri) == ip.end())
        {
            ip.push_back(ri);
        }
    }

    return ip;
}



vector<int> process_entered_key(string key)
{
    vector<int> processed_key;
    for(int i=0; i<key.length();i++)
    {
        if(key[i] == '0')
        {
            processed_key.push_back(0);
        }
        else{
            processed_key.push_back(1);
        }
    }

    return processed_key;
}

vector<int> left_1_bit_circular_shift(vector<int> key) {
    vector<int> new_key(key.size());
    for (int i = 0; i < key.size(); i++) {
        new_key[i] = key[(i+1) % key.size()];
    }
    return new_key;
}

vector<int> left_2_bit_circular_shift(vector<int> key) {
    vector<int> new_key(key.size());
    for (int i = 0; i < key.size(); i++) {
        new_key[i] = key[(i+2) % key.size()];
    }
    return new_key;
}

vector<vector<int>> subkey_generation(vector<int> key)
{
    vector<int> ip = initial_permutation();
    //performing initial permutation on key
    vector<int> permutated_key;
    for(int i = 0; i< key.size();i++)
    {
        permutated_key.push_back(key[ip[i]-1]);
    }
    //Dividing the key into two halves: left and right
    vector<int> left_key(permutated_key.begin(),permutated_key.begin()+5);
    vector<int> right_key(permutated_key.begin() + 5,permutated_key.end());

    //performing 1 bit cicrular left shift on the two parts
    vector<int> new_left_key = left_1_bit_circular_shift(left_key);
    vector<int> new_right_key = left_1_bit_circular_shift(right_key);

    //merging both the halves
    new_left_key.insert(new_left_key.end(),new_right_key.begin(),new_right_key.end());
    vector<int> merged_first = new_left_key;

    //generating 1st subkey: for 1st round
    vector<int> cp = compression_permutation();
    vector<int> first_subkey;
    for(int i = 0; i<cp.size();i++)
    {
        first_subkey.push_back(merged_first[cp[i]-1]);
    }

    vector<int> new_left_key_2(merged_first.begin(),merged_first.begin()+5);
    vector<int> new_right_key_2(merged_first.begin()+5,merged_first.end());
    
    //performing left circular 2 bit shift
    vector<int> new_left_key_3 = left_2_bit_circular_shift(new_left_key_2);
    vector<int> new_right_key_3 = left_2_bit_circular_shift(new_right_key_2);
    new_left_key_3.insert(new_left_key_3.end(),new_right_key_3.begin(),new_right_key_3.end());
    vector<int> merged_second = new_left_key_3;

    vector<int> second_subkey;
    for(int i = 0; i<cp.size();i++)
    {
        second_subkey.push_back(merged_second[cp[i] - 1]);
    }

    vector<vector<int>> res;
    res.push_back(first_subkey);
    res.push_back(second_subkey);
    res.push_back(ip);
    res.push_back(cp);

    return res;
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
    for(int i = 7;i>=0;i--)
    {
        bits[7-i] = (c >> i) & 1;
    }
    return bits;
}

char bitsToChar(const vector<int>& bits) {
    char c = 0;
    for (int i = 0; i < 8; i++) {
        c = (c << 1) | bits[i];
    }
    cout<<c - 'A'<<"\n";
    if(c - 'A' > 26)
    {
        char temp = ((c-'A')%26) + 'A';
        cout<<temp<<"\n";
    }
    else{
        cout<<c<<"\n";
    }
    return c;
}

string bitsToHex(const vector<int>& bits) {
    int value = 0;
    for (int b : bits) {
        value = (value << 1) | b;
    }
    stringstream ss;
    ss << hex << uppercase << setw(2) << setfill('0') << value;
    return ss.str();
}

vector<vector<int>> process_plaintext(string plaintext)
{
    vector<vector<int>> processed_plaintext;
    for(int i=0; i<plaintext.size();i++)
    {
        vector<int> bits = chartobits(plaintext[i]);
        processed_plaintext.push_back(bits);
    }

    return processed_plaintext;
}

string process_ciphertext(vector<vector<int>>cipher)
{
    string proc_cipher = "";
    for(int i=4;i<cipher.size();i++)
    {
        char c = bitsToChar(cipher[i]);
        proc_cipher = proc_cipher + c;
    }
    return proc_cipher;
}

vector<int> process_with_s_box(vector<vector<int>> s_box,int l1,int r1,int l2,int r2)
{
    int row = (2*l1) + r1;
    int col = (2*l2) + r2;
    if(s_box[row][col] == 0)
    {
        return {0,0};
    }
    else if(s_box[row][col] == 1)
    {
        return {0,1};
    }
    else if(s_box[row][col] == 2)
    {
        return {1,0};
    }
    return {1,1};
}

vector<vector<int>> encrypt(vector<vector<int>> plaintext, vector<int> first_subkey, vector<int> second_subkey)
{
    vector<vector<int>> to_ret;
    vector<int> ip = compression_permutation_8();
    vector<int> ep = expansion_permutation();
    vector<int> p4 = compression_permutation_4();
    vector<int> fp = compression_permutation_8();
    to_ret.push_back(ip);
    to_ret.push_back(ep);
    to_ret.push_back(p4);
    to_ret.push_back(fp);
    for(int i=0; i<plaintext.size();i++)
    {
        vector<int> plaintext_to_be_processed = plaintext[i];
        vector<int> afterip;
        for(int j=0; j<ip.size();j++)
        {
            afterip.push_back(plaintext_to_be_processed[ip[j]-1]);
        }
        vector<int> left_half(afterip.begin(),afterip.begin()+4);
        vector<int> right_half(afterip.begin()+4,afterip.end());
        
        
        vector<int> ep_on_right;
        for(int j = 0; j<ep.size();j++)
        {
            ep_on_right.push_back(right_half[ep[j]-1]);
        }
        vector<int> res_on_xor;
        for(int j = 0; j<ep_on_right.size();j++)
        {
            res_on_xor.push_back(ep_on_right[j] ^ first_subkey[j]);
        }
        vector<int> s_box_in_right;
        for(int i = 0;i<res_on_xor.size();i = i+4)
        {
            if(i == 0)
            {
                vector<int> res = process_with_s_box(S_box_0,res_on_xor[i],res_on_xor[i+1],res_on_xor[i+2],res_on_xor[i+3]);
                s_box_in_right.insert(s_box_in_right.end(),res.begin(),res.end());
            }
            else{
                vector<int> res = process_with_s_box(S_box_1,res_on_xor[i],res_on_xor[i+1],res_on_xor[i+2],res_on_xor[i+3]);
                s_box_in_right.insert(s_box_in_right.end(),res.begin(),res.end());
            }
        }

        
        vector<int> f_right;
        for(int i = 0;i<p4.size();i++)
        {
            f_right.push_back(s_box_in_right[p4[i]-1]);
        }

        vector<int> new_left = right_half;
        vector<int> new_right;
        for(int i=0; i<4;i++)
        {
            new_right.push_back(f_right[i]^left_half[i]);
        }

        new_left.insert(new_left.end(),new_right.begin(),new_right.end());


        vector<int> afterip2;
        for(int j=0; j<ip.size();j++)
        {
            afterip2.push_back(new_left[ip[j]-1]);
        }


        vector<int> left_half2(afterip2.begin(),afterip2.begin()+4);
        vector<int> right_half2(afterip2.begin()+4,afterip2.end());


        vector<int> ep_on_right2;
        for(int j = 0; j<ep.size();j++)
        {
            ep_on_right2.push_back(right_half2[ep[j]-1]);
        }

        vector<int> res_on_xor2;
        for(int j = 0; j<ep_on_right2.size();j++)
        {
            res_on_xor2.push_back(ep_on_right2[j] ^ second_subkey[j]);
        }


        vector<int> s_box_in_right2;
        for(int i = 0;i<res_on_xor2.size();i = i+4)
        {
            if(i == 0)
            {
                vector<int> res = process_with_s_box(S_box_0,res_on_xor2[i],res_on_xor2[i+1],res_on_xor2[i+2],res_on_xor2[i+3]);
                s_box_in_right2.insert(s_box_in_right2.end(),res.begin(),res.end());
            }
            else{
                vector<int> res = process_with_s_box(S_box_1,res_on_xor2[i],res_on_xor2[i+1],res_on_xor2[i+2],res_on_xor2[i+3]);
                s_box_in_right2.insert(s_box_in_right2.end(),res.begin(),res.end());
            }
        }

        vector<int> f_right2;
        for(int i = 0;i<p4.size();i++)
        {
            f_right2.push_back(s_box_in_right2[p4[i]-1]);
        }


        vector<int> new_left2 = right_half2;
        vector<int> new_right2;
        for(int i=0; i<4;i++)
        {
            new_right2.push_back(f_right2[i]^left_half2[i]);
        }

        new_left2.insert(new_left2.end(),new_right2.begin(),new_right2.end());


        //Final Swapping
        vector<int> new_left3(new_left2.begin()+4,new_left2.end());
        vector<int> new_right3(new_left2.begin(),new_left2.begin()+4);

        new_left3.insert(new_left3.end(),new_right3.begin(),new_right3.end());
        //Final permutation
        vector<int> cipher;
        for(int i=0; i<fp.size();i++)
        {
            cipher.push_back(new_left3[fp[i]-1]);
        }

        to_ret.push_back(cipher);
    }

    return to_ret;

}


int main()
{
    string key,plaintext;
    cout<<"Enter the key in binary form(10 bits):\n";
    cin>>key;
    cout<<"Enter the plaintext:\n";
    cin.ignore();
    getline(cin,plaintext);

    string processed_text = remove_spaces_and_upper(plaintext);

    vector<vector<int>> broken_plaintext = process_plaintext(processed_text);

    vector<int> processed_key = process_entered_key(key);
    cout<<"Processed key: ";
    for(int i=0; i<processed_key.size();i++)
    {
        cout<<processed_key[i];
    }
    cout<<"\n";
    vector<vector<int>> res = subkey_generation(processed_key);

    vector<int> first_subkey = res[0];
    vector<int> second_subkey = res[1];
    vector<int> ip = res[2];
    vector<int> cp = res[3];

    cout<<"First subkey: ";
    for(int i=0; i<first_subkey.size();i++)
    {
        cout<<first_subkey[i];
    }
    cout<<"\n";

    cout<<"Second subkey: ";
    for(int i=0; i<second_subkey.size();i++)
    {
        cout<<second_subkey[i];
    }
    cout<<"\n";

    vector<vector<int>> after_enc = encrypt(broken_plaintext,first_subkey,second_subkey);
    vector<int> ip_e = after_enc[0];
    vector<int> ep = after_enc[1];
    vector<int> p4 = after_enc[2];
    vector<int> fp = after_enc[3];

    vector<vector<int>> cipher_to_proc(after_enc.begin()+4,after_enc.end());

    cout<<"Final ciphertext in hex:\n";
    for(auto &blocks: cipher_to_proc)
    {
        cout<< bitsToHex(blocks) <<" ";
    }

    return 0;


}


