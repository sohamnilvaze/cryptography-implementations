#include <iostream>
#include <vector>
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

// simple (small) primality test
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

// simple gcd
int gcd_int(int a, int b) {
    return b == 0 ? a : gcd_int(b, a % b);
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


class Party {
    string name;
    long long private_key = 0;   // secret exponent (a or b)
    long long public_key = 0;    // g^private mod p
    long long shared_key = 0;    // computed shared secret
public:
    // set private key and compute public key
    Party(string n=""): name(n), private_key(0), public_key(0), shared_key(-1){}
    void set_private(long long priv) {
        private_key = priv;
    }

    void compute_public(long long g,long long p)
    {
        public_key = mod_pow(g,private_key,p);
    }

    long long get_public() const { return public_key; }
    long long get_private() const { return private_key; }
    long long get_shared() const { return shared_key; }

    // compute shared key given other party's public value
    void compute_shared_from_other_public(long long other_public, long long p) {
        shared_key = mod_pow(other_public, private_key, p);
    }

    string get_name()
    {
        return name;
    }
};

class Eve
{
    long long priv_for_A;
    long long priv_for_B;
    long long pub_for_A;
    long long pub_for_B;
    long long shared_with_A;
    long long shared_with_B;

    public:
        Eve() : priv_for_A(0), priv_for_B(0), pub_for_A(0), pub_for_B(0), shared_with_A(-1), shared_with_B(-1){}

        void set_privates(long long priv_a, long long priv_b)
        {
            priv_for_A = priv_a;
            priv_for_B = priv_b;
        }
        void generate_pub_for_A(long long g, long long p)
        {
            pub_for_A = mod_pow(g, priv_for_A,p);
        }

        void generate_pub_for_B(long long g, long long p)
        {
            pub_for_B = mod_pow(g, priv_for_B,p);
        }

        void generate_shared_for_A(long long A_pub, long long p)
        {
            shared_with_A = mod_pow(A_pub,priv_for_A,p);
        }

        void generate_shared_for_B(long long B_pub,long long p)
        {
            shared_with_B = mod_pow(B_pub, priv_for_B,p);
        }

        long long send_to_A()
        {
            return pub_for_A;
        }

        long long send_to_B()
        {
            return pub_for_B;
        }

        long long send_shared_to_A()
        {
            return shared_with_A;
        }

        long long send_shared_to_B()
        {
            return shared_with_B;
        }


};

int main() {
    long long p;
    cout << "Enter a prime modulus p: ";
    if (!(cin >> p)) return 0;
    if (!is_prime((int)p)) {
        cout << "Provided p is not prime. Exiting.\n";
        return 0;
    }

    long long g = find_primitive_root(p);
    Party A("Alice"), B("Bob");
    Eve C;

    long long a_priv, b_priv;
    cout<<"Enter private key for A(between 0 and " << p << ")\n";
    cin>>a_priv;
    cout<<"Enter private key for B(between 0 and " << p << ")\n";
    cin>>b_priv;

    long long eve_priv_for_A, eve_priv_for_B;
    cout<<"Enter private key for Eve C for its connection with A(between 0 and " << p << ")\n";
    cin>>eve_priv_for_A;
    cout<<"Enter private key for Eve C for its connection with B(between 0 and " << p << ")\n";
    cin>>eve_priv_for_B;

    A.set_private(a_priv);
    B.set_private(b_priv);
    A.compute_public(g,p);
    B.compute_public(g,p);

    C.set_privates(eve_priv_for_A,eve_priv_for_B);
    C.generate_pub_for_A(g,p);
    C.generate_pub_for_B(g,p);
    

    cout<<"If there was no man-in-the-middle attack then the shared key would have been:\n";
    A.compute_shared_from_other_public(B.get_public(),p);
    B.compute_shared_from_other_public(A.get_public(),p);
    long long shared_normal_from_A = A.get_shared();
    long long shared_normal_from_B = B.get_shared();
    cout<<"Shared from A: "<< shared_normal_from_A << " Shared from B: " << shared_normal_from_B << "\n";

    cout<<"In case of man-in-the-middle attack:\n";
    cout<<"Alice public key: " << A.get_public() << "\n";
    cout<<"Bob public key: " << B.get_public() << "\n";

    cout<<"Alice sents the A_pub " << A.get_public() << " to Bob but C intercepts it and computes the shared key for A\n";
    C.generate_shared_for_A(A.get_public(),p);
    long long sent_to_B = C.send_to_B();
    cout<<"Now Eve instead of forwarding A's public to B, send B its own public key that is " << sent_to_B << "\n";

    cout<<" Bob receives the key from C assuming that it is sent by A itself. SO now it computes the shared key.\n";
    B.compute_shared_from_other_public(sent_to_B,p);
    cout<< "B computes the shared key: " << B.get_shared()<< "\n";
    long long B_pub_sent = B.get_public();
    cout<<"C intercepts the pyblic key from B " << B_pub_sent << " and instead of forwarding to A computes the shared key for B.\n";
    C.generate_shared_for_B(B_pub_sent,p);
    cout<<"C does not forward public key from B to A, but instrad sends A its own public key. which is " << C.send_to_A() << "\n";
    cout<<"Now A receives the public key received assuming from B but actually from C and computes the shared key.\n";
    A.compute_shared_from_other_public(C.send_to_A(),p);
    
    cout<<"Shared public key generated by A: " << A.get_shared() << "\n";

    cout<<"Summary\n";
    cout<<"Shared key on A side: "<< A.get_shared() << "\n";
    cout<<"Shared key on B side: "<< B.get_shared() << "\n";
    cout<<"C has shared keys for both sides "<< C.send_shared_to_A() << " and " << C.send_shared_to_B() << "\n";
    
    return 0;
}
