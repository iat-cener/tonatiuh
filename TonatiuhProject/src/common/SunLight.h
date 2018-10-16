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
    void setSunShape(const std::string & nodeurl, const std::map<std::string,std::string> & parameterList);
    std::shared_ptr<TNode> getSunShape(const std::string & nodeurl);

    void setSunPosition(const std::string & nodeurl, float zenith, float azimuth);

    void setSunArea(const std::string & nodeurl);

    void setTransmissivity (const std::string & nodeurl, const std::map<std::string, std::string> & parameters);
    std::shared_ptr<TNode> getTransmissivity (const std::string & nodeurl);

private:
    std::shared_ptr<TNode> CreateNodeObject(const pugi::xml_node & node);

private:
    pugi::xml_document doc_;
};


#endif //TONATIUH_SUNLIGHT_H
