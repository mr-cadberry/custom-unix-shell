#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // get input from user
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    if (command == "exit") { // check if the input is exit
      break;
    } else if (command.substr(0,5) == "echo "){ // is first 5 letters contain echo+space
        std::cout<<command.substr(5)<<"\n";
    } else if (command.substr(0,5) == "type "){
      std::string commandName = command.substr(5);
      std::unordered_set<std::string> builtins = {"echo", "exit", "type"};
      if (builtins.count(commandName)) {
        std::cout << commandName << " is a shell builtin\n";
      } else {
        std::cout << commandName <<": not found"<<"\n";
      }
      
    }
    
    else {
      //output error message if command is not found
      std::cout << command << ": command not found" << std::endl; // command is invalid
    }
  }

}
