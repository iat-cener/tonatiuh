//
// Created by Javier Garcia Blas on 25/9/18.
//

#include "TreeView.h"
#include <TNodesDatabase.h>
#include "TNode.h"
#include <memory>
#include <algorithm>


int TreeView::maxId() {
    int id = 0;

    for (pugi::xpath_node item : file_.select_nodes(".//*[@id]")) {
        pugi::xml_node node = item.node();
        id = std::max(id, node.attribute("id").as_int());
    }

    return id;
}

std::shared_ptr<TNode> TreeView::CreateNodeObject(const pugi::xml_node & node)
{

    TNodeType nodeType = TNodeType::FromName( std::string(node.attribute("type").value()));

    std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
    if( !objectNode || objectNode == nullptr )
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

inline bool TreeView::validGroupNodeChild(const std::string & parent, const std::string & child) {

    /*return (parent.find("GroupNode") != std::string::npos) &&
           (child.find("GroupNode") == std::string::npos) &&
           (child.find("SceneNode") == std::string::npos) &&
           (child.find("Tracker") == std::string::npos);*/
	return true;

}

inline bool TreeView::validSurfaceNodeChild(const std::string & parent, const std::string & child) {

    /*return (parent.find("SurfaceNode") != std::string::npos) &&
           (child.find("Shape") == std::string::npos) &&
           (child.find("Material") == std::string::npos);*/
	return true;
}

inline bool TreeView::validChild(const std::string& parent, const std::string& /*child*/ ) {

    /*return (parent.find("Material") != std::string::npos) ||
           (parent.find("Tracker") != std::string::npos) ||
           (parent.find("Shape") != std::string::npos);*/
	return true;
}


std::string TreeView::addNode(const std::string & nodetype, const std::string & parent_node_url) {
    pugi::xml_node parent_node = file_.select_node(parent_node_url);

    if (!parent_node) {
        throw std::runtime_error("Parent node not found");
    }

    pugi::xml_attribute parent_type_attr = parent_node.attribute("type");
    std::string parent_type = std::string(parent_type_attr.value());

    if (!validGroupNodeChild(parent_type, nodetype)) {
        throw std::runtime_error("Impossible to append node. Parent type: " + parent_type);
    }

    if (!validSurfaceNodeChild(parent_type, nodetype)) {
        throw std::runtime_error("Impossible to append node. Parent type: " + parent_type);
    }

    if (!validChild(parent_type, nodetype)) {
        throw std::runtime_error("Impossible to append node. Parent type: " + parent_type);
    }

    pugi::xml_node childrenlist_node = parent_node.child("childrenList");

    if (!childrenlist_node) {
        childrenlist_node = parent_node.append_child("childrenList");
    }

    pugi::xml_node nodeslist_node = childrenlist_node.child("NodesList");
    if (!nodeslist_node) {
        nodeslist_node = childrenlist_node.append_child("NodesList");
        pugi::xml_attribute type = nodeslist_node.append_attribute("type");
        type.set_value("NodesList");
        pugi::xml_attribute id = nodeslist_node.append_attribute("id");
        id.set_value(maxId() + 1);
    }

    long nchildrens = std::distance(nodeslist_node.children().begin(), nodeslist_node.children().end());

    pugi::xml_node node_new = nodeslist_node.append_child(nodetype.c_str());
    if(!node_new) {
        throw std::runtime_error("Child node not added");
    }

    if (nodetype.find("GroupNode") != std::string::npos) {
        std::string name = std::string{"TSeparatorkit"} + std::to_string(nchildrens);
        node_new.set_name(name.c_str());
    }
    else if (nodetype.find("SurfaceNode") != std::string::npos) {
        std::string name = std::string{"TShapeKit"} + std::to_string(nchildrens);
        node_new.set_name(name.c_str());
    }
    else {
        node_new.set_name(nodetype.c_str());
    }

    pugi::xml_attribute type_new = node_new.append_attribute("type");
    type_new.set_value(nodetype.c_str());

    pugi::xml_attribute id_new = node_new.append_attribute("id");
    id_new.set_value(nchildrens+1);

    return node_new.path('/');
}


//deleteNode:
void TreeView::deleteNode(std::string node_url) {
    pugi::xml_node toDelete = file_.select_node(node_url);
    if (!toDelete)
        throw std::runtime_error("Node not found");
    pugi::xml_node parent = toDelete.parent();
    if (!parent)
        throw std::runtime_error("Node URL not found");
    parent.remove_child(toDelete);
}
//getRootNode:
std::shared_ptr<TNode> TreeView::getRootNode() {
    std::shared_ptr<TNode> nodeObject = nullptr;

    pugi::xml_node rootNode1 = file_.select_node("/Tonatiuh/SceneNode/childrenRoot/RootNode");

    if (rootNode1) {
        nodeObject = CreateNodeObject(rootNode1);
    } else {
        pugi::xml_node rootNode2 = file_.first_child().first_child();
        if (!rootNode2)
            throw std::runtime_error("Root node not found");
        nodeObject = CreateNodeObject(rootNode2);
    }

    return nodeObject;
}

//copyNode:
void TreeView::copyNode(const std::string & node_url)
{
    pugi::xml_node node = file_.select_node(node_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    node_ = node;
}

//copyRefNode:
void TreeView::copyRefNode(const std::string & node_url)
{
    copyNode(node_url);
}

//pasteNode:
std::string TreeView::pasteNode(const std::string & parent_url)
{
    pugi::xml_node node = file_.select_node(parent_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    pugi::xml_node chlid = node.append_copy(node_);

    chlid.attribute("id").set_value(maxId() + 1);

    return chlid.path('/');
}

//pasteRefNode:
std::string TreeView::pasteRefNode(const std::string & parent_url)
{
    pugi::xml_node node = file_.select_node(parent_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    pugi::xml_node chlid = node.append_copy(node_);

    return chlid.path('/');
}


void TreeView::setNodeName(const std::string & node_url, const std::string & new_name)
{
    pugi::xml_node node = file_.select_node(node_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    node.set_name(new_name.c_str());
}

std::string TreeView::getNodeName(const std::string & node_url)
{
    pugi::xml_node node = file_.select_node(node_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    return node.name();
}

TNodeType TreeView::getNodeParamenterType(const std::string & node_url, const std::string& /*param_name*/ )
{
    pugi::xml_node node = file_.select_node(node_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).value() );

    return nodeType;
}

std::map<std::string, std::string> TreeView::getNodeParamenterList(const std::string & node_url)
{
    typedef std::map<std::string, std::string> paramMap;
    paramMap map;

    pugi::xml_node node = file_.select_node(node_url);

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
    pugi::xml_node node = file_.select_node(node_url);

    if(!node)
        throw std::runtime_error("Node URL not found");

    std::shared_ptr< TNode > nodeObject = CreateNodeObject( node );

    return nodeObject;
}

//getNodeType:
std::string TreeView::getNodeType(const std::string & node_url)
{
    pugi::xml_node node = file_.select_node(node_url);

    if(!node) {
        throw std::runtime_error("Node URL not found");
    }

    pugi::xml_attribute type = node.attribute("type");

    if(!type) {
        throw std::runtime_error("Type attribute not found");
    }

    return std::string(type.value());
}

std::string TreeView::getParameter(const std::string & node_url, const std::string & param) {
    pugi::xml_node node = file_.select_node(node_url);

    if(!node) {
        throw std::runtime_error("Node URL not found");
    }

    pugi::xml_attribute attr = node.attribute(param.c_str());

    if (!attr) {
        throw std::runtime_error("Parameter not found");
    }

    return std::string(attr.value());
}

void TreeView::setParameter(const std::string & node_url, const std::string & param, const std::string & value) {
    pugi::xml_node node = file_.select_node(node_url);

    if(!node) {
        throw std::runtime_error("Node URL not found");
    }

    pugi::xml_attribute attr = node.attribute(param.c_str());

    if (!attr) {
        throw std::runtime_error("Parameter not found");
    }

    attr.set_value(value.c_str());
}
