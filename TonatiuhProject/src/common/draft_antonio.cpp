
	// //#include "../../thirdparty/pugixml/src/pugixml.hpp"
	// #include "pugi/pugixml.hpp"
	// #include <iostream>
	// //to return Tnodes:
	// #include "TNode.h"
	// #include "TNodeType.h"
	// #include "TNode.cpp"
	// #include "TNodesDocument"
	
	
	// //-- Sobre esta clase trabajará la libreria
	// class lib_xml_document
	// {

	// public:
	// 	lib_xml_document();
	// 	~lib_xml_document();

	// protected:

	// private:
	// 	pugi::xml_document doc;
	// 	pugi::xml_node copied_node;
	// 	pugi::xml_node *copied_node_ref;

	// };

	
	// //-- TREE VIEW --
	// //addNode:
	// int addNode(std::string nodetype, std::string parent_node_url)
	// {
	// 	pugi::xml_node parentNode = doc.select_node(parent_node_url);
	// 	if(!parentNode) return -1;
	// 	pugi::xml_node toAdd = parentNode.append_child(nodetype.c_str());
	// 	if(!toAdd) return -1;
	// 	return 0;
	// }
	// //deleteNode:
	// int deleteNode(std::string node_url)
	// {
	// 	pugi::xml_node toDelete = doc.select_node(node_url);
	// 	if(!toDelete) return -1;
	// 	pugi::xml_node parent = toDelete.parent();
	// 	if(!parent) return -1;
	// 	parent.remove_child(toDelete);
	// 	return 0;
	// }
	// //getRootNode:
	// int getRootNode()
	// {//TIENE QUE DEVOLVER UN TNODE
	// 	rootNode =  doc.first_child();
	// 	if(!rootNode) return -1;
		
	// 	TNodeType nodeType = TNodeType::FromName( rootNode.attribute( "type" ).value() );
	// 	std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
	// 	if( !objectNode || objectNode == 0 ) return -1;

	// 	objectNode->SetName( node.name() );
	// 	for(pugi::xml_attribute attr: node.attributes())
	// 	{

	// 		std::string attributeName =attr.name();
	// 		if( ( std::string(attributeName) != "type" ) && ( std::string(attributeName) != "name" )  && ( std::string(attributeName) != "id" ))
	// 		{
	// 			std::string parameterValue = attr.value();
	// 			if( !objectNode->SetParameterFormString( attributeName, parameterValue ) )
	// 			{
	// 				//*errMsg= std::string("TNodesDocument::CreateNodeObject. Error defining the attribute '" ) + node.name() +
	// 				//		std::string( "' of node '" ) + attributeName +
	// 				//		std::string( "' and value '") + parameterValue + std::string("'." );

	// 			    return -1;
	// 			}
	// 		}
	// 	}

 //   		//return ( objectNode );
	// 	return 0;
		
	// }
	// //copyNode:
	// int copyNode(std::string node_url)
	// {
	// 	//Pugixml documentacion:
	// 	// you can’t create a standalone copy - you have to immediately insert it somewhere in the tree.
	// 	pugi::xml_node toCopy = doc.select_node(node_url);
	// 	if(!parent) return -1;
	// 	copied_node = toCopy;
	// 	return 0;
	// }
	// //copyRefNode:
	// pugi::xml_node& copyRefNode(std::string node_url)
	// {//HA DE DEVOLVER LA REFERENCIA AL NODO
	// 	pugi::xml_node toCopy = doc.select_node(node_url);
	// 	if(!parent) //handle error--> no especificado;
	// 	//*copied_node_ref = &toCopy;
	// 	return toCopy;
	// }
	// //pasteNode:
	// int pasteNode(std::string node_url, std::string parent_node_url)
	// {
	// 	pugi::xml_node toCopy = copied_node;
	// 	pugi::xml_node parent = doc.select_node(parent_node_url);
	// 	//Harian lo mismo:
	// 	//parent.first_child().prepend_copy(toCopy);
	// 	parent.prepend_child(toCopy);
	// 	return 0;
	// }
	// //pasteRefNode:
	// int pasteRefNode(pugi::xml_node &toPaste, std::string parent_node_url)
	// {

	// 	return 0;
	// }
	// //setNodeName:
	// int setNodeName(std::string node_url,  std::string new_name)
	// {
	// 	pugi::xml_node node = doc.select_node(node_url);
	// 	node.set_name(new_name.c_str());
	// 	return 0;
	// }
	// //getNodeName:
	// std::string getNodeName(std::string node_url)
	// {
	// 	pugi::xml_node node = doc.select_node(node_url);
	// 	return node.name();
	// }
	// //getNodeParamenter:
	// //DEVUELVE VARIOS TIPOS: usar template?

	// //setNodeParameter:
	// //RECIBE VARIOS TIPOS: usar template?

	// //getNodeParameterType:
	// std::string getNodeParamenterType(std::string node_url, std::string param_name)
	// {
	// 	pugi::xml_node node = doc.select_node(node_url);
	// 	pugi::xml_attribute attr = node.attribute(param_name.c_str())
	// 	//probar si existe:
	// 	//https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
	// 	return attr.type();
	// }
	// //getNodeParameterList:
	// //getNode:
	// //getNodeType:
	// std::string getNodeType(std::string node_url)
	// {
	// 	pugi::xml_node node = doc.select_node(node_url);
	// 	return node.type();
	// }
	// //getNodeIcon:

	// //-- SUN LIGHT FUNCTIONS --

	// //setSunShape:
	// int setSunShape(std::string node_url, std::vector<std::string> param_list)
	// {
	// 	this->doc.child("sun_node").attribute("type").set_value(param_list[0].c_str());
	// 	this->doc.child("sun_node").attribute("azimuth").set_value(param_list[1].c_str());
	// 	this->doc.child("sun_node").attribute("disabledNodes").set_value(param_list[2].c_str());
	// 	this->doc.child("sun_node").attribute("id").set_value(param_list[3].c_str());
	// 	this->doc.child("sun_node").attribute("zenith").set_value(param_list[4].c_str());
	// }
	// //getSunShape:
	// //setSunPosition:
	// //getSunPosition:
	// //setSunArea:
	// //setTranssitivity:
	// //getTranssitivity:

	// /*
	// 	void setSunNode( std::string type,
	// 	double azimuth,
	// 	std::string disabledNodes,
	// 	int id,
	// 	double zenith,
	// 	int irradiance,
	// 	std::string type_child,
	// 	double cs,
	// 	int id_child)
	// {
	// 	//Sun node
	// 	doc.child("sun_node").attribute("type").set_value(type.c_str());
	// 	doc.child("sun_node").attribute("azimuth").set_value(azimuth);
	// 	doc.child("sun_node").attribute("disabledNodes").set_value(disabledNodes);
	// 	doc.child("sun_node").attribute("id").set_value(id);
	// 	doc.child("sun_node").attribute("zenith").set_value(zenith);
	// 	//Sun shape model
	// 	doc.child("sun_node").attribute("irradiance").set_value(irradiance);
	// 	doc.child("sun_node").attribute("type").set_value(type_child);
	// 	doc.child("sun_node").attribute("csr").set_value(csr);
	// 	doc.child("sun_node").attribute("id").set_value(id_child);
	// }
	// */

	// //-- FILE FUNCTIONS -- 

	// //newFile:
	// int newFile()
	// {
	//     //pugi::xml_parse_result result = doc.load_file("filename");
	// }
	// //openFile:
	// int openFile(std::string filename, std::string path)
	// {
	// 	if(path.back() =! "/") path += "/";
	// 	if(filename.substr(filename.find_last_of(".") + 1) != "xml") return -1;
	// 	path += filename;

	// 	xml_parse_result result = this.doc.load_file("filename");
	// 	if (!result) return -1;
	// 	return 0;

	// }
	// //saveFile:
	// int saveFile(std::string filename, std::string path)
	// {
	// 	if(path.back() =! "/") path += "/";
	// 	if(s.find('.') == std::string::npos) filename += ".xml";
	// 	if(filename.substr(filename.find_last_of(".") + 1) != "xml") return -1;
	// 	path += filename;

	// 	this->doc.save_file("path");
		
	// 	return 0;
	// }
	// //saveComponent
	// //insertUserComponent:

	// //-- RUNNING FUNCTIONS --