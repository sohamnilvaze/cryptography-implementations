#include <bits/stdc++.h>
using namespace std;

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

// simple gcd
int gcd_int(int a, int b) {
    return b == 0 ? a : gcd_int(b, a % b);
}

// primality test (simple, OK for moderate p)
bool is_prime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; 1LL * i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

// factorize n and return distinct prime factors
vector<int> factorize(int n) {
    vector<int> factors;
    for (int p = 2; 1LL * p * p <= n; ++p) {
        if (n % p == 0) {
            factors.push_back(p);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

// find a primitive root modulo p (p must be prime)
int find_primitive_root(int p) {
    if (!is_prime(p)) return -1;
    int phi = p - 1;
    vector<int> factors = factorize(phi);
    for (int g = 2; g < p; ++g) {
        bool ok = true;
        for (int q : factors) {
            // if g^(phi/q) ≡ 1 (mod p) then g is NOT a primitive root
            if (mod_pow(g, phi / q, p) == 1) { ok = false; break; }
        }
        if (ok) return g;
    }
    return -1;
}

int main() {
    int p;
    cout << "Enter a prime modulus p: ";
    if (!(cin >> p)) return 0;

    if (!is_prime(p)) {
        cout << "Provided p is not prime. This program expects a prime modulus.\n";
        return 0;
    }

    int g = find_primitive_root(p);
    if (g == -1) {
        cout << "No primitive root found (unexpected for prime p).\n";
        return 0;
    }
    cout << "Found primitive root g = " << g << " (mod " << p << ")\n";

    long long Xa, Xb;
    cout << "Enter private key Xa for A (0 < Xa < " << p << "): ";
    cin >> Xa;
    cout << "Enter private key Xb for B (0 < Xb < " << p << "): ";
    cin >> Xb;
    if (Xa <= 0 || Xa >= p || Xb <= 0 || Xb >= p) {
        cout << "Private keys should be in range (0, p).\n";
        return 0;   
    }

    // public values (sent over insecure channel)
    long long Ya = mod_pow(g, Xa, p); // A -> publishes Ya
    long long Yb = mod_pow(g, Xb, p); // B -> publishes Yb
    cout << "A publishes Ya = g^Xa mod p = " << Ya << "\n";
    cout << "B publishes Yb = g^Xb mod p = " << Yb << "\n";

    // shared secrets
    long long shared_from_A = mod_pow(Yb, Xa, p); // (g^Xb)^Xa = g^(Xa*Xb)
    long long shared_from_B = mod_pow(Ya, Xb, p); // (g^Xa)^Xb
    cout << "Shared key computed by A = Yb^Xa mod p = " << shared_from_A << "\n";
    cout << "Shared key computed by B = Ya^Xb mod p = " << shared_from_B << "\n";

    if (shared_from_A == shared_from_B)
        cout << "Success: shared keys match.\n";
    else
        cout << "Failure: shared keys differ (bug).\n";

    return 0;


}
