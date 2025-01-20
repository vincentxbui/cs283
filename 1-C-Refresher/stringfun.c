#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int reverse_string(char *, int);
int print_words(char *, int, int);

//function gets the user input and stores it in buffer
//the length of the user supplied string is returned
int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *buff_ptr = buff;
    char *user_str_ptr = user_str;
    int len_str = 0;
    int consecutive_spaces = 0;

    //loop to skip spaces
    while (*user_str_ptr == ' ') {
        user_str_ptr++;
    }

    //loop to check each character
    while (*user_str_ptr != '\0') {
        //error if the string is too large
        if (buff_ptr - buff >= len) {
            printf("The user supplied string is too large.");
            return -1;
        }
        //copies character to buffer if it is not a space
        if (*user_str_ptr != ' ') {
            *buff_ptr = *user_str_ptr;
            buff_ptr++;
            len_str++;
            consecutive_spaces = 0;
        //ensures there is only a single space when copying to buffer
        } else if (!consecutive_spaces) {
            *buff_ptr = ' ';
            buff_ptr++;
            len_str++;
            consecutive_spaces = 1;
        }
        //goes to the next character
        user_str_ptr++;
    }

    //removes trailing space
    if (buff_ptr > buff && buff_ptr[-1]== ' ') {
        buff_ptr--;
        len_str--;
    }

    //fills remaining buffer with '.' characters
    while (buff_ptr - buff < len) {
        *buff_ptr = '.';
        buff_ptr++;
    }
    return len_str; //for now just so the code compiles. 
}

//function prints the entire buff
void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

//function prints usage example
void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

//function counts the number of words
//the number of words is returned
int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int word_count = 0;
    int word_exists = 0;

    //loop checks every character
    for (int i = 0; i < str_len; i++) {
        //checks if it is a valid character
        if (buff[i] != ' ' && buff[i] != '.') {
            //increments the word count if it is a new word
            if (!word_exists) {
                word_count++;
                word_exists = 1;
            }
        } else {
            word_exists = 0;
        }
    }
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
//function reverses the characters of a string
int reverse_string(char *buff, int str_len) {
    int start = 0;
    int end = str_len - 1;
    char temp;

    //loop swaps the characters
    while (start < end) {
        //stores temp with the start character
        temp = buff[start];
        //replaces the start character with the end
        buff[start] = buff[end];
        //replaces the end character with the temp
        buff[end] = temp;
        start++;
        end--;
    }
    return 0;
}

//function prints each word on its own line
int print_words(char *buff, int buff_len, int str_len) {
    int word_count = 0;
    int char_count = 0;
    int inside_word = 0;

    printf("Word Print\n");
    printf("----------\n");

    //loops through each character in string
    for (int i = 0; i < str_len; i++) {
        char c = buff[i];
        //checks if it is a valid character
        if (c != ' ' && c != '.') {
            //increments the count if it is a new word by checking if currently inside one
            if (!inside_word) {
                word_count++;
                inside_word = 1;
            }
            //increments the character count
            char_count++;
        //prints the word if character is ' ' or '.'
        } else {
            printf("%d. ", word_count);
            for (int j = i - char_count; j < i; j++) {
                putchar(buff[j]);
            }
            //prints the character count
            printf("(%d)\n", char_count);
            char_count = 0;
            inside_word = 0;;
        }
    }

    //prints the last word
    if (inside_word) {
        printf("%d. ", word_count);
        for (int i = str_len - char_count; i < str_len; i++) {
            putchar(buff[i]);
        }
        printf("(%d)\n", char_count);
    }
    printf("\nNumber of words returned: %d\n", word_count);
    return 0;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*This is safe because the conditions ensure that the program has enough arguments and a valid flag. They check whether there are at least two arguments and if argv[1] is a flag. If argv[1] does not exist, there is no flag to be checked for printing, counting, reversing, writing, or replacing. If these conditions do not pass, the program exits.*/
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*The purpose of the if statement is to check if theres a third argument for the string. If there is not a string to be operated on, the program will exit.*/
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            rc = reverse_string(buff, user_str_len);
            if (rc < 0) {
                printf("Error reversing string, rc = %d", rc);
                exit(2);
            }
            break;      
        case 'w':
            rc = print_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error printing words, rc = %d", rc);
                exit(2);
            }
            break;
        case 'x':
            printf("Not Implemented!\n");
            exit(0);
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
/*The pointer and length is a good practice for helper functions because they allow programs to find data and know when that data ends. Pointers are addresses in memory and allow programs to identify where specific data can be found. For length, it lets programs to know how big or small data might be.*/
