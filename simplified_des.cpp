// Simplified DES: 
// Plaintext block size: 8 bits
// Key size: 10 bits
// Number of Rounds : 2


#include<iostream>
#include<vector>
#include<random>
#include<algorithm>

using namespace std;

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
        bits[7-i] = (c >> i) & i;
    }
    return bits;
}

char bitsToChar(const vector<int>& bits) {
    char c = 0;
    for (int i = 0; i < 8; i++) {
        c = (c << 1) | bits[i];
    }
    return c;
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

string encrypt_first_round(vector<vector<int>> plaintext, vector<int> first_subkey)
{
    
    vector<int> ip = compression_permutation();

    for(int i=0; i<plaintext.size();i++)
    {
        vector<int> plaintext_to_be_processed = plaintext[i];
        vector<int> afterip;
        for(int j=0; j<ip.size();j++)
        {
            afterip.push_back(plaintext_to_be_processed[ip[j]]);
        }
        vector<int> left_half(afterip.begin(),afterip.begin()+4);
        vector<int> right_half(afterip.begin()+4,afterip.end());
        
        vector<int> ep = expansion_permutation();
        vector<int> ep_on_right;
        for(int j = 0; j<ep.size();j++)
        {
            ep_on_right.push_back(right_half[ep[j]]);
        }
        vector<int> res_on_xor;
        for(int j = 0; j<ep_on_right.size();j++)
        {
            res_on_xor.push_back(ep_on_right[j] ^ first_subkey[j]);
        }

        

    }

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

}


