// Author: Tyelor Klein
// Written for my game, and reused here in our project. Just a simple controller for handling file i/o.
// Probably not the best way to go about it, but likely not a horrible way either. Can handle outputting to directories.
// Does not do any error checking or catching. Assumes valid inputs when being used, so make sure to check your input before
// calling any FileController methods!!

/*  
 * ============================================================================================================================  
 *                                                       !!WARNING!!
 *                                  THIS CODE DOES NOT ERROR CHECK AND ASSUMES VALID INPUT
 *                         MAKE SURE TO CHECK FOR VALID INPUT BEFORE CALLING ANY OF THE METHODS BELOW
 * ============================================================================================================================
*/

using System.IO;

namespace DefendYourCodeMidTerm
{
    /// <summary>
    /// This class is used to encapsulate all the file I/O into a single reusuable class.
    /// <para/>Note that this is likely not the best way to handle file I/O, however it works fairly well with minimal code.
    /// <para/>Assumes valid inputs when being used, so make sure to check your input before calling any FileController methods!!
    /// <para/>DOES NOT DO ANY ERROR CHECKING OR CATCHING!
    /// </summary>
    public static class FileController
    {
        
        /// ==========================================================================================================================
        /// <summary>
        /// This method takes in several parameters, the first two being required and the last three being optional.
        /// <para/>Does no error checking to see if any of the strings are null, and may throw errors if parameters are invalid.
        /// <para>This method will write the text parameter to the given filename parm. If a path is specified, this method will
        /// create said directory if it doesn't already exist and then output the file to that directory.</para>
        /// <para/>If create is false and the file doesn't already exist than no file will be created and no output will be done.
        /// <para/>If append is false the current file will be overwritten with the new text, otherwise it will be appended to it.
        /// <para/>
        /// </summary>
        /// <param name="filename">The file to be written to.</param>
        /// <param name="text">The text to be output to the given file.</param>
        /// <param name="path">[Optional Param]The path that file is located at. Set to "" as default.</param>
        /// <param name="create">[Optional Param]Determines if the output file should be created if it doesn't exist. Set to true as default.</param>
        /// <param name="append">[Optional Param]Determines if the text should be appended to the output or overwritten. Set to true as default.</param>
        /// ==========================================================================================================================
        public static void WriteToFile(string filename, string text, string path = "", bool create = true, bool append = true)
        {
            if (path != null && path != "" && !Directory.Exists(path) && create)
                Directory.CreateDirectory(path);

            string fullPath = path + filename;

            if (File.Exists(fullPath) || create)
            {
                StreamWriter writer = new StreamWriter(fullPath, append);
                writer.Write(text);
                writer.Close();
            }
        }


        /// ==========================================================================================================================
        /// <summary>
        /// This method takes in two parameters, the first is required, the last being optional.
        /// <para/>Does no error checking to see if any of the strings are null, and may throw errors if parameters are invalid.
        /// <para/>This method will check if a given input filename at an optional path exists, and if it does it will return
        /// the contents of that file as a string. Otherwise it will return null.
        /// </summary>
        /// <param name="filename">The file to be read.</param>
        /// <param name="path">[Optional Param]The path that file is located at. Set to "" as default.</param>
        /// <returns>Returns contents of given file if it exists, otherwise returns null.</returns>
        /// ==========================================================================================================================
        public static string ReadAsString(string filename, string path = "")
        {
            string fullPath = path + filename;

            if (File.Exists(fullPath))
            {
                StreamReader reader = new StreamReader(fullPath);
                string ret = reader.ReadToEnd();
                //Debug.Log(ret);
                reader.Close();

                return ret;
            }

            return null;
        }

        /// ==========================================================================================================================
        /// <summary>
        /// This method takes in two parameters, the first is required, the last being optional.
        /// <para>This method will check to see if a given filename exists at a given path. Returns true if it does, 
        /// otherwise false.</para>
        /// <para/>Note: this method is mostly equivalent to calling File.Exists(path + filename);
        /// </summary>
        /// <param name="filename">The file to be checked.</param>
        /// <param name="path">[Optional Param]The path that file is located at. Set to "" as default.</param>
        /// <returns>Returns whether or not a given file exists.</returns>
        /// ==========================================================================================================================
        public static bool FileExists(string filename, string path = "")
        {
            if (path == null || filename == null || filename == "")
                return false;

            return File.Exists(path + filename);
        }
    }
}