/*#include "SunLight.h"
#include "TContainerNode.h"
#include "TNode.h"
#include "TNodeType.h"
#include "TNodesList.h"
#include "TParameter.h"
#include "TParameterList.h"
*/
#include "File.h"
#include "../../thirdparty/pugixml/src/pugixml.hpp"

#include <map>
#include <iterator>
#include <memory>

File::File(const std::string &filename) {
  pugi::xml_node decl = doc_.prepend_child(pugi::node_declaration);
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "UTF-8";

  auto documentRootElement = doc_.append_child("Tonatiuh");

  bool saveSucceeded = doc_.save_file(filename.c_str(), PUGIXML_TEXT("  "));
  if (!saveSucceeded) {
    throw std::runtime_error("Error creating file " + filename);
  }
}

void File::openFile(const std::string &filename) {
  auto result = doc_.load_file(filename.c_str(),
                               pugi::parse_default | pugi::parse_declaration);
  if (!result) {
    throw std::runtime_error("Error opening file " + filename);
  }
}

void File::saveFile(const std::string & filename)
{
  bool saveSucceeded = doc_.save_file(filename.c_str(), PUGIXML_TEXT("  "));
  if (!saveSucceeded) {
    throw std::runtime_error("Error saving file " + filename);
  }
}


//insertUserComponent:

