#include "main.h"


int main() {
    std::cout << "Hello World!" << std::endl;
    
    FileManager manager;
    manager.exec("mkdir -v Downloads Documents Desktop Music Pictures Videos Public");
    manager.exec("mkdir -v Documents/prog Documents/tasks Documents/notes Documents/pron Documents/work Documents/homework");
    // manager.exec("cd Documents");
    // manager.exec("pwd");
    // manager.exec("ls -a");
    // manager.exec("ls -a -R");
    // std::cout << "START" << '\n';
    // manager.exec("find / Doc*p");
    // manager.exec("count / Doc*p");
    
    // User input loop
    std::string line;
    std::cout << '\n' << ">>> ";
    for (std::string line; std::getline(std::cin, line, '\n');) {
        manager.exec(line);
        std::cout << '\n' << ">>> ";
    }
    
    
    std::cout << "Goodbye Wrld!" << std::endl;
    return 0;
}
