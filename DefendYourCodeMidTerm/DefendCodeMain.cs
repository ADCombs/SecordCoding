using System;
using System.IO;
using System.Security;
using System.Text.RegularExpressions;
using System.Security.Permissions;

namespace DefendYourCodeMidTerm
{
    partial class DefendCodeMain
    {
        private static string _name;
        private static Regex _regexNameMatcher;
        private static int _numOne;
        private static int _numTwo;
        private static string _fileInputName;
        private static string _fileOutputName;

        // Pattern for matching the file input stream
        // Makes sure that the file that is entered only lives within the debug/bin folder of program location
        // Example of accepted patterns: test.txt, te-st.txt, -test .txt, te st.txt, te- st.txt
        // Example of not acceptable input: /test.txt, c\\:test.txt, t3st.txt
        private static string _fileNamePattern = @"^([A-Z]|[a-z]|\.| |\-)*\.txt$"; // A better regex (imo) would be: ^[\w\. \-]+\.txt$


        /// <summary>
        /// Logs all errors that happen in system to a log file
        /// </summary>
        /// <param name="errorInfo">String that tells what kind of error occured and where it occured at</param>
        private static void ErrorLogFile(string errorInfo)
        {
            using (StreamWriter sw = File.AppendText("../ErrorLog.txt"))
            {
                sw.WriteLine(errorInfo);
            }
        }

        /// <summary>
        /// Detects whether user has attempted to overflow integer for addition and multiplication (As specified by requirements)
        /// </summary>
        /// <param name="numOne"></param>
        /// <param name="numTwo"></param>
        /// <returns></returns>
        private static bool IsIntegerOverflow(int numOne, int numTwo)
        {
            bool overflow = false;

            try
            {
                int sumAdd = checked(numOne + numTwo);
                int sumTimes = checked(numOne * numTwo);
            }
            catch (System.OverflowException e)
            {
                ErrorLogFile($"Error: {e.ToString()} {DateTime.Now}");
                Console.WriteLine("Overflow detected");
                overflow = true;
            }

            return overflow;
        }

        /// <summary>
        /// Allows access to current file to read and write to it
        /// </summary>
        /// <param name="absoluteFilePath"></param>
        /// <returns> returns true if permissions were set to allow access else false</returns>
        private static bool GiveFilePermissions(string absoluteFilePath)
        {
            var filePermission = new FileIOPermission(FileIOPermissionAccess.AllAccess, absoluteFilePath);
            var isFilePermissionsSet = true;
            

            try
            {
                filePermission.Demand();
            }
            catch (SecurityException e)
            {
                ErrorLogFile($"Error: {e.ToString()} {DateTime.Now}");
                Console.WriteLine("Error in giving permissions");
                isFilePermissionsSet = false;
            }

            return isFilePermissionsSet;
        }

        /// <summary>
        /// Grabs user input for both first name and last name that is at least 50 characters long.
        /// Gathers information for first name then last name.
        /// Utilizes regeular expressions to validate user input.
        /// Combines first name and last name to be written out to output file.
        /// </summary>
        public static void UserInputName()
        {
            // Pattern to match a name that is at least 1 character long to 50 characters max
            // and can contain as many hyphens as wanted. Will not accept numbers or 
            // special characters
            string namePattern = @"^([a-z]|[A-Z]|\-){1,50}$";
            string inputFirstName;
            string inputLastName;
            _regexNameMatcher = new Regex(namePattern);

            Console.WriteLine("Please input a first name: " +
                              "first name can be no longer than 50 characters, " +
                              "name may contain hyphens, no other special characters, or numbers");

            inputFirstName = Console.ReadLine();

            while (inputFirstName != null && !_regexNameMatcher.IsMatch(inputFirstName))
            {
                Console.WriteLine("Invalid name entry. Please input a valid entry");
                ErrorLogFile($"Warning: Bad input data during name collection {DateTime.Now}");
            }

            Console.WriteLine("Please input a last name: " +
                              "last name can be no longer than 50 characters, " +
                              "name may contain hyphens, no other special characters, or numbers");

            inputLastName = Console.ReadLine();

            while (inputLastName != null && !_regexNameMatcher.IsMatch((inputLastName)))
            {
                Console.WriteLine("Invalid name entry. Please input a valid entry");
                ErrorLogFile($"Warning: Bad input data during name collection {DateTime.Now}");
                inputLastName = Console.ReadLine();
            }

            _name = inputFirstName + " " + inputLastName;
        }

        /// <summary>
        /// Prompts for and reads in two integer values from the user
        /// Reads integers one at a time to ensure that each integer that is entered is within specifications
        /// </summary>
        public static void UserInputIntegers()
        {
            int numOne;
            int numTwo;
            string input = "";

            do
            {
                Console.WriteLine(
                    "Input integer values that are less than 2,147,483,647. Integers must be input one at a time");
                input = Console.ReadLine();

                while (!int.TryParse(input, out numOne) || input == null)
                {
                    Console.WriteLine("Invalid input");
                    ErrorLogFile($"Warning: Invalid input detected {DateTime.Now}");
                    input = Console.ReadLine();
                }

                numOne = int.Parse(input);

                input = Console.ReadLine();

                while (!int.TryParse(input, out numTwo) || input == null)
                {
                    Console.WriteLine("Invalid input");
                    ErrorLogFile($"Warning: Invalid input detected {DateTime.Now}");
                    input = Console.ReadLine();
                }

                numTwo = int.Parse(input);
            } while (IsIntegerOverflow(numOne, numTwo));

            _numOne = numOne;
            _numTwo = numTwo;
        }

        /// <summary>
        /// Prompts for and reads the name of an input file from user. 
        /// File must be a .txt extension and must live within the directory of file
        /// </summary>
        public static void UserInputFile()
        {
            string inputFileName = "";

            Console.WriteLine("Enter a valid input file. File must be a .txt extension and must be within debug/bin" +
                              "File must also be readable.");

            do
            {
                inputFileName = Console.ReadLine();
                _regexNameMatcher = new Regex(_fileNamePattern);

                while ((inputFileName != null && !File.Exists(inputFileName)) || !_regexNameMatcher.IsMatch(inputFileName))
                {
                    Console.WriteLine("File does not exist or does not match valid input");
                    inputFileName = Console.ReadLine();
                    ErrorLogFile($"Warning: Invalid name type detected {DateTime.Now}");
                }

            } while (!GiveFilePermissions(Path.GetFullPath(inputFileName)));

            _fileInputName = inputFileName;
        }

        public static void UserOutputFile()
        {
            string outputFileName = "";

            Console.WriteLine("Enter a valid existing output file. File Must exist within debug/bin and must be a .txt extension." +
                              "File must be able to be readable and writable.");

            do
            {
                outputFileName = Console.ReadLine();
                _regexNameMatcher = new Regex(_fileNamePattern);

                while ((outputFileName != null && !File.Exists(outputFileName)) || !_regexNameMatcher.IsMatch(outputFileName))
                {
                    Console.WriteLine("File does not exist or does not match valid input");
                    outputFileName = Console.ReadLine();
                    ErrorLogFile($"Warning: Invalid name type detected {DateTime.Now}");
                }

            } while (!GiveFilePermissions(Path.GetFullPath(outputFileName)));

            _fileOutputName = outputFileName;
        }

        static void Main(string[] args)
        {
            UserInputName();
            //UserInputIntegers();
            //UserInputFile();
            UserOutputFile();
        }
    }
}
