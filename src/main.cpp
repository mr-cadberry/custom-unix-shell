#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // get input from user
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    //extract the first word from command to firstword variable
    // std::istringstream stream(command);
    // std::string firstword; //declare firstword variable
    // stream >> firstword;

    if (command == "exit") {
      break;
    } else if (command.substr(0,5) == "echo "){
        std::cout<<command.substr(5)<<"\n";
    } else {
      //output error message if command is not found
      std::cout << command << ": command not found" << std::endl;
    }
  }

}
