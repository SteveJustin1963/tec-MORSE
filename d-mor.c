// morse code decoding 
Initialize an empty string "result" and an index variable "j" to store the decoded string and the current index in the result string, respectively.
Iterate over each character in the input string "str".
If the current character is a space or a forward slash, add a space to the "result" string and increment the index "j".
If the current character is a dot or a dash, search for the next character to form a code for an alphabet.
If the next character forms a valid code, add the corresponding alphabet to the "result" string and increment the index "j".
After iterating over the entire input string, add a null termination character to the "result" string.
Print the "result" string.
/////////////////////////

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
 

\\\\\\\\\\\\\\\\\\\
forth

: decode ( str len -- ) 
    create result len + 1 allot 
    0 swap 0 ?do
        dup i c@
        dup ' ' = 
        if
            ' ' result i + c!
            1+ 
        else
            dup '/' =
            if
                ' ' result i + c!
                1+
            else
                dup '-' = 
                if
                    i 1+ c@ '.' = 
                    if
                        'A' result i + c!
                        2 +
                    else
                        i 1+ c@ '-' = 
                        if
                            'B' result i + c!
                            2 +
                        then
                    then
                else
                    dup '.' = 
                    if
                        i 1+ c@ '-' =
                        if
                            'A' result i + c!
                            2 +
                        else
                            i 1+ c@ '.' =
                            if
                                'W' result i + c!
                                2 +
                            then
                        then
                    then
                then
            then
        then
    loop
    result len 0 c!
    result type
;
\\\\\\\\\\\\\\\\\\\\\\\\\\
// Driver program
int main()
{
    // input string
    char str[] = "... --- ...";
 
    // call the decode() function
    decode(str);
 
    return 0;
}
