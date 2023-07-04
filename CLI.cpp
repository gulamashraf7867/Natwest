#include "CLI.h"

std:: string Cli::getNotesName(){
std::cout << "Enter the Notes name\n";
    getline(std::cin, notesName);
    notesName = notesName + ".txt";
    openNewNotes(notesName);
    return notesName;
}

void Cli::openNewNotes(std::string fileName){
    std::ofstream file(fileName);
    if (file.is_open()) {
        std::cout << "Creating "<<fileName<< " Notes....." << std::endl;
        createNotes(fileName);
        
    } 
    else
    {
        std::cout << "this notes already exists" << std::endl;
        
    }
}

void Cli::createNotes(std::string fileName)
{
    std::string notes;
    std::string line;
    std::cout<<"type __close__ in a new line to close the notes "<<std::endl;
    while (std::getline(std::cin, line)) {
        if(line == "__close__")
        break;
        
        notes += line + "\n";
    }
    std::ofstream file(fileName,std::ios::app);
    if (file.is_open())
        file << notes;
    file.close();
}