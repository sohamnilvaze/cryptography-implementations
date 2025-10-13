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

// fast modular exponentiation: computes (base^exp) % mod
long long mod_pow(long long base, long long exp, long long mod) {
    long long res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

//Function to determine whether a given number is prime or not
bool is_prime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; 1LL * i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}


//Function to find gcd of 2 numbers recursively
int gcd_int(int a, int b)
{
    if(b == 0)
    {
        return a;
    }
    return gcd_int(b,a%b);
}

//Function to determine whether the given two numbers are relatively prime or not
int is_relatively_prime(int a, int b)
{
    return gcd_int(a,b) == 1;
}

//Performing extended euclidean algorithm for given 2 numbers
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

//Function to find multiplicative inverse of 2 numbers using extended euclidean algorithm
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

// Encrypt plaintext (return space-separated cipher numbers)
string encrypt(int enc_key, string plaintext, int n) {
    vector<int> encrypted_values;
    for (char c : plaintext) {
        int cn = (int)c;
        int en = mod_pow(cn, enc_key, n);
        encrypted_values.push_back(en);
    }

    // convert to string for display
    string cipher = "";
    for (size_t i = 0; i < encrypted_values.size(); ++i) {
        cipher += to_string(encrypted_values[i]);
        if (i + 1 < encrypted_values.size()) cipher += " ";
    }
    return cipher;
}

// Decrypt numeric ciphertext string
string decrypt(int dec_key, string ciphertext, int n) {
    stringstream ss(ciphertext);
    vector<int> cipher_nums;
    int val;
    while (ss >> val) cipher_nums.push_back(val);

    string plain = "";
    for (int cn : cipher_nums) {
        int en = mod_pow(cn, dec_key, n);
        plain.push_back((char)en);
    }
    return plain;
}


int main()
{
    int p,q;
    string text;
    cout<<"Enter the value of p(should be prime number):-\n";
    cin>>p;
    cout<<"Enter the value of q(should be prime number):-\n";
    cin>>q;

    if(p == q)
    {
        cout<<"p and q must not be equal";
        return 1;
    }

    if(!is_prime(p) || !is_prime(q))
    {
        cout<<"p and q both must be prime.\n";
        return 1;
    }

    int n = p*q;
    int m = (p-1) * (q-1);
    cout<<"Value of n: "<< n << "\n";
    cout<<"Value of m: "<< m << "\n";

    int enc_key;
    cout<<"Enter the value of encryption key(should be greater than 1 and lesser than m and also relatively prime to m):\n";
    cin>>enc_key;

    if( enc_key<=1 || enc_key>=m)
    {
        cout<<"Incorrect value of encrypted key: "<< enc_key << "\n";
        return 1;
    }

    if(!is_relatively_prime(enc_key,m))
    {
        cout<<"Entered value of encrypted key not relatively prime to m.\n";
        return 1;
    }

    int decrypted_key = mod_inverse(enc_key,m);

    cout<<"Enter the plaintext:-\n";
    cin.ignore();
    getline(cin,text);

    string preprocessed_plaintext = remove_spaces_and_upper(text);

    string cipher = encrypt(enc_key,preprocessed_plaintext,n);
    cout<<"Ciphertext generated is:- " << cipher << "\n";

    string decrypted = decrypt(decrypted_key,cipher,n);

    cout<<"Revived decrypted text:- " << decrypted << "\n";

    return 0;








}


