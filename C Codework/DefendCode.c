#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <regex.h>
#include <limits.h>
#include <errno.h>

char * _firstName;
regex_t regex;
char * _lastName;
long long _valueAfterAdd;
long long _valueAfterMultiply;
char * _inputFileName;
const int FILE_IS_READABLE = 1;
char * _outputFileName;

void freeMemory();
int is_numbers_valid_range(char *);
void buffer_clear();
void add_values(int,int);
void multiply_values(int,int);
void error_log(char*);
int is_file_valid(char*);

void error_log(char * error)
{
    FILE *errorLog;
    char *mode = "rw";

    errorLog = fopen("errorlog.txt", mode);

    if(errorLog == NULL)
    {
        printf("Can't open errorlog.txt program terminated ");
        freeMemory();
        exit(1);
    }
    else
        fprintf(errorLog, "%s", error);
    
    fclose(errorLog);
}


void remove_new_line(char * input)
{
    char *p;

    if((p = strchr(input, '\n')) != NULL)
        *p = '\0';
}


void add_values(int a, int b)
{
    long long aLong = (long long)a;
    long long bLong = (long long)b;
    _valueAfterAdd = aLong + bLong;
}


void multiply_values(int a, int b)
{
    long long aLong = (long long)a;
    long long bLong = (long long)b;
    _valueAfterMultiply = aLong * bLong;
}


int is_numbers_valid_range(char * input)
{
    char **end = NULL;
    int intBase = 10;

    strtol(input, end, intBase);

    if(errno == ERANGE)
        return 0;
    return 1; 
}


void buffer_clear()
{
    printf("\n");
    while((getchar()) != '\n');
    
}


void readName()
{
    char input[50];
    int correctInputMatch;
    printf("Please input your first name. First name must be at most 50 characters long cannot contain numbers or special characters: ");
    fgets(input, 50, stdin);

    remove_new_line(input);

    correctInputMatch = regcomp(&regex, "^[a-z|A-Z|\\-]{1,50}$", REG_EXTENDED);
    if(correctInputMatch ){printf("no work "); exit(1);}
    correctInputMatch = regexec(&regex,input,0,NULL,0);

    while(correctInputMatch == REG_NOMATCH || input[0]=='\0')
    {
        printf("Invalid input try again ");
        fgets(input,50,stdin);
        remove_new_line(input);
        correctInputMatch = regexec(&regex,input,0,NULL,0);    
    }

    buffer_clear();
    size_t length = strnlen(input, sizeof input);

    _firstName = (char*)calloc(length, sizeof(char));

    printf("Please input your last name. Last name must be at most 50 characters long cannot contain numbers or special characters: ");
    fgets(input, 50, stdin);

    remove_new_line(input);
    correctInputMatch = regexec(&regex,input,0,NULL,0);

    while(correctInputMatch == REG_NOMATCH || input[0]=='\0')
    {
        printf("Invalid input try again ");
        fgets(input,50,stdin);
        remove_new_line(input);
        correctInputMatch = regexec(&regex,input,0,NULL,0);    
    }

    buffer_clear();
    length = strnlen(input, sizeof input);

    _lastName = (char*)calloc(length, sizeof(char)); 

    regfree(&regex);
}


void gather_ints_from_user()
{
    int numOne = 0;
    int numTwo = 0;
    char input[10];
    int validInput;

    validInput = regcomp(&regex, "^[+\\-]?[0-9]{1,10}$", REG_EXTENDED);
    if(validInput ){printf("no work "); exit(1);}

    printf("Input an integer. Value MUST be less than %d or greater than %d: ", INT_MIN, INT_MAX);
    fgets(input, 11, stdin);
    remove_new_line(input);
        

    validInput = regexec(&regex,input,0,NULL,0);

    while(validInput == REG_NOMATCH || !is_numbers_valid_range(input))
    {
        buffer_clear();
        printf("Invalid input. Try again: ");
        fgets(input, 10, stdin);
        remove_new_line(input);

        validInput = regexec(&regex,input,0,NULL,0);
            
    }

    numOne = (int)strtol(input, NULL, 10);

    buffer_clear();

    printf("Input another Integer. Value MUST be less than %d or greater than %d: ", INT_MIN, INT_MAX);
    fgets(input, 11, stdin);
    remove_new_line(input);

    validInput = regexec(&regex,input,0,NULL,0);

    while(validInput == REG_NOMATCH || !is_numbers_valid_range(input))
    {
        printf("Invalid input. Try again: ");
        fgets(input, 11, stdin);
        remove_new_line(input);

        validInput = regexec(&regex,input,0,NULL,0);
    }


    numTwo = (int)strtol(input, NULL, 10);

    buffer_clear();
    multiply_values(numOne, numTwo);
    add_values(numOne, numTwo);
    regfree(&regex);
}


int is_file_valid(char* inputFileName)
{
    FILE * isFile;
    char * mode = "r";

    isFile = fopen(inputFileName, mode);

    if(isFile == NULL)
    {
        printf("Input File does not exist or is not readable\n");
        return 0;
    }

    fclose(isFile);
    return 1;
}

void gather_user_input_file()
{
    int validInput;
    char input[265];
    int maxFileName = 265; // As part of windows standards, files must be at minimum 260 length; plus the extension of .txt; plus the null terminator

    validInput = regcomp(&regex, "^[A-Z|a-z|0-9](\\-|\\_)*[A-Z|a-z|0-9]{1,260}\\.txt$", REG_EXTENDED); 
    if(validInput ){printf("no work "); exit(1);}

    do{

        printf("Enter a valid input file. File must be a .txt extension and must be within the same folder as program. File must be readable: ");
        fgets(input, maxFileName, stdin);
        remove_new_line(input);

        validInput = regexec(&regex,input,0,NULL,0);

        while(validInput == REG_NOMATCH || input[0]=='\0')
        {
            printf("Invalid input. Try again: ");
            fgets(input, maxFileName, stdin);
            remove_new_line(input);

            validInput = regexec(&regex,input,0,NULL,0);
        }

    }while(is_file_valid(input) != FILE_IS_READABLE);

    size_t length = strnlen(input, sizeof input);

    _inputFileName =(char*)calloc(length, sizeof(char));

    regfree(&regex);
}

void gather_user_output_file()
{
    int validInput;
    char input[265];
    int maxFileName = 265; // As part of windows standards, files must be at minimum 260 length; plus the extension of .txt; plus the null terminator

    validInput = regcomp(&regex, "^(\\w){1,264}$", REG_EXTENDED); 
    if(validInput ){printf("no work "); exit(1);}

    printf("Enter a valid input file. File must be a .txt extension and must be within the same folder as program. File must be readable: ");
    fgets(input, maxFileName, stdin);
    remove_new_line(input);

    while(validInput == REG_NOMATCH || input[0]=='\0')
    {
        printf("Invalid input. Try again: ");
        fgets(input, maxFileName, stdin);
        remove_new_line(input);
    }
}


void freeMemory()
{
    free(_firstName);
    free(_lastName);
    free(_inputFileName);
    regfree(&regex);

    _inputFileName = NULL;
    _lastName = NULL;
    _firstName = NULL;
}


int main()
{
    readName();
    gather_ints_from_user();
    gather_user_input_file();
    freeMemory();
    return 0;
}