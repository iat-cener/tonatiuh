#ifndef TONATIUH_FILE_H
#define TONATIUH_FILE_H


#include "../../thirdparty/pugixml/src/pugixml.hpp"

#include <iostream>
#include <memory>
#include "TNode.h"
#include "TNodeType.h"
#include "TNode.h"


//-- Sobre esta clase trabajará la libreria
	class File
	{
	protected:
	//newFile:
	int openFile(std::string filename, std::string path);
	int saveFile(std::string filename, std::string path);
	//saveComponent:
	//insertUserComponent:

		/*int setSunShape(const std::string & nodeurl, const std::map<std::string,std::string> & parameterList);
	    int getSunShape(const std::string & nodeurl, TNode & node);
	    int setSunPosition(const std::string & nodeurl, float zenith, float azimuth);
	    int setSunArea(const std::string & nodeurl);
	    int setTransmissivity (const std::string & nodeurl);
	    std::shared_ptr<TNode> getTransmissivity (const std::string & nodeurl);
	    std::shared_ptr<TNode> CreateNodeObject(const pugi::xml_node & node);*/
	private:
		pugi::xml_document doc;
	};


#endif //TONATIUH_FILE_H
