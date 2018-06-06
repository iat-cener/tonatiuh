/*
 * MaterialOneSideSpecularFactory.h
 *
 *  Created on: 17/11/2015
 *      Author: amutuberria
 */

#ifndef MATERIALONESIDESPECULARFACTORY_H_
#define MATERIALONESIDESPECULARFACTORY_H_


#include "TMaterialFactory.h"
#include "MaterialOneSideSpecular.h"

class MaterialOneSideSpecularFactory: public TMaterialFactory
{

public:
    ~MaterialOneSideSpecularFactory()
    {

    }
    void Init() const;
    std::string  TMaterialName() const;
    std::string  TMaterialIcon() const;
    std::shared_ptr< TMaterial > CreateTMaterial( ) const;
};

#endif /* MATERIALONESIDESPECULARFACTORY_H_ */
