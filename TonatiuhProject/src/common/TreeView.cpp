//
// Created by Javier Garcia Blas on 25/9/18.
//

#include "TreeView.h"
#include <memory>

TreeView::TreeView(const std::string & path) {
    if (!doc_.load_file(path.c_str()))
    {
        std::cerr<< "Cannot find " << path << "\n" <<std::endl;
    }
}

std::shared_ptr<TNode> TreeView::CreateNodeObject(const pugi::xml_node & node)
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


int TreeView::addNode(const std::string & nodetype, const std::string & parent_node_url)
{
    pugi::xpath_query query(parent_node_url.c_str());
    pugi::xml_node parentNode = doc_.select_node(query).node();
    if(!parentNode) return -1;
    pugi::xml_node toAdd = parentNode.append_child(nodetype.c_str());
    if(!toAdd) return -1;
    return 0;
}
//deleteNode:
int TreeView::deleteNode(std::string node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node toDelete = doc_.select_node(query).node();
    if(!toDelete) return -1;
    pugi::xml_node parent = toDelete.parent();
    if(!parent) return -1;
    parent.remove_child(toDelete);
    return 0;
}
//getRootNode:
std::shared_ptr<TNode> TreeView::getRootNode()
{
    pugi::xml_node rootNode =  doc_.first_child();
    if(!rootNode) return nullptr;

    std::shared_ptr< TNode > nodeObject = CreateNodeObject( rootNode );

    //return ( objectNode );
    return nodeObject;

}
//copyNode:
int TreeView::copyNode(const std::string & node_url)
{

    return 0;
}

//copyRefNode:
void TreeView::copyRefNode(const std::string & node_url)
{

}

//pasteNode:
int TreeView::pasteNode(const std::string & node_url, const std::string & parent_node_url)
{
    return 0;
}
//pasteRefNode:
int TreeView::pasteRefNode(const pugi::xml_node & toPaste, const std::string & parent_node_url)
{
    return 0;
}
//setNodeName:
int TreeView::setNodeName(const std::string & node_url, const std::string & new_name)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node node = doc_.select_node(query).node();
    node.set_name(new_name.c_str());
    return 0;
}
//getNodeName:
std::string TreeView::getNodeName(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node node = doc_.select_node(query).node();
    return node.name();
}
//getNodeParamenter:
//DEVUELVE VARIOS TIPOS: usar template?

//setNodeParameter:
//RECIBE VARIOS TIPOS: usar template?

//getNodeParameterType:
TNodeType TreeView::getNodeParamenterType(const std::string & node_url, const std::string & param_name)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node node = doc_.select_node(query).node();
    TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).value() );

    return nodeType;
}
//getNodeParameterList:
std::map<std::string, std::string> TreeView::getNodeParamenterList(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node node = doc_.select_node(query).node();

    typedef std::map<std::string, std::string> paramMap;
    paramMap map;


    //EN EL EJEMPLO USAN DE node UN xml_node_iterator
    pugi::xml_node_iterator nit = node.begin();
    for(pugi::xml_attribute_iterator ait = nit->attributes_begin(); ait != nit->attributes_end(); ++ait)
    {
        map.insert(paramMap::value_type(ait->name(), ait->value()));
    }

    return map;
}
//getNode:
std::shared_ptr<TNode> TreeView::getNode(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node node = doc_.select_node(query).node();

    std::shared_ptr< TNode > nodeObject = CreateNodeObject( node );

    //return ( objectNode );
    return nodeObject;

}

//getNodeType:
std::string TreeView::getNodeType(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node node = doc_.select_node(query).node();
    return std::string(node.value());
}
//getNodeIcon: