Team Members: Andrew Combs, Derek Sams, Tyelor Klein
Software used C#: Windows 7 – 10, Framework 4.6.1, environment: Visual Studios
Software used C: Ubuntu 16.04, Compiler: GCC 5.4, need: gcc -pedantic -Wall -Wextra -Werror DefendCode.c -lcrypt
Notes: C# input file and output file must exist in c# and live within the bin/debug folder while being a txt extension. 
	C Files input file must exist and live within the same folder as the program, must also have a .txt extension.
Issues relating to program: Currently 3 errors are being reported in Valgrind, related to filename pointer giving space than freeing it.
				C Sometimes user may need to hit enter when putting in input. Reason for this is buffer clearing sometimes requires for a \n
				C password hashing is not able to validate correctly