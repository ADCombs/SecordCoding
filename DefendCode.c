#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <regex.h>

char * _firstName;
regex_t regex;
char * _lastName;

void removeNewLine(char * input)
{
    char *p;

    if((p = strchr(input, '\n')) != NULL)
        *p = '\0';
}

void readName()
{
    char input[50];
    int correctInputMatch;
    printf("Please input your first name. First name must be at most 50 characters long cannot contain numbers or special characters: ");
    fgets(input, 50, stdin);

    removeNewLine(input);

    correctInputMatch = regcomp(&regex, "^[a-z|A-Z|\\-]{1,50}$", REG_EXTENDED);
    if(correctInputMatch ){printf("no work "); exit(1);}
    correctInputMatch = regexec(&regex,input,0,NULL,0);


    while(correctInputMatch == REG_NOMATCH)
    {
        printf("Invalid input try again ");
        fgets(input,50,stdin);
        removeNewLine(input);
        correctInputMatch = regexec(&regex,input,0,NULL,0);    
    }

    size_t length = strnlen(input, sizeof input);

    _firstName = (char*)calloc(length, sizeof(char));

    printf("Please input your last name. Last name must be at most 50 characters long cannot contain numbers or special characters: ");
    fgets(input, 50, stdin);

    removeNewLine(input);
    correctInputMatch = regexec(&regex,input,0,NULL,0);


    while(correctInputMatch == REG_NOMATCH)
    {
        printf("Invalid input try again ");
        fgets(input,50,stdin);
        removeNewLine(input);
        correctInputMatch = regexec(&regex,input,0,NULL,0);    
    }

    length = strnlen(input, sizeof input);

    _lastName = (char*)calloc(length, sizeof(char)); 

    regfree(&regex);
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
    
    freeMemory();
    return 0;
}