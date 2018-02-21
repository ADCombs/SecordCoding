#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <regex.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>

//static const int CHUNK = 1024;

char * _firstName;
regex_t regex;
char * _lastName;
long long _valueAfterAdd;
long long _valueAfterMultiply;
char * _inputFileName;
const int FILE_IS_READABLE = 1;
char * _outputFileName;
FILE * _inputFile;
FILE * _outputFile;

const char * _errorlog = "error.log";

void freeMemory();
int is_numbers_valid_range(char *);
void buffer_clear();
void prompt_user(char input[], char * prompt, char * regex, size_t max_size);
void add_values(int,int);
void multiply_values(int,int);
void error_log(char*);
int is_file_valid(char*);
int output_to_file();
FILE * open_file();
int close_File(FILE *);
char *readline (FILE *fp, char **buffer);


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


void clean_stdin()
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


void prompt_user(char input[], char * prompt, char * regex, size_t max_size)
{
	if(input == NULL || prompt == NULL || regex == NULL)
		return;

	char userInput[max_size*2];
	regex_t regext;
	int correctInputMatch;
	printf("%s", prompt);
	fgets(userInput, max_size*2, stdin);
	remove_new_line(userInput);

	correctInputMatch = regcomp(&regext, regex, REG_EXTENDED);
	if(correctInputMatch ){printf("no work"); exit(1);}
	correctInputMatch = regexec(&regext,userInput,0,NULL,0);

	if(correctInputMatch == REG_NOMATCH || userInput[0]=='\0')
	{
		clean_stdin();
		printf("%s\r\n", "Invalid input...");
		prompt_user(input, prompt, regex, max_size);
	}
	else if(correctInputMatch == REG_NOERROR)
	{
		strncpy(input, userInput, max_size);
	}

	regfree(&regext);
}


void readName()
{
    char input[50] = "\0";
    //int correctInputMatch;
    /*printf("Please input your first name. First name must be at most 50 characters long cannot contain numbers or special characters: ");
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

    buffer_clear();*/
	prompt_user(input, "Please input your first name. First name must be at most 50 characters long cannot contain numbers or special characters: ",
								"^[a-z|A-Z|\\-]{1,50}$", 50);
    size_t length = strnlen(input, 50);

    _firstName = (char*)calloc(length, sizeof(char));
    strncpy(_firstName, input, length);

    printf("%s\r\n", _firstName);

    /*printf("Please input your last name. Last name must be at most 50 characters long cannot contain numbers or special characters: ");
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

    buffer_clear();*/
    prompt_user(input, "Please input your last name. Last name must be at most 50 characters long cannot contain numbers or special characters: ",
    								"^[a-z|A-Z|\\-]{1,50}$", 50);
    length = strnlen(input, sizeof input);

    _lastName = (char*)calloc(length, sizeof(char)); 
    strncpy(_lastName, input, length);

    printf("%s\r\n", _lastName);

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

    size_t length = strnlen(input, sizeof(input));

    _inputFileName =(char*)calloc(length, sizeof(char));
    strncpy(_inputFileName, input, length);

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
    
    size_t length = strnlen(input, sizeof(input));

    _outputFileName =(char*)calloc(length, sizeof(char));
    strncpy(_outputFileName, input, length);
    
    regfree(&regex);
    
}

int output_to_file() {
    printf("Check 0 in output to file\n");

    //_inputFile = open_file(_inputFileName, "r")

    printf("Check 1 in output to file\n");

    //_outputFile = open_file(_outputFileName, "w");


    //write_to_file(_inputFileName, "w", "")






    return 0;
}

/*
off_t fileSize(const char * filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}
*/

int close_file(FILE * file)
{

   if (NULL == file) {
       return -1;
     }
     /* ... */
     if (fclose(file) == EOF) {
       return -1;
     }
     return 0;
}

FILE * open_file(const char * fileName, const char * mode)
{
  FILE * file = fopen(fileName, mode);
  if (file == NULL) {
    /* Handle error */
    perror("NULL FILE in open_file");
    return NULL;
  }
  
  return file;
}

char * read_from_file(const char * fileName) 
{
    FILE * file = open_file(fileName, "r");

    char * buf = readline(file, &buf);
    
    if(close_file(file) != 0) {
        perror("error while trying to close file in read_from_file\n");
        return "default";
    }

    return buf;
}

int write_to_file(const char * fileName, const char * mode, char * text, size_t maxSize)
{
    FILE * file = open_file(fileName, mode);

    if(text == NULL) {
        return -1;
    }

    if(fwrite(text, sizeof(char), maxSize, file) > 0)
        return (close_file(file) == 0);

    if(close_file(file) != 0)
        return -2;

    return -1;
}

/* read line from 'fp' allocate *buffer nchar in size
 * realloc as necessary. Returns a pointer to *buffer
 * on success, NULL otherwise.
 */
char *readline (FILE *fp, char **buffer) 
{
    int ch;
    size_t buflen = 0, nchar = 64;

    *buffer = calloc (nchar, sizeof(char));    /* allocate buffer nchar in length */
    if (!*buffer) {
        fprintf (stderr, "readline() error: virtual memory exhausted.\n");
        return NULL;
    }

    while ((ch = fgetc(fp)) != '\n' && ch != EOF) 
    {
        (*buffer)[buflen++] = ch;

        if (buflen + 1 >= nchar) {  /* realloc */
            char *tmp = realloc (*buffer, nchar * 2);
            if (!tmp) {
                fprintf (stderr, "error: realloc failed, "
                                "returning partial buffer.\n");
                (*buffer)[buflen] = 0;
                return *buffer;
            }
            *buffer = tmp;
            nchar *= 2;
        }
    }
    (*buffer)[buflen] = 0;           /* nul-terminate */

    if (buflen == 0 && ch == EOF) {  /* return NULL if nothing read */
        free (*buffer);
        *buffer = NULL;
    }

    return *buffer;
}

void freeMemory()
{
    free(_firstName);
    free(_lastName);
    free(_inputFileName);
    regfree(&regex);

    fclose(_inputFile);
    fclose(_outputFile);

    _inputFileName = NULL;
    _lastName = NULL;
    _firstName = NULL;

    _inputFile = NULL;
    _outputFile = NULL;
}


int main()
{
    /*
    int fd;
    if((fd=open(_errorlog,O_CREAT|O_APPEND|O_WRONLY,0644))!=-1)
    {
        dup2(fd,2);
    }
    else
    {
        perror(_errorlog);
    }    
    close_file(fd);
    */

    readName();
    //gather_ints_from_user();
    //gather_user_input_file();
    //gather_user_output_file();
    //output_to_file();
    freeMemory();
    return 0;
}
