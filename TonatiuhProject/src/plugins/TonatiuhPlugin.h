/*
 * TonatiuhPlugin.h
 *
 *  Created on: 17 oct. 2017
 *      Author: amaia
 */

#ifndef TONATIUHPLUGIN_H_
#define TONATIUHPLUGIN_H_

#include <string>

class TonatiuhPlugin
{

public:
	TonatiuhPlugin(){ };
    virtual ~TonatiuhPlugin() {};
    virtual std::string command(std::string command, std::string options) {return "";}
};


#define DEFINE_PLUGIN(classType, pluginName, pluginVersion)     \
  extern "C" {                                                  \
    std::shared_ptr<TonatiuhPlugin> load()                                         \
    {                                                           \
      return std::make_shared<classType>();                     \
    }                                                           \
                                                                \
    const char* name()                                          \
    {                                                           \
      return pluginName;                                        \
    }                                                           \
                                                                \
    const char* version()                                       \
    {                                                           \
      return pluginVersion;                                     \
    }                                                           \
  }



#endif /* TONATIUHPLUGIN_H_ */
