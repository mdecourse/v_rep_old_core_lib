
#pragma once

#include "mainCont.h"
#include "luaScriptObject.h"
#include "broadcastDataContainer.h"
#include "v_rep_internal.h"

class CLuaScriptContainer : public CMainCont 
{
public:
    CLuaScriptContainer();
    virtual ~CLuaScriptContainer();
    void simulationAboutToStart();
    void simulationEnded();
    void simulationAboutToEnd();
    void renderYour3DStuff(CViewableBase* renderingObject,int displayAttrib);
    void removeAllScripts();
    void announceObjectWillBeErased(int objectID);
    bool removeScript_safe(int scriptId);
    bool removeScript(int scriptID);
    int insertScript(CLuaScriptObject* script);
    CLuaScriptObject* getScriptFromID_alsoAddOnsAndSandbox(int scriptID) const;
    CLuaScriptObject* getScriptFromID_noAddOnsNorSandbox(int scriptID) const;
    CLuaScriptObject* getMainScript() const;
    CLuaScriptObject* getScriptFromObjectAttachedTo_child(int threeDObjectID) const;
    CLuaScriptObject* getScriptFromObjectAttachedTo_customization(int threeDObjectID) const;

    int getScriptsFromObjectAttachedTo(int threeDObjectID,std::vector<CLuaScriptObject*>& scripts) const;
    void killAllSimulationLuaStates();
    int insertDefaultScript_mainAndChildScriptsOnly(int scriptType,bool threaded);
    void setInMainScriptNow(bool launched,int startTimeInMs);
    bool getInMainScriptNow() const;
    int getMainScriptExecTimeInMs() const;
    void checkIfContactCallbackFunctionAvailable();
    int getObjectIdContactCallbackFunctionAvailable() const;
    void checkAvailableDynCallbackFunctions();
    const std::vector<int>* getObjectIdsWhereDynCallbackFunctionsAvailable() const;

    int removeDestroyedScripts(int scriptType);

    void addCallbackStructureObjectToDestroyAtTheEndOfSimulation_new(SScriptCallBack* object);
    void addCallbackStructureObjectToDestroyAtTheEndOfSimulation_old(SLuaCallBack* object);
    bool addCommandToOutsideCommandQueues(int commandID,int auxVal1,int auxVal2,int auxVal3,int auxVal4,const float aux2Vals[8],int aux2Count);

    int handleCustomizationScriptExecution(int callType,CInterfaceStack* inStack,CInterfaceStack* outStack);
    bool handleCustomizationScriptExecution_beforeMainScript();
    void handleNonThreadedChildScript_specialCall(int callType,CInterfaceStack* inStack,CInterfaceStack* outStack);
    void callAddOnMainChildCustomizationWithData(int callType,CInterfaceStack* inStack);
    void sceneOrModelAboutToBeSaved(int modelBase);

    CLuaScriptObject* getScriptFromObjectAttachedTo_jointCallback_OLD(int threeDObjectID) const;
    CLuaScriptObject* getCustomContactHandlingScript_callback_OLD() const;
    CLuaScriptObject* getGeneralCallbackHandlingScript_callback_OLD() const;

    std::vector<CLuaScriptObject*> allScripts;

    bool hasSceneIncludeScripts() const;
    bool hasModelIncludeScripts(int modelBase) const;

    int getScriptSimulationParameter_mainAndChildScriptsOnly(int scriptHandle,const char* parameterName,std::string& parameterValue) const;
    int setScriptSimulationParameter_mainAndChildScriptsOnly(int scriptHandle,const char* parameterName,const char* parameterValue,int parameterValueLength);

    CBroadcastDataContainer broadcastDataContainer;

    static bool getSaveIncludeScriptFiles();
    static void setSaveIncludeScriptFiles(bool save);

protected:
    bool _inMainScriptNow;
    int _mainScriptStartTimeInMs;
    int _objectIdContactCallbackFunctionAvailable;
    std::vector<int> _objectIdsWhereDynCallbackFunctionsAvailable;

    std::vector<SScriptCallBack*> _callbackStructureToDestroyAtEndOfSimulation_new;
    std::vector<SLuaCallBack*> _callbackStructureToDestroyAtEndOfSimulation_old;
    static bool _saveIncludeScriptFiles;
};
