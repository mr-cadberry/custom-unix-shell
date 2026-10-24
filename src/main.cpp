#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>




std::string getPath(const std::string& commandName);
std::string runExternalCommand(const std::string& input);


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // get input from user
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);


    std::stringstream ss(command);

    std::string programName;
    ss >> programName; //takes the first word of the input

    std::vector<std::string> parsedArgs;
    std::string arg;
    while (ss >> arg) {
      parsedArgs.push_back(arg); // grabs the rest of the inputs in this vector
    }

    std::string output; // temporary string to hold output for echo command
    if (programName == "exit") { // check if the input is exit
      break;
    } else if (programName == "echo"){ // is first word is echo
        for (const auto& target : parsedArgs) { 
          output.append(target);
          output.append(" ");
        } 
      std::cout << output << "\n"; 

    } else if (programName == "type"){ // if the first word is type
      std::unordered_set<std::string> builtins = {"echo", "exit", "type"}; // create a set, put the builtin command names in the set     
      for (const auto& target : parsedArgs) { // loop through the parsedArgs vector, and check if each argument is a builtin command
        if (builtins.count(target)) {
          std::cout << target << " is a shell builtin\n";
        }  else if (std::string result = getPath(target); !result.empty()){ //returns the full path (e.g. "/usr/bin/ls")  
          std::cout << target << " is " << result << "\n";    // if found and executable, or an empty string "" if not found.
        } else {
          std::cout << target << ": not found" << "\n";
        }
      }
    } 
    else if (std::string result = getPath(programName); !result.empty()){ 
      
      std::vector<char*>args; // create a vector of named args, which holds the pointer
      
      //Add the command name in the pointer vector
      args.push_back(const_cast<char*>(programName.c_str())); // c_str() return const char* pointer to the string,
                                                              // const_cast is used to remove the constness of the pointer,
                                                              // so that it can be passed to execvp function, 
                                                              //which expects a non-const char* pointer.

      // add other arguments to the vector, if any
      for (const auto& a : parsedArgs) { // loop through the parsedArgs vector, and add each argument to the args vector
        args.push_back(const_cast<char*>(a.c_str()));
      }
      args.push_back(nullptr); // add a null pointer to the end of the vector, to indicate the end of the arguments


      pid_t  pid = fork(); // create a new child process
      if (pid == 0) {
        // this is a child process.
        // transform this clone into the new program
        execv(result.c_str(), args.data()); 

        // if execv returns, it means there was an error
        perror("execv failed");
        exit(1);

      } else if (pid > 0) {
        // this is the parent process
        // do not exit untill the child is finished
        int status;
        waitpid(pid, &status, 0);
      } else {
        // fork faild, something is wrong with the creating process
        std::cerr << "fork failed" << "\n";
      }
    }
    
    else {
      //output error message if command is not found
      std::cout << programName << ": command not found" << std::endl; // command is invalid
    }

  }
return 0;

}





//array that uses null pointer to indicate the end of the array. The array is used to store the command and its arguments, which are passed to the execvp function to execute the command. The execvp function replaces the current process with a new process that runs the specified command. If execvp fails, it returns -1 and sets errno to indicate the error. In this case, we print an error message and return an empty string.


// Returns the full path (e.g. "/usr/bin/ls") if found and executable,
// or an empty string "" if not found.
std::string getPath(const std::string& commandName){

  namespace fs = std::filesystem;

  // get the existing paths 
  const char* pathEnv = std::getenv("PATH");
  //check if the pointer is null or not
  if (pathEnv != nullptr){
    //Loop through the address
     std::string pathName = pathEnv;
     std::stringstream ss(pathName);
     std::string directory;

     while (std::getline(ss, directory, ':')){
      
      fs::path dir = directory;
      fs::path fullPath = dir / commandName;

      //check if the file or directory has access
      int returnValue = access (fullPath.c_str(), X_OK);
      if (returnValue == 0){
        return fullPath.string();
      } 
     }
  } 
  return "";
}



