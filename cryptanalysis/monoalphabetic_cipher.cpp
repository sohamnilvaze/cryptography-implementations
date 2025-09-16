#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<vector>
#include<random>
#include<map>

using namespace std;

static vector<float> relaive_letter_frequencies = {8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.996,0.153,0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,2.758,0.978,2.36,0.15,1.974,0.074};

void analyse_ciphertext(string text)
{
    map<char,int> mp;
    mp['A'] = 0;
    mp['B'] = 0;
    mp['C'] = 0;
    mp['D'] = 0;
    mp['E'] = 0;
    mp['F'] = 0;
    mp['G'] = 0;
    mp['H'] = 0;
    mp['I'] = 0;
    mp['J'] = 0;
    mp['K'] = 0;
    mp['L'] = 0;
    mp['M'] = 0;
    mp['N'] = 0;
    mp['O'] = 0;
    mp['P'] = 0;
    mp['Q'] = 0;
    mp['R'] = 0;
    mp['S'] = 0;
    mp['T'] = 0;
    mp['U'] = 0;
    mp['V'] = 0;
    mp['W'] = 0;
    mp['X'] = 0;
    mp['Y'] = 0;
    mp['Z'] = 0;
    int n = text.size();
    for(int i=0; i<text.size();i++)
    {
        mp[text[i]]++;
    }

    for(auto it: mp)
    {
        cout<<it.first<<" "<<it.second<<"\t";
    }
}

int main()
{
    string ciphertext;
    cout<<"Enter the cipher text";
    //MLVXFMPXYFUFPSFP

    analyse_ciphertext("MLVXFMPXYFUFPSFP"); 
    return 0;

}