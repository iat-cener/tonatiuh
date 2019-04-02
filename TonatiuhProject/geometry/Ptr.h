/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/
 
#ifndef PTR_H_
#define PTR_H_

template <class T>  
class Ptr 
{
public:
    Ptr( T* realPointer = 0 );
    Ptr( const Ptr& rhs );
    ~Ptr();
    Ptr& operator=( const Ptr<T>& rhs );
    Ptr& operator=( const T*& realPointer );
    T* operator->() const;
    T& operator*() const;
    operator bool() const { return m_realPointer != NULL; }
	void ShowCount() const;
	
private:
	void Init();
	T* m_realPointer;
};

template <class T>
Ptr<T>::Ptr( T* realPointer )
: m_realPointer(realPointer)
{
	Init();
}

template <class T>
Ptr<T>::Ptr( const Ptr& rhs )
: m_realPointer(rhs.m_realPointer)
{
	Init();
}

template <class T>
void Ptr<T>::Init()
{
	if( m_realPointer ) m_realPointer->Upcount();
	return;
}

template <class T>
Ptr<T>::~Ptr()
{
	if( m_realPointer ) m_realPointer->Downcount(); 
}

template <class T>
Ptr<T>& Ptr<T>::operator=( const Ptr<T>& rhs )
{
	if( m_realPointer != rhs.m_realPointer )
	{
		T* oldRealPointer = m_realPointer;
		m_realPointer = rhs.m_realPointer;
		Init();
		if( oldRealPointer ) oldRealPointer->Downcount();
	}
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=( const T*& realPointer )
{
	if( m_realPointer != realPointer )
	{
		T* oldRealPointer = m_realPointer;
		m_realPointer = realPointer;
		Init();
		if( oldRealPointer ) oldRealPointer->Downcount();
	}
	return *this;
}

template <class T>
T* Ptr<T>::operator->() const
{
	return m_realPointer;
}

template <class T>
T& Ptr<T>::operator*() const
{
	return *m_realPointer;
}

template <class T>
void Ptr<T>::ShowCount() const
{ 
	m_realPointer->PrintCount();
}

#endif /*PTR_H_*/
