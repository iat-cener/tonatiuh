/*
 * TParameterList.h
 *
 *  Created on: 07/09/2016
 *      Author: amutuberria
 */

#ifndef TPARAMETERLIST_H_
#define TPARAMETERLIST_H_

#include <map>
#include <string>

#include <QVariant>


class TParameterList
{
private:
	TParameterList( const TParameterList& node) = delete;
//	Q_OBJECT

public:
	TParameterList();
	~TParameterList();

	void Append( const std::string& key, const QVariant& value, bool visible = true );

	// Testing
	bool Contains(const std::string& name) const;

	// Query Values
	QVariant GetValue(const std::string& name) const;
	QVariant GetValue(const std::string& name, const QVariant& defaultValue) const;
	bool GetVisibility(const std::string& name) const;
	std::vector< std::string > GetParametersNames() const;

	int NumberOfParameters() const;

	bool RemoveParameter( const std::string& name );

	bool SetValue(const std::string& name, const QVariant& value );

protected:
	std::map< std::string, QVariant > m_parametersList;
	std::map< std::string, bool > m_parametersVisibilityList;
};

#endif /* TPARAMETERLIST_H_ */
