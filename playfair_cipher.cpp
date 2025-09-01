#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>
#include<vector>
#include<random>

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

bool is_present(vector<vector<char>> mat, char let)
{
    for(int i=0; i<mat.size();i++)
    {
        for(int j = 0; j<mat[0].size();j++)
        {
            if(mat[i][j] == let)
            {
                return true;
            }
        }
    }

    return false;
}


vector<vector<char>> prepare_key_matrix(string key)
{
    vector<vector<char>> mat(5,vector<char>(5,'#'));
    int i = 0;
    int k = 0;
    int j;
    while(k < key.size() && i < 5)
    {
        j = 0;
        while(j<5 && k < key.size())
        {
            if(is_present(mat,key[k]) == false)
            {
                mat[i][j++] = key[k++];
            }
        }
        i++;
    }

    if(key.find("J") != string::npos || key.find("I") != string::npos)
    {
        vector<char> alphas = {'A','B','C','D','E','F','G','H','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        int k = 0;
        while(k < alphas.size() && i < 5)
        {
            if(j == 5)
            {
                j = 0;
            }
            while(j<5 && k < alphas.size())
            {
                if(is_present(mat,alphas[k]) == false)
                {
                    mat[i][j++] = alphas[k];
                }
                k++;
            }
            i++;
        }


    }

    else{
        vector<char> alphas = {'A','B','C','D','E','F','G','H','I','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        int k = 0;
        while(k < alphas.size() && i < 5)
        {
            if(j == 5)
            {
                j = 0;
            }
            while(j<5 && k < alphas.size())
            {
                if(is_present(mat,alphas[k]) == false)
                {
                    mat[i][j++] = alphas[k];
                }
                k++;
            }
            i++;
        }
    }

    return mat;

}

vector<string> prepare_plaintext(string plaintext)
{
    vector<string> res;
    int i = 0;
    while(i<plaintext.size())
    {
        if( i == plaintext.size() - 1)
        {
            string temp = plaintext.substr(i,1) + "X";
            res.push_back(temp);
            i = i+1;
        }
        else
        {
            if(plaintext[i] != plaintext[i+1])
            {
                string temp = plaintext.substr(i,2);
                res.push_back(temp);
                i = i+2;
            }
            else if(plaintext[i] == plaintext[i+1])
            {
                string temp = plaintext.substr(i,1) + "X";
                res.push_back(temp);
                i = i+1;
            }

        }
    }

    return res;
}

pair<int,int> return_rc(vector<vector<char>> mat,char c)
{
    pair<int, int> ans = {-1,-1};
    for(int i = 0; i<mat.size();i++)
    {
        for(int j =0;j<mat[0].size();j++)
        {
            if(mat[i][j] == c)
            {
                pair<int,int> temp = {i,j};
                return temp;
            }
        }
    }

    return ans;

}

string encrypt(vector<vector<char>> mat, vector<string> prep)
{
    string cipher = "";
    int n = mat.size();
    int m = mat[0].size();
    for(int i = 0; i<prep.size();i++)
    {
        string t = prep[i];
        char f = t[0];
        char s = t[1];
        pair<int,int> rc1 = return_rc(mat,f);
        pair<int,int> rc2 = return_rc(mat,s);
        int i1 = rc1.first;
        int j1 = rc1.second;
        int i2 = rc2.first;
        int j2 = rc2.second;
        if(i1 == i2)
        {
            char c1 = mat[i1][(j1+1)%m];
            char c2 = mat[i2][(j2+1)%m];
            string s1(1,c1);
            string s2(1,c2);
            string temp = s1 + s2;
            cipher = cipher + temp;
        }
        else if(j1 == j2)
        {
            char c1 = mat[(i1+1)%n][j1];
            char c2 = mat[(i2+1)%n][j2];
            string s1(1,c1);
            string s2(1,c2);
            string temp = s1 + s2;
            cipher = cipher + temp;    
        }
        else{
            char c1 = mat[i1][j2];
            char c2 = mat[i2][j1];
            string s1(1,c1);
            string s2(1,c2);
            string temp = s1 + s2;
            cipher = cipher + temp;
        }
        

    }

    return cipher;
}

string decrypt(vector<vector<char>> mat, string cipher)
{
    vector<string> pre;
    string res;
    for(int i = 0;i<cipher.size();i = i+2)
    {
        cout<<cipher.substr(i,2)<<"\n";
        pre.push_back(cipher.substr(i,2));
    }
    int n = mat.size();
    int m = mat[0].size();

    for(int i=0; i<pre.size();i++)
    {
        string t = pre[i];
        char f = t[0];
        char s = t[1];
        pair<int,int> rc1 = return_rc(mat,f);
        pair<int,int> rc2 = return_rc(mat,s);
        int i1 = rc1.first;
        int j1 = rc1.second;
        int i2 = rc2.first;
        int j2 = rc2.second;
        if(i1 == i2)
        {
            char c1 = mat[i1][((m + j1)-1)%m];
            char c2 = mat[i2][((m + j2)-1)%m];
            string s1(1,c1);
            string s2(1,c2);
            string temp = s1 + s2;
            res = res + temp;
        }
        else if(j1 == j2)
        {
            char c1 = mat[((n + i1)-1)%n][j1];
            char c2 = mat[((n + i2)-1)%n][j2];
            string s1(1,c1);
            string s2(1,c2);
            string temp = s1 + s2;
            res = res + temp;   
        }
        else{
            char c1 = mat[i1][j2];
            char c2 = mat[i2][j1];
            string s1(1,c1);
            string s2(1,c2);
            string temp = s1 + s2;
            res = res + temp;
        }
    }

    return res;
}


int main()
{
    string key = "SOHAM";
    key = toUpperCase(key);
    vector<vector<char>> mat = prepare_key_matrix(key);

    for(int i=0; i<5;i++)
    {
        for(int j = 0; j<5;j++)
        {
            cout<<mat[i][j]<<" ";
        }
        cout<<"\n";
    }

    string plaintext;
    cout<<"Enter the plaintext:\n";
    getline(cin,plaintext);
    string processed_plaintext = remove_spaces(plaintext);
    vector<string> prepared_plaintext = prepare_plaintext(processed_plaintext);


    string cipher = encrypt(mat,prepared_plaintext);
    cout<<cipher<<"\n";

    string res = decrypt(mat,cipher);
    cout<<res<<"\n";


    

    return 0;
}