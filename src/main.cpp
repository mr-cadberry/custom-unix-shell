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
    //output error message if command is not found
    std::cout << command << ": command not found" << std::endl;
  }

}
