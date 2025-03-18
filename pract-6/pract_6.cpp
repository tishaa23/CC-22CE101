#include <stdio.h>
#include <string.h>

char s[100];  
int i = 0, flag = 0;  

void S();  
void L();
void Ld();

// Recursive Descent Parser functions

void S() {
    if (s[i] == 'a') {  // Case: S → a
        i++;
    } 
    else if (s[i] == '(') {  // Case: S → ( L )
        i++;  // Move past '('
        L();
        if (s[i] == ')') {
            i++;  // Move past ')'
        } else {
            flag = 1;  // Invalid string
        }
    } 
    else {
        flag = 1;  // Invalid string
    }
}

void L() {
    S();  // First call S
    Ld(); // Then call L'
}

void Ld() {
    if (s[i] == ',') {  // Case: L' → , S L'
        i++;  // Move past ','
        S();
        Ld();  // Recursively check for more elements
    }
    // Case: L' → ε (do nothing, base case)
}

int main() {
    printf("Enter the string to validate: ");
    scanf("%s", s);

    i = 0;
    flag = 0;
    
    S();  // Start parsing from S

    if (s[i] == '\0' && flag == 0) {  // Ensure the entire string is parsed
        printf("Valid string\n");
    } else {
        printf("Invalid string\n");
    }

    return 0;
}