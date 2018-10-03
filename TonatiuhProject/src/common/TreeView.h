//
// Created by Javier Garcia Blas on 25/9/18.
//

#ifndef TONATIUH_TREEVIEW_H
#define TONATIUH_TREEVIEW_H

#include "../../thirdparty/pugixml/src/pugixml.hpp"
#include <iostream>
#include <memory>
#include "TNode.h"
#include "TNodeType.h"
#include "TNode.h"

class TreeView {

protected:
    TreeView(const pugi::xml_document & doc);
    TreeView(const std::string & path);
    std::shared_ptr<TNode> CreateNodeObject(const pugi::xml_node & node);
    int addNode(const std::string & nodetype, const std::string & parent_node_url);
    int deleteNode(std::string node_url);
    std::shared_ptr<TNode> getRootNode();
    int copyNode(const std::string & node_url);
    void copyRefNode(const std::string & node_url);
    int pasteNode(const std::string & node_url, const std::string & parent_node_url);
    int pasteRefNode(const pugi::xml_node & toPaste, const std::string & parent_node_url);
    int setNodeName(const std::string & node_url, const std::string & new_name);
    std::string getNodeName(const std::string & node_url);
    TNodeType getNodeParamenterType(const std::string & node_url, const std::string & param_name);
    
    std::map<std::string, std::string> getNodeParamenterList(const std::string & node_url);
    std::shared_ptr<TNode> getNode(const std::string & node_url);

    std::string getNodeType(const std::string & node_url);
    
private:
    pugi::xml_document doc_;
};


#endif //TONATIUH_TREEVIEW_H
