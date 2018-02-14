using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace DefendYourCodeMidTerm
{
    partial class DefendCodeMain
    {
        private static string _name;
        private static Regex _regexName;

        // Pattern to match a name that is at least 1 character long to 50 characters max
        // and can contain as many hyphens as wanted. Will not accept numbers or 
        // special characters
        private static string NAMEPATTERN = @"^([a-z]|[A-Z]|-){1,50}$";

        /// <summary>
        /// Grabs user input for both first name and last name that is at least 50 characters long.
        /// Utilizes regeular expressions to validate user input.
        /// Combines first name and last name to be written out to output file.
        /// </summary>
        public static void UserInputName()
        {
            string inputFirstName;
            string inputLastName;
            _regexName = new Regex(NAMEPATTERN);

            Console.WriteLine("Please input a first name: " +
                              "first name can be no longer than 50 characters, " +
                              "name may contain hyphens, no other special characters, or numbers");

            inputFirstName = Console.ReadLine();

            while (inputFirstName != null && !_regexName.IsMatch(inputFirstName))
            {
                Console.WriteLine("Invalid name entry. Please input a valid entery");
                inputFirstName = Console.ReadLine();
            }

            Console.WriteLine("Please input a last name: " +
                              "last name can be no longer than 50 characters, " +
                              "name mat contain hyphens, no other special characters, or numbers");

            inputLastName = Console.ReadLine();

            while (inputLastName != null && !_regexName.IsMatch((inputLastName)))
            {
                Console.WriteLine("Invalid name entry. Please input a valid entery");
                inputLastName = Console.ReadLine();
            }

            _name = inputFirstName + " " + inputLastName;
        }


        static void Main(string[] args)
        {
            UserInputName();
        }
    }
}
