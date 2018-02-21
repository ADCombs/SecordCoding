using System;
using System.IO;
using System.Security;
using System.Text.RegularExpressions;
using System.Security.Permissions;
using System.Security.Cryptography;
using System.Text;
using System.Collections.Generic;
using System.Numerics;
using System.Security.AccessControl;
using System.Security.Principal;

namespace DefendYourCodeMidTerm
{
    partial class DefendCodeMain
    {
        private static string _name;
        private static BigInteger _numOne;
        private static BigInteger _numTwo;
        private static string _fileInputName;
        private static string _fileOutputName;

        // Pattern for matching the file input stream
        // Makes sure that the file that is entered only lives within the debug/bin folder of program location
        // Example of accepted patterns: test.txt, te-st.txt, -test .txt, te st.txt, te- st.txt
        // Example of not acceptable input: /test.txt, c\\:test.txt, t3st.txt
        private static string _fileNamePattern = @"^\w+(?:[\. -]\w+)*\.txt$"; // This new regex allows for alphanumeric, spaces, hypens, and periods. However spaces, hypens, and periods must come between at least two alphanumberics, for example a.a.txt is valid, but a..txt is not and ..txt is not either.
        private static string _passwordPatternRegex = @"(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[!@%\^&\*])(?=^[\w!@%\^&\*]{8,24}$)";


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
        private static void CalculateIntegers(int numOne, int numTwo)
        {
            BigInteger intone = numOne;
            BigInteger inttwo = numTwo;
            _numOne = intone + inttwo;
            _numTwo = intone * inttwo;
        }

        /// <summary>
        /// Allows access to current file to read and write to it
        /// </summary>
        /// <param name="absoluteFilePath"></param>
        /// <returns> returns true if permissions were set to allow access else false</returns>
        private static bool CheckFilePermissions(string absoluteFilePath)
        {
            var writeAllowAndReadAllow = false;
            var writeDenyAndReadDeny = false;
            var accessControlList = Directory.GetAccessControl(absoluteFilePath);

            if (accessControlList == null)
                return false;
            var accessRules = accessControlList.GetAccessRules(true, true,
                typeof(System.Security.Principal.SecurityIdentifier));
            if (accessRules == null)
                return false;

            foreach (FileSystemAccessRule rule in accessRules)
            {
                if ((FileSystemRights.Write & rule.FileSystemRights) != FileSystemRights.Write || (FileSystemRights.Read & rule.FileSystemRights) != FileSystemRights.Read)
                    continue;

                if (rule.AccessControlType == AccessControlType.Allow)
                    writeAllowAndReadAllow = true;
                else if (rule.AccessControlType == AccessControlType.Deny)
                    writeDenyAndReadDeny = true;
            }

            return writeAllowAndReadAllow && !writeDenyAndReadDeny;

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
            string namePattern = @"(?=^.{1,50}$)^[A-Za-z]+(?:[\- '][A-Za-z]+)*$";
            string inputFirstName = GetUserInput("Please input a first name no longer than 50 characters, name may contain letters, and a space, hyphen, or apostraphe between letters. No other characters, or numbers.", namePattern);
            string inputLastName = GetUserInput("Please input a last name no longer than 50 characters, name may contain letters, and a space, hyphen, or apostraphe between letters. No other characters, or numbers.", namePattern);

            _name = inputFirstName + " " + inputLastName;
        }

        /// <summary>
        /// Prompts for and reads in two integer values from the user
        /// Reads integers one at a time to ensure that each integer that is entered is within specifications
        /// </summary>
        public static void UserInputIntegers()
        {
            int numOne = 0;
            int numTwo = 0;
            int results = 0;
            string input = string.Empty;

                input = GetUserInput("Input an integer that is between " + int.MinValue + " and " + int.MaxValue, @"^[+\-]?\d{1,10}$");

                while (!int.TryParse(input, out results))
                {
                    Console.WriteLine("Input was not between " + int.MinValue + " and " + int.MaxValue);
                    input = GetUserInput("Input an integer that is between " + int.MinValue + " and " + int.MaxValue, @"^[+\-]?\d{1,10}$");
                }

                numOne = int.Parse(input);

                input = GetUserInput("Input an integer that is between " + int.MinValue + " and " + int.MaxValue, @"^[+\-]?\d{1,10}$");

                while (!int.TryParse(input, out results))
                {
                    Console.WriteLine("Input was not between " + int.MinValue + " and " + int.MaxValue);
                    input = GetUserInput("Input an integer that is between " + int.MinValue + " and " + int.MaxValue, @"^[+\-]?\d{1,10}$");
                }

                numTwo = int.Parse(input);

            CalculateIntegers(numOne, numTwo);
        }

        /// <summary>
        /// Prompts for and reads the name of an input file from user. 
        /// File must be a .txt extension and must live within the directory of file
        /// </summary>
        public static void UserInputFile()
        {
            string fileInputName = string.Empty;

            do
            {
                fileInputName = GetUserInput("Enter a valid input file. File must be a .txt extension and must be within debug/bin. File must also be readable.", _fileNamePattern);

                while(!FileController.FileExists(fileInputName))
                {
                    Console.WriteLine("Input file does not exist!");
                    ErrorLogFile($"Warning: User attempted to input an input file that did not exist! {DateTime.Now}");
                    fileInputName = GetUserInput("Enter a valid input file. File must be a .txt extension and must be within debug/bin. File must also be readable.", _fileNamePattern);
                }

            } while (!CheckFilePermissions(Path.GetFullPath(fileInputName)));

            _fileInputName = fileInputName;
        }

        public static void UserOutputFile()
        {
            string fileOutputName = string.Empty;

            do
            {
                fileOutputName = GetUserInput("Enter a valid output file. File must be a .txt extension and must be within debug/bin. File must also be readable.", _fileNamePattern);

            } while (!CheckFilePermissions(Path.GetFullPath(fileOutputName)) || fileOutputName == _fileInputName);

            _fileOutputName = fileOutputName;
        }

        private static void UserInputPassword()
        {
            string prompt = "Enter a password between 8 and 24 characters, with no spaces, only alphanumeric characters, and the following special characters: ! @ % ^ & *" +
                              "\r\nThe password must contain at least one uppercase letter, one lowercase letter, one number, and one special character.";


            //SHA256 sha256 = SHA256.Create();
            RNGCryptoServiceProvider rngProvider = new RNGCryptoServiceProvider();
            byte[] salt = new byte[32];
            rngProvider.GetNonZeroBytes(salt);

            FileController.WriteToFile(".password.ini", GetHashSHA256(GetUserInput(prompt, _passwordPatternRegex), salt) + "\n" + Convert.ToBase64String(salt), append: false, hidden: true, noWrite: true, encrypted: true);

            bool valid = ValidatePassword(GetUserInput("Repeat Password: ", _passwordPatternRegex));

            while (!valid)
            {
                Console.WriteLine("Password was not correct.");

                valid = ValidatePassword(GetUserInput("Repeat Password: ", _passwordPatternRegex));
            }

            Console.WriteLine("Password was correct.");
        }

        /// <summary>
        /// Per assignment requirements:
        /// Opens the output file and writes the user's name along with the result of 
        /// adding the two integer values and the result of multiplying the two integer values, 
        /// followed by the contents of the input file
        /// </summary>
        private static void OutputToFile()
        {
            try
            {
                FileController.WriteToFile(
                    _fileOutputName,
                    _name + " " + "Added: " + _numOne + " Muliplied: " + _numTwo + "\n",
                    string.Empty,
                    true,
                    false);

                StreamReader reader = new StreamReader(Path.GetFullPath(_fileInputName), System.Text.Encoding.UTF8, true, 512);
                StreamWriter writer = new StreamWriter(Path.GetFullPath(_fileOutputName), append: true);

                string line;
                
                while ((line = reader.ReadLine()) != null)

                {
                    writer.WriteLine(line);
                }

                reader.Close();
                writer.Close();
            }
            catch (Exception e)
            {
                ErrorLogFile(
                    $"Warning: An Error occured during the FileInAndOut method at {DateTime.Now}\n" + e.Message);
            }
        }

        private static string GetUserInput(string prompt, string regex)
        {
            Regex regexMatcher = new Regex(regex);
            string ret = string.Empty;

            Console.WriteLine(prompt);
            ret = Console.ReadLine();

            while (ret == null || ret == string.Empty || !regexMatcher.IsMatch(ret))
            {
                Console.WriteLine("Invalid entry. Please input a valid entry");
                ErrorLogFile($"Warning: Bad input data during name collection {DateTime.Now}");

                Console.WriteLine(prompt);
                ret = Console.ReadLine();
            }

            return ret;
        }

        private static string GetHashSHA256(string input, byte[] salt)
        {
            if (input == null || input == "")
                return null;

            byte[] hash;
            HashAlgorithm sha = SHA256.Create();

            List<byte> pass = new List<byte>(Encoding.Unicode.GetBytes(input));

            pass.AddRange(salt);

            hash = sha.ComputeHash(pass.ToArray());

            return Convert.ToBase64String(hash);
        }

        private static bool ValidatePassword(string input)
        {
            if (input == null || input == "" || !FileController.FileExists(".password.ini"))
                return false;

            string[] fin = FileController.ReadAsString(".password.ini").Split('\n');

            if (fin.Length != 2)
                return false;

            byte[] s = Convert.FromBase64String(fin[1].Trim());

            return fin[0].Trim() == GetHashSHA256(input, s);
        }

        static void Main(string[] args)
        {
            UserInputName();
            UserInputIntegers();
            UserInputFile();
            UserOutputFile();
            UserInputPassword();
            OutputToFile();
            Console.ReadLine();
        }
    }
}
