/*
 * TonatiuhPlugin.h
 *
 *  Created on: 17 oct. 2017
 *      Author: amaia
 */

#ifndef TONATIUHPLUGIN_H_
#define TONATIUHPLUGIN_H_

#include <string>
#include <iostream>

#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h> //dlopen
#endif


#if defined(  WIN32 )  && defined(_MSC_VER)
	#define TONATIUH_PLUGIN_EXPORT __declspec(dllexport)
#else
	#define TONATIUH_PLUGIN_EXPORT
#endif



#if defined(  WIN32 )  && defined(_MSC_VER)
	#ifdef TONATIUH_PLUGIN_SHARED_LIBRARY
		#define TONATIUH_PLUGIN_API __declspec(dllexport)
	#else
		#define TONATIUH_PLUGIN_API __declspec(dllimport)
	#endif
#else
	#define TONATIUH_PLUGIN_API
#endif


class LibraryHandle;

class TONATIUH_PLUGIN_API TonatiuhPlugin
{

public:
	TonatiuhPlugin()
	:m_pHandle ( nullptr )
	{

	};
    virtual ~TonatiuhPlugin()
    {

    };

    void SetLibraryHandle( std::shared_ptr< LibraryHandle >& handle )
    {
    	m_pHandle = handle;
    }

private:
	std::shared_ptr< LibraryHandle >  m_pHandle;

};



struct TonatiuhPluginDetails
{
	~TonatiuhPluginDetails()
	{
	}

	const char* className;
	const char* pluginType;
	const char* pluginVersion;

};

#define DEFINE_PLUGIN(classType, pluginType, pluginVersion)     			\
	extern "C" {															\
		TONATIUH_PLUGIN_EXPORT TonatiuhPluginDetails Exports() {			\
			return TonatiuhPluginDetails{									\
				#classType,													\
				#pluginType,												\
				pluginVersion,												\
			};																\
		}        															\
		TONATIUH_PLUGIN_EXPORT std::unique_ptr< classType > Instance() { 	\
			return std::make_unique<classType> ();							\
		}																	\
	}


//!  LibraryHandle class stores a handle to a shared library.
/*!
*  LibraryHandle class manages a handle to shared library. This handle could be stored in a plugin loader or in the plugin if the loader is deleted.
*
*/
struct LibraryHandle
{
	/*!
	 * Creates a handle object
	 */
	LibraryHandle( void* handle ):
		m_pHandle( handle )
	{

	}
	/*!
	 * Destroys the handle
	 */
	~LibraryHandle()
	{
		if( m_pHandle != nullptr )
		{
#if _WIN32
			FreeLibrary( (HMODULE) m_pHandle );
			m_pHandle = nullptr;
#else
			dlclose( m_pHandle );
#endif
		}
	};

	/*!
	 * Returns a raw pointer to the handle
	 */
	void* GetHandle()
	{
		return ( m_pHandle );
	};

private:
	void* m_pHandle{ nullptr };

};



//!  PluginLoader class loads a plugin for Tonatiuh from a file .
/*!
*  PluginLoader class loads a plugin in run time. This class add the functionality of loading a shared plugin in a platform-independent way.
*
*/
class PluginLoader
{

public:

	/*!
	 * Creates a plugin loader for the file \a filename.
	 */
	PluginLoader( const std::string& filename )
	{
#if _WIN32
		const size_t size = strlen( filename.c_str() ) + 1;
		wchar_t* wText = new wchar_t[size];
		mbstowcs(wText, filename.c_str(), size);
		m_pHandle = std::make_shared< LibraryHandle > ( LoadLibrary( wText ) );
#else
		m_pHandle = std::make_shared< LibraryHandle > ( dlopen( filename.c_str(), RTLD_LAZY ) );
#endif
		if ( m_pHandle == nullptr )
			PrintError();
	};
	~PluginLoader()
	{
		m_pHandle  = nullptr;
	};

private:

	/*!
	 * Prints the error.
	 */
	void PrintError()
	{
#if _WIN32
		std::cerr << GetLastError() << std::endl;
#else
		std::cerr << dlerror() << std::endl;
#endif
	}

public:
	/*!
	 * Loads a plugin from \a filename file.
	 * If the file contains a valid plug�n the details of the plugin are returned. Otherwise, a nullptr is returned.
	 */
	std::unique_ptr< TonatiuhPluginDetails > Details (  )
	{
		if( m_pHandle == nullptr )
			return ( nullptr );

		std::unique_ptr<TonatiuhPluginDetails> tonatiuhPluginDetails = nullptr;

		TonatiuhPluginDetails (*fCreate )() = nullptr;
#if _WIN32
		fCreate  = (TonatiuhPluginDetails (*)()) GetProcAddress( (HMODULE) m_pHandle->GetHandle(), "Exports" );
#else
		fCreate =  (TonatiuhPluginDetails (*)()) dlsym( m_pHandle->GetHandle(), "Exports" );
#endif

		if( !fCreate || fCreate == nullptr)
		{
			//PrintError();
			return ( tonatiuhPluginDetails );
		}

		tonatiuhPluginDetails = std::make_unique< TonatiuhPluginDetails> ( fCreate() );
		return ( tonatiuhPluginDetails );
	}


	/*!
	 * Creates an instance of the plugin and returns a pointer to instance. If the creations fails a null pointer is returned
	 */
	std::unique_ptr< TonatiuhPlugin > Instance()
	{
		if( m_pHandle == nullptr )
			return ( nullptr );

		std::unique_ptr< TonatiuhPlugin > (*fInstance )() = nullptr;
#if _WIN32
		fInstance  = (std::unique_ptr< TonatiuhPlugin >(*)() ) GetProcAddress( (HMODULE) m_pHandle->GetHandle(), "Instance" );
#else
		fInstance =  (std::unique_ptr< TonatiuhPlugin >(*)() ) dlsym( m_pHandle->GetHandle(), "Instance" );
#endif

		//return ( fInstance() );
		std::unique_ptr< TonatiuhPlugin > pluginInstance = fInstance();
		pluginInstance->SetLibraryHandle( m_pHandle );
		return ( pluginInstance );
	}

private:
	std::shared_ptr< LibraryHandle >  m_pHandle; // handle to library

};



#endif /* TONATIUHPLUGIN_H_ */
