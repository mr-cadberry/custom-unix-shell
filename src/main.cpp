#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_set>




std::string getPath(const std::string& commandName);


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // get input from use
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    if (command == "exit") { // check if the input is exit
      break;
    } else if (command.substr(0,5) == "echo "){ // is first 5 letters contain echo+space
        std::cout<<command.substr(5)<<"\n";
    } else if (command.substr(0,5) == "type "){ // if the first 5 letters contains type+space
      std::string commandName = command.substr(5); 
      std::unordered_set<std::string> builtins = {"echo", "exit", "type"}; // create a set, put the builtin command names in the set     
        if (builtins.count(commandName)) {  //count returns how many times the element exist, so basically 0 or 1. which can be taken as false or true
          std::cout << commandName << " is a shell builtin\n";
        } else if (std::string result = getPath(commandName); !result.empty()){
          std::cout << result << "\n";
        } else {
          std::cout << commandName <<": not found"<<"\n";
        } 
    }else {
      //output error message if command is not found
      std::cout << command << ": command not found" << std::endl; // command is invalid
    }
  }
  return 0;
}


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



