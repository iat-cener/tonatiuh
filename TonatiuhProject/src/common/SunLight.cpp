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
#include <utility>

/*!
 * Reads the content of the node \a node and sets inside of the \a parentNode.
 */
std::shared_ptr<TNode> SunLight::CreateNodeObject(const pugi::xml_node & node)
{
    TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).value() );
    std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
    if( !objectNode || objectNode == 0 )
    {
        throw std::runtime_error("Runtime error!");
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

void SunLight::setSunShape(const std::string & nodeurl, const std::map<std::string,std::string> & parameterList){
    pugi::xpath_query query_sunShape("light/sun_node/sunshape");
    pugi::xpath_node_set xpath_sunShapes = doc_.select_nodes(query_sunShape);

    if(!xpath_sunShapes.size())
        throw std::runtime_error("Sunshape not found");

    if (!parameterList.size()) {
        throw std::runtime_error("Used empty parameter list");
    }

    pugi::xpath_node sunShape = xpath_sunShapes.first();
    pugi::xml_node node_sunShape = sunShape.node();
    pugi::xml_node model = node_sunShape.first_child();

    for (std::pair<std::string,std::string> parameter : parameterList) {
        pugi::xml_attribute attr = model.attribute(parameter.first.c_str());
        if (attr)
            attr.set_value(parameter.second.c_str());
        else {
            pugi::xml_attribute attr_new = model.append_attribute(parameter.first.c_str());
            attr_new.set_value(parameter.second.c_str());
        }
    }
}

std::shared_ptr<TNode> SunLight::getSunShape(const std::string & nodeurl){
    pugi::xpath_query query_sunShape("light/sun_node/sunshape");
    pugi::xpath_node_set xpath_sunShapes = doc_.select_nodes(query_sunShape);

    if(!xpath_sunShapes.size())
        throw std::runtime_error("Sunshape not found");

    pugi::xpath_node sunShape = xpath_sunShapes.first();
    pugi::xml_node node_sunShape = sunShape.node();
    pugi::xml_node node = node_sunShape.first_child();

    std::shared_ptr< TNode > nodeObject = CreateNodeObject(node);

    return nodeObject;
}



void SunLight::setSunPosition(const std::string & nodeurl, float zenith, float azimuth){
    pugi::xpath_query query_sun_node("light/sun_node");
    pugi::xpath_node_set xpath_sun_nodes = doc_.select_nodes(query_sun_node);

    if(!xpath_sun_nodes.size())
        throw std::runtime_error("Sun node not found");

    pugi::xpath_node sun_node = xpath_sun_nodes.first();
    pugi::xml_node node = sun_node.node();

    pugi::xml_attribute attr_azimuth = node.attribute("azimuth");
    if (!attr_azimuth)
        throw std::runtime_error("azimuth attribute not found");
    attr_azimuth.set_value(azimuth);

    pugi::xml_attribute attr_zenith = node.attribute("zenith");

    if (!attr_zenith)
        throw std::runtime_error("zenith attribute not found");
    attr_zenith.set_value(zenith);
}

void SunLight::setSunArea(const std::string & disabledNodes){
    pugi::xpath_query query_area_node("light/Light");
    pugi::xpath_node_set xpath_area_nodes = doc_.select_nodes(query_area_node);

    if(!xpath_area_nodes.size())
        throw std::runtime_error("Sun node not found");

    pugi::xpath_node light_node = xpath_area_nodes.first();
    pugi::xml_node node = light_node.node();

    pugi::xml_attribute attr = node.attribute("disabledNodes");

    if (!attr)
        throw std::runtime_error("disabledNodes attribute not found");
    attr.set_value(disabledNodes.c_str());
}

void SunLight::setTransmissivity (const std::string & nodeurl, const std::map<std::string, std::string> & parameters) {

    pugi::xpath_query query_transmissivity_node(nodeurl.c_str());
    pugi::xpath_node_set xpath_transmissivity_nodes = doc_.select_nodes(query_transmissivity_node);

    if(!xpath_transmissivity_nodes.size())
        throw std::runtime_error("Transmissivity node not found");

    if (!parameters.size())
        throw std::runtime_error("Used empty parameters list");

    pugi::xpath_node transmissivity_node = xpath_transmissivity_nodes.first();
    pugi::xml_node node = transmissivity_node.node();
    for (std::pair<std::string, std::string> param : parameters ) {
        pugi::xml_attribute attr = node.attribute(param.first.c_str());
        if (attr)
            attr.set_value(param.second.c_str());
        else {
            pugi::xml_attribute attr_new = node.append_attribute(param.first.c_str());
            attr_new.set_value(param.second.c_str());
        }
    }
}

std::shared_ptr<TNode> SunLight::getTransmissivity (const std::string & nodeurl)  {
    pugi::xpath_query query_transmissivity_node(nodeurl.c_str());
    pugi::xpath_node_set xpath_transmissivity_nodes = doc_.select_nodes(query_transmissivity_node);

    if(!xpath_transmissivity_nodes.size())
        throw std::runtime_error("Transmissivity node not found");

    pugi::xpath_node transmissivity_node = xpath_transmissivity_nodes.first();
    pugi::xml_node node = transmissivity_node.node();

    std::shared_ptr< TNode > nodeObject = CreateNodeObject(node);

    return nodeObject;
}
