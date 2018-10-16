#ifndef TONATIUH_FILE_H
#define TONATIUH_FILE_H


#include "../../thirdparty/pugixml/src/pugixml.hpp"

#include <iostream>
#include <memory>
#include "TNode.h"
#include "TNodeType.h"
#include "TNode.h"

class File {
public:
  // Constructor
  File(const std::string & filename);

  // Open
  void openFile(const std::string & filename);

  // Save
  void saveFile(const std::string & filename);
  void saveComponent(const std::string & filename) {
      saveFile(filename);
  }

private:
  pugi::xml_document doc_;
};


#endif //TONATIUH_FILE_H
