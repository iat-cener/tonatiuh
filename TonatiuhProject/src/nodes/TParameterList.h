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

class TParameterList;

class TParameterList: public QObject
{
	Q_OBJECT

public:
	TParameterList();
	~TParameterList();

	void Append(const TParameterList* list);
	void Append(const QHash<QString, QVariant>&  list );
	void Append(const QString& key, const QVariant& value);

	// Testing
	bool Contains(const QString& name) const;

	// Query Values
	QVariant Get(const QString& name) const;
	QVariant Get(const QString& name, const QVariant& defaultValue) const;
	QStringList GetParametersNames() const;

	int NumberOfParameters() const;

	void Set(const QString& name, QVariant value );

	//template<class T> T value(const QString& name) const;
	//template<class T> T value(const QString& name, const T& defaultValue) const;

protected:
	QHash<QString, QVariant> m_parametersList;
};

/*


template<class T>
T TParameterList::value(const QString& name) const
{
  QVariant val = get(name);
  return val.value<T>();
}


template<class T>
T TParameterList::value(const QString& name, const T& defaultValue) const
{
  QVariant val = get(name, QVariant::fromValue(defaultValue));
  return val.value<T>();
}
*/




#endif /* TPARAMETERLIST_H_ */
