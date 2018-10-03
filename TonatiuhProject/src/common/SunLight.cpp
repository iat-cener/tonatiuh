//
// Created by Javier Garcia Blas on 25/9/18.
//

#include "SunLight.h"
#include "TContainerNode.h"
#include "TNode.h"
#include "TNodeType.h"
#include "TNodesList.h"
#include "TParameter.h"
#include "TParameterList.h"

#include <map>
#include <iterator>
#include <memory>

/*!
 * Reads the content of the node \a node and sets inside of the \a parentNode.
 */
std::shared_ptr<TNode> SunLight::CreateNodeObject(const pugi::xml_node & node)
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

int SunLight::setSunShape(const std::string & nodeurl, const std::map<std::string,std::string> & parameterList){
    pugi::xpath_query query_sunShape("light/sun_node/sunshape");
    pugi::xpath_node_set xpath_sunShapes = doc_.select_nodes(query_sunShape);

    pugi::xpath_node sunShape = xpath_sunShapes.first();
    pugi::xml_node node_sunShape = sunShape.node();
    pugi::xml_node model = node_sunShape.first_child();

    for (std::pair<std::string,std::string> parameter : parameterList) {
        model.remove_attribute(parameter.first.c_str());
        auto attr = model.append_attribute(parameter.first.c_str());
        attr.set_value(parameter.second.c_str());
    }

    return 0;
}

int SunLight::getSunShape(const std::string & nodeurl, TNode & node){
    pugi::xpath_query query_sunShape("light/sun_node/sunshape");
    pugi::xpath_node_set xpath_sunShapes = doc_.select_nodes(query_sunShape);

    pugi::xpath_node sunShape = xpath_sunShapes.first();
    pugi::xml_node node_sunShape = sunShape.node();
    pugi::xml_node model = node_sunShape.first_child();

    std::shared_ptr< TNode > nodeObject = CreateNodeObject( model );

    return 0;
}



int SunLight::setSunPosition(const std::string & nodeurl, float zenith, float azimuth){
    pugi::xpath_query query_sun_node("light/sun_node");
    pugi::xpath_node_set xpath_sun_nodes = doc_.select_nodes(query_sun_node);

    pugi::xpath_node sun_node = xpath_sun_nodes.first();
    pugi::xml_node node = sun_node.node();

    node.attribute("azimuth").set_value(azimuth);
    node.attribute("zenith").set_value(zenith);

    return 0;
}

int SunLight::setSunArea(const std::string & nodeurl){
    return 0;
}

int SunLight::setTransmissivity (const std::string & nodeurl) {
    return 0;
}

std::shared_ptr<TNode> SunLight::getTransmissivity (const std::string & nodeurl) {
    return nullptr;
}