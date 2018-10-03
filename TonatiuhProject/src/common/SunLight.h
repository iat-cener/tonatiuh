//
// Created by Javier Garcia Blas on 25/9/18.
//

#ifndef TONATIUH_SUNLIGHT_H
#define TONATIUH_SUNLIGHT_H


#include "../../thirdparty/pugixml/src/pugixml.hpp"

class TContainerNode;
class TNode;
class TNodesList;



#include <iostream>
#include <memory>
#include "TNode.h"
#include "TNodeType.h"
#include "TNode.h"


class SunLight {
protected:
    int setSunShape(const std::string & nodeurl, const std::map<std::string,std::string> & parameterList);
    int getSunShape(const std::string & nodeurl, TNode & node);
    int setSunPosition(const std::string & nodeurl, float zenith, float azimuth);
    int setSunArea(const std::string & nodeurl);
    int setTransmissivity (const std::string & nodeurl);
    std::shared_ptr<TNode> getTransmissivity (const std::string & nodeurl);
    std::shared_ptr<TNode> CreateNodeObject(const pugi::xml_node & node);
private:
    pugi::xml_document doc_;
};


#endif //TONATIUH_SUNLIGHT_H
