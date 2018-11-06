#ifndef TONATIUH_FILE_H
#define TONATIUH_FILE_H


#include "../../thirdparty/pugixml/src/pugixml.hpp"

//#include <iostream>
#include <memory>
#include <sstream>

#include "TNode.h"
#include "TNodeType.h"
#include "TNode.h"

class File {
public:
  File () = default;

  // Constructor
  explicit File(const std::string & filename);

  // Open
  void openFile(const std::string & filename);

  // Save
  void saveFile(const std::string & filename);
  void saveComponent(const std::string & filename) {
      saveFile(filename);
  }

  std::vector<std::string> split(std::string strToSplit, char delimeter)
  {
      std::stringstream ss(strToSplit);
      std::string item;
      std::vector<std::string> splittedStrings;
      while (std::getline(ss, item, delimeter))
      {
          splittedStrings.push_back(item);
      }
      return splittedStrings;
  }

  auto select_nodes(const std::string & query) {
        return doc_.select_nodes(pugi::xpath_query{query.c_str()});
  }


  pugi::xml_node select_node(const std::string & query) {

      pugi::xml_node node;
      std::vector<std::string> elements;

      elements = split(query, '/');

      std::string newquery = "//*[local-name(.) = '" + elements.back() + "']";
      pugi::xpath_node_set xset =  doc_.select_nodes(pugi::xpath_query{newquery.c_str()});

      for (pugi::xpath_node  xpath : xset) {
          bool found = true;
          auto node_xpath = xpath.node();
          std::string path = std::string(node_xpath.path('/'));

          size_t pos = 0;
          for (std::string elem : elements) {
              auto find_elem = path.find(elem, pos);
              found = found && find_elem != std::string::npos;

              if (found) {
                  pos =  find_elem + elem.length();
              }
          }
          if (found) {
              return node_xpath;
          }
      }
      throw std::runtime_error("Node '" + query + "' not found.");
  }

  auto first_child() {
      return doc_.first_child();

  }

  private:
    pugi::xml_document doc_;
};


#endif //TONATIUH_FILE_H
