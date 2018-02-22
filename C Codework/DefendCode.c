#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <regex.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <crypt.h>
#include <time.h>
#include <unistd.h>


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
int is_numbers_valid_range(long input);
void buffer_clear();
void prompt_user(char input[], char * prompt, char * regex, size_t max_size);
void add_values(int,int);
void multiply_values(int,int);
void clean_stdin();
void error_log(char*);
int is_file_valid(char*);
int output_to_file();
FILE * open_file(const char * fileName, const char * mode);
int close_File(FILE *);
char *readline (FILE *fp, char **buffer);
int write_to_file(const char * fileName, const char * mode, char * text);
void gather_password();
int validate_password();


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


int is_numbers_valid_range(long input)
{
    return(input <= INT_MAX && input >= INT_MIN);
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

	prompt_user(input, "Please input your first name. First name must be at most 50 characters long cannot contain numbers or special characters: ",
								"^[a-z|A-Z|\\-]{1,50}$", 50);
    size_t length = strnlen(input, 50);

    _firstName = (char*)calloc(length+1, sizeof(char));
    strncpy(_firstName, input, length);

    printf("%s\r\n", _firstName);

    prompt_user(input, "Please input your last name. Last name must be at most 50 characters long cannot contain numbers or special characters: ",
    								"^[a-z|A-Z|\\-]{1,50}$", 50);
    length = strnlen(input, sizeof input);

    _lastName = (char*)calloc(length + 1, sizeof(char)); 
    strncpy(_lastName, input, length);

    printf("%s\r\n", _lastName);

    regfree(&regex);
}


void gather_ints_from_user()
{
    int numOne = 0;
    int numTwo = 0;
    char input[20];
    int validInput;

    validInput = regcomp(&regex, "^[+\\-]?[0-9]{1,10}$", REG_EXTENDED);
    if(validInput ){printf("no work "); exit(1);}

    printf("Input an integer. Value MUST be less than %d or greater than %d: ", INT_MIN, INT_MAX);
    fgets(input, 20, stdin);
    
    remove_new_line(input);
        

    validInput = regexec(&regex,input,0,NULL,0);
    printf("\r\nInput: %s",input);

    while(validInput == REG_NOMATCH || is_numbers_valid_range(strtol(input, NULL, 10))  == 0)
    {
        buffer_clear();
        printf("Invalid input. Try again: ");
        fgets(input, 20, stdin);
        remove_new_line(input);

        validInput = regexec(&regex,input,0,NULL,0);
            
    }

    numOne = (int)strtol(input, NULL, 10);

    buffer_clear();

    printf("Input another Integer. Value MUST be less than %d or greater than %d: ", INT_MIN, INT_MAX);
    fgets(input, 20, stdin);
    remove_new_line(input);

    validInput = regexec(&regex,input,0,NULL,0);

    while(validInput == REG_NOMATCH || is_numbers_valid_range(strtol(input, NULL, 10))  == 0)
    {
        printf("Invalid input. Try again: ");
        fgets(input, 20, stdin);
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

    validInput = regcomp(&regex, "^[A-Z|a-z|0-9](\\-|\\_)*[A-Z|a-z|0-9]{0,260}\\.txt$", REG_EXTENDED); 
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
    char input[265];
    size_t maxFileName = 265; // As part of windows standards, files must be at minimum 260 length; plus the extension of .txt; plus the null terminator


    prompt_user(input, "Enter a valid output file. File must be a .txt extension and must be within the same folder as program. File must be readable: ",
                             "^[A-Z|a-z|0-9](\\-|\\_)*[A-Z|a-z|0-9]{0,260}\\.txt$", maxFileName + 5);
    
    size_t length = strnlen(input, sizeof(input));

    _outputFileName =(char*)calloc(length, sizeof(char));
    strncpy(_outputFileName, input, length);
    regfree(&regex);
    
}

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
  /*if (file == NULL) {
    perror("NULL FILE in open_file");
    return NULL;
  }*/
  
  return file;
}

int output_to_file() {
    _inputFile = open_file(_inputFileName, "r");

    char text[250];
    char * line;

    sprintf(text,"%s %s Added: %lld Multiplied: %lld", _firstName, _lastName, _valueAfterAdd, _valueAfterMultiply);


    if(write_to_file(_outputFileName, "w+", strncat(text,"\n", INT_MAX)) == 0)
        printf("\r\nerror in write_to_file from output_to_file for base info");


    if(write_to_file(_outputFileName, "a+", line = readline(_inputFile, &line)) == 0)
        printf("\r\nerror in write_to_file from output_to_file for base info");


    free(line);
    line = NULL;

    fclose(_inputFile);
    _inputFile = NULL;


    return 0;
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

int write_to_file(const char * fileName, const char * mode, char * text)
{

    FILE * file = open_file(fileName, mode);

    if(text == NULL) {
        return -1;
    }


    if(fwrite(text, sizeof(char), strnlen(text, INT_MAX), file) > 0) 
    {
        return (close_file(file) == 0);
    }

    if(close_file(file) == 0)
        return -2;


    return -1;
}


char *readline (FILE *fp, char **buffer) 
{
    int ch;
    size_t buflen = 0, nchar = 64;

    *buffer = calloc (nchar, sizeof(char));
    if (!*buffer) {
        fprintf (stderr, "readline() error: virtual memory exhausted.\n");
        return NULL;
    }

    while ((ch = fgetc(fp)) != EOF) 
    {
        (*buffer)[buflen++] = ch;

        if (buflen + 1 >= nchar) {  
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
    (*buffer)[buflen] = 0;           

    if (buflen == 0 && ch == EOF) {  
        free (*buffer);
        *buffer = NULL;
    }

    return *buffer;
}


char * encrypt_password(char * pass)
{
    return pass;
}


void gather_password()
{
    unsigned long seed[2];
    char salt[] = "$5$........";
    char pass[43];
    char * hash;
    const char *const seedchars = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    prompt_user(pass, "Please input a password: ", "", 24);//regex at ""

    printf("You Typed: %s\n", pass);


  
  
    //char *password;
    int i;

    /* Generate a (not very) random seed.
    You should do it better than this... */
    seed[0] = time(NULL);
    seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

    /* Turn it into printable characters from ‘seedchars’. */
    for (i = 0; i < 8; i++)
    salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];

    remove_new_line(pass);

    /* Read in the user’s password and encrypt it. */
    hash = crypt(pass, salt);

    /* Print the results. */
    printf("The password hash is: %s\n", hash);

    static char *save;
    printf("%s\n", strtok_r(hash, "$", &save));

    printf("%s\n", save);

    //printf("%s\n", strtok_r(save, "$", &save));
    //printf("%s\n", strtok_r(save, "$", &save));

    write_to_file(".password.ini", "w+", save);

    //puts(password);
    free(hash);
    hash = NULL;

    printf("%d\n", validate_password());

    return;
}


int validate_password()
{
    unsigned long seed[2];
    char pass[43];
    char * salthash = read_from_file(".password.ini");
    char * hash;
    char * loadedSalt;
    int i;

    seed[0] = time(NULL);
    seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

    prompt_user(pass, "Please repeat the password: ", "", 24);

    remove_new_line(pass);

    printf("You Typed: %s\n", pass);

    static char *save;
    printf("Salt: %s\n", loadedSalt = strtok_r(salthash, "$", &save));

    printf("Hash: %s\n", save);

    hash = crypt(pass, loadedSalt);

    free(hash);
    hash = NULL;

    return strcmp(hash, save) == 0;
}


void freeMemory()
{
    free(_firstName);
    free(_lastName);
    free(_inputFileName);
    free(_outputFileName);
    regfree(&regex);


    _inputFileName = NULL;
    _outputFile = NULL;
    _lastName = NULL;
    _firstName = NULL;

    _inputFile = NULL;
    _outputFile = NULL;
}


int main()
{

    readName();
    gather_ints_from_user();
    gather_user_input_file();
    gather_user_output_file();
    output_to_file();
    gather_password();
    freeMemory();
    return 0;
}
