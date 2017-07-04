/*
 * TParameterList.h
 *
 *  Created on: 07/09/2016
 *      Author: amutuberria
 */

#ifndef TPARAMETERLIST_H_
#define TPARAMETERLIST_H_

#include <QHash>
#include <QString>
#include <QVariant>


class TParameterList: public QObject
{
	Q_OBJECT

public:
	TParameterList();
	~TParameterList();

	void Append( const QString& key, const QVariant& value, bool visible = true );

	// Testing
	bool Contains(const QString& name) const;

	// Query Values
	QVariant GetValue(const QString& name) const;
	QVariant GetValue(const QString& name, const QVariant& defaultValue) const;
	bool GetVisibility(const QString& name) const;
	QStringList GetParametersNames() const;

	int NumberOfParameters() const;

	bool RemoveParameter( const QString& name );

	void SetValue(const QString& name, QVariant value );

protected:
	QHash< QString, QVariant > m_parametersList;
	QHash< QString, bool > m_parametersVisibilityList;
};

#endif /* TPARAMETERLIST_H_ */
