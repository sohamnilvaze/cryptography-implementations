#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<cctype>
#include<math.h>
using namespace std;

int extended_euclidean_gcd(int a, int b, int &x, int &y)
{
    // goal is to find ax + by = gcd(a,b)
    if(b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    int x1,y1;
    int g = extended_euclidean_gcd(b, a%b, x1,y1);
    x = y1;
    y = x1 - (a / b)*y1;
    return g;
}

int mod_inverse(int a,int m)
{
    //goal is to find x and then adjust x such that a.x = 1 (mod m); 
    int x,y;
    int g = extended_euclidean_gcd(a,m,x,y);
    if(g!= 1)
    {
        cout<<"Modular inverse does not exists.(numbers are not coprime)";
        return -1;
    }

    return (x % m + m) % m;
}

long long chinese_remainder_theorem(const vector<int> &remainders, const vector<int> &divisors)
{
    long long N = 1;
    for(int d: divisors)
    {
        N = N*d;
    }

    long long result = 0;
    for(int i=0; i<divisors.size();i++)
    {
        long long Ni = N / divisors[i];
        long long inv = mod_inverse(Ni % divisors[i],divisors[i]);
        cout<<Ni<<" "<<inv<<"\n";
        if(inv == -1)
        {
            return -1;
        }
        cout<<(long long)remainders[i] * inv * Ni<<"\n";
        result = result + (long long)remainders[i] * inv * Ni;
    }
    cout<<"Final res: "<<result<<" after processing "<<(result % N + N) % N<<"\n";
    return (result % N + N) % N;


}

int main()
{
    vector<int> remainders,divisors;
    int n;
    cout<<"Enter the number of input pairs:\n";
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int rem,div;
        cout<<"Enter the remainder:\n";
        cin>>rem;
        cout<<"Enter the divisor:\n";
        cin>>div;
        remainders.push_back(rem);
        divisors.push_back(div);
    }

    long long solution = chinese_remainder_theorem(remainders,divisors);
    if(solution != -1)
    {
        cout<<"Solution x = "<<solution<< " (mod ";
        long long mod = 1;
        for(int d : divisors)
        {
            mod = mod * d;
        }
        cout<< mod << ")"<<endl;
    }

    return 0;



}