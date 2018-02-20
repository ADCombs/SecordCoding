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
int _numOne;
int _numTwo;
const int INT_OVERFLOW = 1;

void freeMemory();
int is_numbers_valid_range(char *);
void buffer_clear();
int is_safe_add(int,int);
int is_safe_multiply(int,int);
void error_log(char*);


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


int is_safe_add(int a, int b)
{
    if(a > 0 && b > INT_MAX - a)
        return 1;
    else if(a < 0 && b < INT_MIN - a)
        return 1;
    else 
        return 0;
}


int is_safe_multiply(int a, int b)
{
    if(b != 0 && a > INT_MAX/b)
        return 1;
    else
        return 0;
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

    while(correctInputMatch == REG_NOMATCH && !is_numbers_valid_range(input))
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

    while(correctInputMatch == REG_NOMATCH && !is_numbers_valid_range(input))
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

    validInput = regcomp(&regex, "^[0-9]+$", REG_EXTENDED);
    if(validInput ){printf("no work "); exit(1);}

    do
    {
        printf("Input an integer. Value MUST be less than -2,147,483,647 or greater than 2,147,483,647: ");
        fgets(input, 11, stdin);
        remove_new_line(input);
        

        validInput = regexec(&regex,input,0,NULL,0);
        
        while(validInput == REG_NOMATCH && !is_numbers_valid_range(input))
        {
            printf("Invalid input. Try again: ");
            fgets(input, 10, stdin);
            remove_new_line(input);

            validInput = regexec(&regex,input,0,NULL,0);
            
        }

         numOne = (int)strtol(input, NULL, 10);
        
        buffer_clear();

        printf("Input another Integer. Value MUST be less than -2,147,483,647 or greater than 2,147,483,647: ");
        fgets(input, 10, stdin);
        remove_new_line(input);

        validInput = regexec(&regex,input,0,NULL,0);

        while(validInput == REG_NOMATCH)
        {
            printf("Invalid input. Try again: ");
            fgets(input, 10, stdin);
            remove_new_line(input);

            validInput = regexec(&regex,input,0,NULL,0);
        }


        numTwo = (int)strtol(input, NULL, 32);

        buffer_clear();

    }while(is_safe_add(numOne, numTwo) == INT_OVERFLOW || is_safe_multiply(numOne, numTwo) == INT_OVERFLOW);

    _numOne = numOne;
    _numTwo = numTwo;
}


void freeMemory()
{
    free(_firstName);
    free(_lastName);
    regfree(&regex);

    _lastName = NULL;
    _firstName = NULL;
}


int main()
{
    readName();
    gather_ints_from_user();
    freeMemory();
    return 0;
}