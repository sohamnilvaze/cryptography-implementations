#include "md5.h"

uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;

//selecting a prime number q(10 bit) q = 101
string q = "1100101";

//selecting p such that p is prime number and p = qk + 1 so selecting p = 3031
string p = "101111010111";

//selecting h (1 < h < p-1) so here h = 1950
string h = "11110011110";

//computing g = h^k mod p such that g > 1
// so here h^2 = 1950*1950 = 3802500  h^k mod p = 3802500 % 3031 = 1626
string g = "11001011010";

//selecting a private key X such that [0 < x < q] so x = 67
string x = "1000011";

//selecting a secret key k such that 0 < k < q
int k = 59;

long long power_with_modulo(long long base, long long exp, long long mod) {
    long long res = 1;
    // Take modulo of base to handle cases where base >= mod
    base %= mod;

    while (exp > 0) {
        // If exp is odd, multiply result with current base
        if (exp % 2 == 1) { // or (exp & 1)
            // Perform multiplication and take modulo at each step to prevent overflow
            res = (res * base) % mod;
        }

        // Square the base and take modulo
        base = (base * base) % mod;
        // Halve the exponent (equivalent to exp = exp / 2 or exp >>= 1)
        exp >>= 1;
    }

    return res;
}

pair<int,int> signing_algorithm(int g, int p, int x, int y, int q, string final_hash)
{
    
    int temp = power_with_modulo(g,k,p);
    int r = temp % q;
    
}


int main()
{
    string plaintext,A_fin,B_fin,C_fin,D_fin;
    cout<<"Enter the plaintext:\n";
    getline(cin,plaintext);

    string processed_plaintext = remove_spaces_and_upper(plaintext);

    vector<vector<int>> string_to_bits = whole_string_to_bits(processed_plaintext);

    int len_str_in_bits = 8 * string_to_bits.size();
    
    //cout<<"\n The length of the string is:- " << len_str_in_bits << "\n";

    string len_str_in_bin = dec_to_binary(processed_plaintext.length());

    vector<vector<char>> padding_str = padding(string_to_bits,len_str_in_bin); //split into 32-bits "words" ready for processing

    // cout<<"Padding string size in blocks:-" << padding_str.size() << "\n";
    // cout<<"String after padding:-\n";
    // for(int i=0; i<padding_str.size();i++)
    // {
    //     for(int j = 0; j<padding_str[i].size();j++)
    //     {
    //         cout<<padding_str[i][j];
    //     }
    //     cout<<" ";
    // }
    
    vector<vector<string>> padded_to_hex = bin_str_to_hex(padding_str);
    //cout<<"Size of padded_to_hex:- " << padded_to_hex.size() << " || " << padded_to_hex[0].size() << "\n";

    vector<uint32_t> first_round_res = first_round(padding_str,A,B,C,D);
    //cout<<"After first round:- \n";
    uint32_t after_first_A = first_round_res[0];
    uint32_t after_first_B = first_round_res[1];
    uint32_t after_first_C = first_round_res[2];
    uint32_t after_first_D = first_round_res[3];
    //cout<<"A:- " << after_first_A << "\n";
    //cout<<"B:- " << after_first_B << "\n";
    //cout<<"C:- " << after_first_C << "\n";
    //cout<<"D:- " << after_first_D << "\n";



    vector<uint32_t> second_round_res = second_round(padding_str,after_first_A,after_first_B,after_first_C,after_first_D);
    //cout<<"After second round:- \n";
    uint32_t after_second_A = second_round_res[0];
    uint32_t after_second_B = second_round_res[1];
    uint32_t after_second_C = second_round_res[2];
    uint32_t after_second_D = second_round_res[3];
    //cout<<"A:- " << after_second_A << "\n";
    //cout<<"B:- " << after_second_B << "\n";
    //cout<<"C:- " << after_second_C << "\n";
    //cout<<"D:- " << after_second_D << "\n";


    vector<uint32_t> third_round_res = third_round(padding_str,after_second_A,after_second_B,after_second_C,after_second_D);
    //cout<<"After third round:- \n";
    uint32_t after_third_A = third_round_res[0];
    uint32_t after_third_B = third_round_res[1];
    uint32_t after_third_C = third_round_res[2];
    uint32_t after_third_D = third_round_res[3];
    //cout<<"A:- " << after_third_A << "\n";
    //cout<<"B:- " << after_third_B << "\n";
    //cout<<"C:- " << after_third_C << "\n";
    //cout<<"D:- " << after_third_D << "\n";


    vector<uint32_t> fourth_round_res = fourth_round(padding_str,after_third_A,after_third_B,after_third_C,after_third_D);
    //cout<<"After fourth round:- \n";
    uint32_t after_fourth_A = fourth_round_res[0];
    uint32_t after_fourth_B = fourth_round_res[1];
    uint32_t after_fourth_C = fourth_round_res[2];
    uint32_t after_fourth_D = fourth_round_res[3];
    //cout<<"A:- " << after_fourth_A << "\n";
    //cout<<"B:- " << after_fourth_B << "\n";
    //cout<<"C:- " << after_fourth_C << "\n";
    //cout<<"D:- " << after_fourth_D << "\n";


    A_fin = to_binary_string(after_fourth_A);
    B_fin = to_binary_string(after_fourth_B);
    C_fin = to_binary_string(after_fourth_C);
    D_fin = to_binary_string(after_fourth_D);

    string final_md5_hash = A_fin + B_fin + C_fin + D_fin;

    cout<<"Final 128 bit hash value is:- " << final_md5_hash << "\n";

    //computing y = g^x mod p
    int g = 1626;
    int x = 67;
    int p = 3031;
    int q = 101;
    long long y = power_with_modulo(1626,67,3031);

    cout<<"Public key (p | g | x | y) is:- " << p << " | " << g << " | " << x << " | " << y << "\n";



}
