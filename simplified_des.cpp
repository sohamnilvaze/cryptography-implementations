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



int main()
{
    string key;
    cout<<"Enter the key in binary form(10 bits):\n";
    cin>>key;

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


