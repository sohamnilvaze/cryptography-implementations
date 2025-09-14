#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<vector>
#include<random>

using namespace std;

//Key space of ceaser cipher is only 25 and hence it is easy to carry out a brute force attack by searching with every value of the key from 0 to 25.

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

vector<string> cryptanalysis(string cipher)
{
    vector<string> cryptanalysis_results;
    for(int i=1;i<=25;i++)
    {
        int key = i;
        string temp = "";
        for(int j = 0; j < cipher.length();j++)
        {
            char c = ((cipher[j] - 'A' - key + 26) % 26) + 'A';
            temp = temp + c;
        }
        cryptanalysis_results.push_back(temp);
    }
    return cryptanalysis_results;

}

int main()
{
    string cipher;
    cout<<"Enter the cipher text:\n";
    cin>>cipher;

    string processed_cipher = remove_spaces(cipher);

    vector<string> cryptanalysis_res = cryptanalysis(processed_cipher);

    cout<<"The plaintext can be:\n";
    for(int i=0; i< cryptanalysis_res.size();i++)
    {
        cout<<cryptanalysis_res[i]<<"\n";
    }

    return 0;




}