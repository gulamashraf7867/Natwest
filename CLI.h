#include <iostream>
#include <string>
#include <fstream>
class Cli{
    std::string notesName;
    public:
      std::string getNotesName();
      void openNewNotes(std::string fileName);
      void createNotes(std::string fileName);
};