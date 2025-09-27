#include<iostream>
#include<vector>
#include<random>
#include<algorithm>
#include<sstream>
#include<iomanip>
#include<cmath>

using namespace std;

vector<int> find_reduced_residues(int n)
{
    vector<int> res;
    res.push_back(1);
    for(int i=2;i<n;i++)
    {
        if(gcd(i,n) == 1)
        {
            res.push_back(i);
        }
    }
    return res;
}



bool is_prime(int n)
{
    int no_of_factors = 0;
    for(int i=1;i<n+1;i++)
    {
        if(n % i == 0)
        {
            no_of_factors++;
        }
    }

    return no_of_factors == 2;

}

bool is_all(vector<int> all_mods, int n)
{
    if(is_prime(n))
    {
        for(int i=1;i<n;i++)
        {
            if(find(all_mods.begin(),all_mods.end(),i) == all_mods.end())
            {
                return false;
            }
        }
    }
    else{
        vector<int> red_res = find_reduced_residues(n);
        for(int i=0;i<red_res.size();i++)
        {
            if(find(all_mods.begin(),all_mods.end(),red_res[i]) == all_mods.end())
            {
                return false;
            }
        }
    }
    return true;
}

int gcd(int a, int b)
{
    if(b == 0)
    {
        return a;
    }

    return gcd(b,a%b);
}


int find_primitive_root(int n)
{
    if(is_prime(n))
    {
        for(int i=1; i<n;i++)
        {
            vector<int> temp;
            for(int j=0; j<n-1;j++)
            {
                temp.push_back((int)(pow((double)i,(double)j))%n);
            }
            if(is_all(temp,n))
            {
                return i;
            }
        }
        
    }

    else{
        vector<int> reduced_res = find_reduced_residues(n);
        cout<<"Reduced root of "<<n<<"\n";
        for(int i=0;i<reduced_res.size();i++)
        {
            cout<<reduced_res[i]<<", ";
        }
        cout<<"\n";
        for(int i=0; i<reduced_res.size();i++)
        {
            vector<int> temp;
            for(int j=0; j<n-1;j++)
            {
                temp.push_back((int)(pow((double)reduced_res[i],(double)j))%n);
            }
            for(int k=0;k<temp.size();k++)
            {
                cout<<temp[k]<<", ";
            }
            cout<<"\n";
            if(is_all(temp,n))
            {
                return reduced_res[i];
            }
        }
    }

    return -1;
}

int main()
{
    int num;
    cout<<"Enter a number:\n";
    cin>>num;

    int prim_root = find_primitive_root(num);
    cout<<"Primitive root:-"<<prim_root<<"\n";
}