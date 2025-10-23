#include "md5.h"

uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;

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

}
