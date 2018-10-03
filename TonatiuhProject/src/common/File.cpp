/*#include "SunLight.h"
#include "TContainerNode.h"
#include "TNode.h"
#include "TNodeType.h"
#include "TNodesList.h"
#include "TParameter.h"
#include "TParameterList.h"
*/
#include "File.h"

#include <map>
#include <iterator>
#include <memory>

/*!
 * Reads the content of the node \a node and sets inside of the \a parentNode.
 */
/*std::shared_ptr<TNode> SunLight::CreateNodeObject(const pugi::xml_node & node)
{
    TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).value() );
    std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
    if( !objectNode || objectNode == 0 )
    {
        return 0;
    }

    objectNode->SetName( node.name() );
    for(pugi::xml_attribute attr: node.attributes())
    {

        std::string attributeName =attr.name();
        if( (attributeName != "type" ) && ( attributeName != "name" )  && ( attributeName != "id" ))
        {
            std::string parameterValue = attr.value();
            if( !objectNode->SetParameterFormString( attributeName, parameterValue ) )
            {
                return 0 ;
            }
        }
    }

    return objectNode;
}
*/
    //newFile:
    //openFile:
int File::openFile(std::string filename, std::string path)
{
    if(path.back() =! "/") path += "/";
    if(filename.substr(filename.find_last_of(".") + 1) != "xml") return -1;
    path += filename;

    pugi::xml_parse_result result = doc.load_file("filename");
    if (!result) return -1;
    return 0;

}
    //saveFile:
int File::saveFile(std::string filename, std::string path)
{
    if(path.back() =! "/") path += "/";
    if(filename.find('.') == std::string::npos) filename += ".xml";
    if(filename.substr(filename.find_last_of(".") + 1) != "xml") return -1;
    path += filename;

    doc.save_file("path");

    return 0;
}
//saveComponent
//insertUserComponent:

