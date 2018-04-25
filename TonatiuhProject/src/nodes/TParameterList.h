/*
 * TParameterList.h
 *
 *  Created on: 07/09/2016
 *      Author: amutuberria
 */

#ifndef TPARAMETERLIST_H_
#define TPARAMETERLIST_H_

//#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "NodeLibrary.h"
#include "TParameter.h"



class NODE_API TParameterList
{
private:
	TParameterList( const TParameterList& node) = delete;


public:
	TParameterList();
	~TParameterList();

	template<typename  T> void Append( const std::string& key, const T& value, bool visible, std::function<bool()> connectedFunction = 0 );

	// Testing
	bool Contains(const std::string& name) const;

	// Query Values
	tonatiuh_variant GetValue( const std::string& name ) const;
	std::string GetValueToString(const std::string& name) const;
	bool GetVisibility(const std::string& name) const;
	std::vector< std::string > GetParametersNames() const;

	int NumberOfParameters() const;

	bool RemoveParameter( const std::string& name );

	bool SetValue(const std::string& name, const tonatiuh_variant& value );
	bool SetValueFromString(const std::string& name, const std::string& value );


protected:
	std::map< std::string, std::unique_ptr< TParameter> > m_parametersList;
	std::map< std::string, bool > m_parametersVisibilityList;
};

template<class T> void TParameterList::Append( const std::string& key, const T& value, bool visible, std::function<bool()> connectedFunction )
{
	if( m_parametersList.find( key ) != m_parametersList.end() )
		return;

	TParameter newParameter;
	m_parametersList[key] = std::make_unique<TParameter>();
	m_parametersList[key]->SetValue( value );
	if( connectedFunction!= 0 )
		m_parametersList[key]->SetConnectedFuntion( connectedFunction );
	m_parametersVisibilityList[key] = visible;
}

#endif /* TPARAMETERLIST_H_ */
