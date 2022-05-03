// morse code decoding 

#include <stdio.h>
#include <string.h>
 
// Morse code table
char morse_code[26][5] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
 
// Function to decode Morse code
void decode(char *str)
{
    // result stores decoded string
    char result[1000];
 
    // result index
    int j = 0;
 
    // iterate over the input string
    for (int i = 0; str[i]; i++) {
 
        // if str[i] is ' ', then store a space
        // in result
        if (str[i] == ' ') {
            result[j++] = ' ';
        }
 
        // if str[i] is '/', then store a space
        // in result
        else if (str[i] == '/') {
            result[j++] = ' ';
        }
 
        // if str[i] is '-', then search for next
        // '-' and '.' to form a code for a
        // alphabet
        else if (str[i] == '-') {
            int k = i + 1;
 
            // if str[k] is '.', then we have
            // ".-" which is code for 'A'
            if (str[k] == '.') {
                result[j++] = 'A';
                i = i + 1;
            }
 
            // if str[k] is '-', then we have
            // "-..." which is code for 'B'
            else if (str[k] == '-') {
                result[j++] = 'B';
                i = i + 1;
            }
        }
 
        // if str[i] is '.', then search for next
        // '.' and '-' to form code for a
        // alphabet
        else if (str[i] == '.') {
            int k = i + 1;
 
            // if str[k] is '-', then we have
            // ".-" which is code for 'A'
            if (str[k] == '-') {
                result[j++] = 'A';
                i = i + 1;
            }
 
            // if str[k] is '.', then we have
            // ".--" which is code for 'W'
            else if (str[k] == '.') {
                result[j++] = 'W';
                i = i + 1;
            }
        }
    }
 
    // '\0' terminate the string result
    result[j] = '\0';
 
    // print the string result
    printf("%s\n", result);
}
 
// Driver program
int main()
{
    // input string
    char str[] = "... --- ...";
 
    // call the decode() function
    decode(str);
 
    return 0;
}
