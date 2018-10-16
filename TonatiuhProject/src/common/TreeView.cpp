//
// Created by Javier Garcia Blas on 25/9/18.
//

#include "TreeView.h"
#include <memory>

TreeView::TreeView(const std::string & path) {
    if (!doc_.load_file(path.c_str()))
    {
        throw std::runtime_error("Cannot find path " + path);
    }
}

std::shared_ptr<TNode> TreeView::CreateNodeObject(const pugi::xml_node & node)
{
    TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).value() );
    std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
    if( !objectNode || objectNode == 0 )
    {
        throw std::runtime_error("Node type not found");
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
                throw std::runtime_error("Runtime error!");
            }
        }
    }

    return objectNode;
}


void TreeView::addNode(const std::string & nodetype, const std::string & parent_node_url)
{
    pugi::xpath_query query(parent_node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Parent node not found");

    pugi::xml_node node_new = node.append_child(nodetype.c_str());
    if(!node_new)
        throw std::runtime_error("Child node not added");
}


//deleteNode:
void TreeView::deleteNode(std::string node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xml_node toDelete = doc_.select_node(query).node();
    if(!toDelete)
        throw std::runtime_error("Node not found");
    pugi::xml_node parent = toDelete.parent();
    if(!parent)
        throw std::runtime_error("Node URL not found");
    parent.remove_child(toDelete);
}
//getRootNode:
std::shared_ptr<TNode> TreeView::getRootNode()
{
    pugi::xml_node rootNode =  doc_.first_child();
    if(!rootNode)
        throw std::runtime_error("Root node not found");

    std::shared_ptr< TNode > nodeObject = CreateNodeObject( rootNode );

    return nodeObject;

}
//copyNode:
void TreeView::copyNode(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    node_ = node;
}

//copyRefNode:
void TreeView::copyRefNode(const std::string & node_url)
{

}

//pasteNode:
void TreeView::pasteNode(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    node = node_;
}

//pasteRefNode:
void TreeView::pasteRefNode(const pugi::xml_node & toPaste, const std::string & parent_node_url)
{
}


void TreeView::setNodeName(const std::string & node_url, const std::string & new_name)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    node.set_name(new_name.c_str());
}

std::string TreeView::getNodeName(const std::string & node_url)
{

    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    return node.name();
}

TNodeType TreeView::getNodeParamenterType(const std::string & node_url, const std::string & param_name)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).value() );

    return nodeType;
}

std::map<std::string, std::string> TreeView::getNodeParamenterList(const std::string & node_url)
{
    typedef std::map<std::string, std::string> paramMap;
    paramMap map;

    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

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
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    std::shared_ptr< TNode > nodeObject = CreateNodeObject( node );

    return nodeObject;

}

//getNodeType:
std::string TreeView::getNodeType(const std::string & node_url)
{
    pugi::xpath_query query(node_url.c_str());
    pugi::xpath_node_set xpath = doc_.select_nodes(query);
    pugi::xpath_node xpath_node = xpath.first();
    pugi::xml_node node = xpath_node.node();

    if(!node)
        throw std::runtime_error("Node URL not found");

    return std::string(node.value());
}
//getNodeIcon:
