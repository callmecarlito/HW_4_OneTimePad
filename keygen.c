#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHAR_SET_SZ 27

char* RandomKey(size_t key_length);
void WriteKey(char* otp_key);

int main(int argc, char *argv[]){
    srand(time(NULL));
    char* otp_key;
    size_t key_length = atoi(argv[1]); //convert string value to int

    otp_key = RandomKey(key_length);
    //printf("otp_key: %s[%d]\n", otp_key, strlen(otp_key));
    WriteKey(otp_key);    
    
    free(otp_key); //free memory allocated by RandomKey()
    return 0;
}
/**********************************************************************
 * 
 **********************************************************************/
char* RandomKey(size_t key_length){
    //key generated must contain only the 26 upper-case letters of the alphabet and space char
    static char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    int i, index;
    char* rand_key;
    //allocate memory for key; key_length + 1 to account for newline char
    rand_key = malloc(sizeof(char) * key_length + 1); 
    
    //randomly fill rand_key with chars from char_set
    for(i = 0; i < key_length; i++){
        index = rand() % CHAR_SET_SZ; //randomly select an index from 0 to 26
        rand_key[i] = char_set[index]; //add char to key
    }
    rand_key[key_length] = '\0';//add null terminator to end of string
    return rand_key;
}
/**********************************************************************
 * 
 **********************************************************************/
void WriteKey(char* otp_key){
    FILE* output_file = fopen("key_file", "w+");
    if(output_file == NULL){
        perror("Unable to create output file: ");
        exit(1);
    }
    else{
        fprintf(output_file, "%s\n", otp_key);
        fclose(output_file);
    }

}