#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char text[100], result[100];
    int choice, key;
    
    printf("Enter the key: ");
    scanf("%d", &key);
    
    printf("Enter 1 for encrypting, 2 for decrypting: ");
    scanf("%d", &choice);
    getchar(); // to consume leftover newline
    
    printf("Enter the text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // remove trailing newline
    
    int j = 0;
    for(int i = 0; i < strlen(text); i++) {
        char c = text[i];
        
        if(isupper(c)) { // uppercase letters
            if(choice == 1) // encrypt
                result[j++] = ((c - 'A' + key) % 26) + 'A';
            else // decrypt
                result[j++] = ((c - 'A' - key + 26) % 26) + 'A';
        }
        else if(islower(c)) { // lowercase letters
            if(choice == 1)
                result[j++] = ((c - 'a' + key) % 26) + 'A';
            else
                result[j++] = ((c - 'a' - key + 26) % 26) + 'A';
        }
        else { // spaces or punctuation remain unchanged
            result[j++] = c;
        }
    }
    result[j] = '\0';
    
    if(choice == 1)
        printf("Encrypted text: %s\n", result);
    else
        printf("Decrypted text: %s\n", result);
    
    return 0;
}
