
#pragma once

#include <string>
#include <QPixmap>
#include "pluginContainer.h"
#include "persistentDataContainer.h"

class App;

class AppBase
{
public:
    AppBase(){}
    virtual ~AppBase(){}

    static void handleVerSpecConstructor1()
    {
        Q_INIT_RESOURCE(imageFiles_vrep);
    }

    static void handleVerSpecDestructor1()
    {
        Q_CLEANUP_RESOURCE(imageFiles_vrep);
    }

    static std::string handleVerSpecSetAppName1(const char* name)
    {
        std::string retName(name);
        return(retName);
    }

    static void handleVerSpecRunInitCallback1(bool toFile)
    {
        // Following is so that we can display the correct splash screen at start-up (at that time we haven't yet loaded the plugins!):
        CPersistentDataContainer a(FILENAME_OF_USER_SETTINGS_IN_BINARY_FILE);
        if (CPluginContainer::isMeshPluginAvailable())
            a.writeData("usesMajorPlugin__","yes",toFile);
        else
            a.writeData("usesMajorPlugin__","",toFile);
    }

    static void handleVerSpecRun1(){}
    static void handleVerSpecRun2(){}

    static void handleVerSpecShowSplash1(QPixmap& pixmap)
    {
        // At this stage the plugins are not yet loaded. So we verify if previous run used one of the major plugins that have a different license:
        CPersistentDataContainer temp(FILENAME_OF_USER_SETTINGS_IN_BINARY_FILE); // very unelegant, change this later!
        std::string dummyVal;
        if (temp.readData("usesMajorPlugin__",dummyVal))
            pixmap.load(":/imageFiles_vrep/splashEdu.png"); // you may change this splash screen only if you use the commercial version of the plugins!
        else
            pixmap.load(":/imageFiles_vrep/splashBasic.png");
    }

    static std::string handleVerSpecSetIcon1()
    {
        std::string retStr;
        #ifdef WIN_VREP
            retStr=":/variousImageFiles/v_repIcon16.png";
        #else
            retStr=":/imageFiles_vrep/v_repIcon128.png";
        #endif
        return(retStr);
    }
};
