
#include "vrepMainHeader.h"
#include "v_rep_internal.h"
#include "luaScriptContainer.h"
#include "tt.h"
#include "vVarious.h"
#include "threadPool.h"
#include "vFileFinder.h"
#include "v_repStrings.h"
#include "app.h"
#include "vDateTime.h"
#include "funcDebug.h"

bool CLuaScriptContainer::_saveIncludeScriptFiles=false;

bool CLuaScriptContainer::getSaveIncludeScriptFiles()
{
   return(_saveIncludeScriptFiles);
}

void CLuaScriptContainer::setSaveIncludeScriptFiles(bool save)
{
    _saveIncludeScriptFiles=save;
}

CLuaScriptContainer::CLuaScriptContainer()
{
    _inMainScriptNow=false;
    insertDefaultScript_mainAndChildScriptsOnly(sim_scripttype_mainscript,false);
}

CLuaScriptContainer::~CLuaScriptContainer()
{
    removeAllScripts();
    for (size_t i=0;i<_callbackStructureToDestroyAtEndOfSimulation_new.size();i++)
        delete _callbackStructureToDestroyAtEndOfSimulation_new[i];
    for (size_t i=0;i<_callbackStructureToDestroyAtEndOfSimulation_old.size();i++)
        delete _callbackStructureToDestroyAtEndOfSimulation_old[i];
    broadcastDataContainer.removeAllObjects();
}

bool CLuaScriptContainer::hasSceneIncludeScripts() const
{
    for (size_t i=0;i<allScripts.size();i++)
    {
        CLuaScriptObject* it=allScripts[i];
        if (it->isSceneScript()&&(it->getIncludeScriptFilePathAndName().length()>0))
            return(true);
    }
    return(false);
}

bool CLuaScriptContainer::hasModelIncludeScripts(int modelBase) const
{
    C3DObject* modelBaseObject=App::ct->objCont->getObject(modelBase);
    std::vector<C3DObject*> objects;
    modelBaseObject->getAllObjectsRecursive(&objects,true,true);
    for (size_t i=0;i<objects.size();i++)
    {
        std::vector<CLuaScriptObject*> scripts;
        getScriptsFromObjectAttachedTo(objects[i]->getID(),scripts);
        for (size_t j=0;j<scripts.size();j++)
        {
            if (scripts[j]->getIncludeScriptFilePathAndName().length()>0)
                return(true);
        }
    }
    return(false);
}

void CLuaScriptContainer::simulationAboutToStart()
{
    broadcastDataContainer.simulationAboutToStart();
    for (size_t i=0;i<allScripts.size();i++)
        allScripts[i]->simulationAboutToStart();
}

void CLuaScriptContainer::simulationEnded()
{
    for (size_t i=0;i<allScripts.size();i++)
        allScripts[i]->simulationEnded();

    broadcastDataContainer.simulationEnded();
    removeDestroyedScripts(sim_scripttype_mainscript);
    removeDestroyedScripts(sim_scripttype_childscript);
    removeDestroyedScripts(sim_scripttype_jointctrlcallback);
    removeDestroyedScripts(sim_scripttype_contactcallback);
    for (size_t i=0;i<_callbackStructureToDestroyAtEndOfSimulation_new.size();i++)
        delete _callbackStructureToDestroyAtEndOfSimulation_new[i];
    _callbackStructureToDestroyAtEndOfSimulation_new.clear();
    for (size_t i=0;i<_callbackStructureToDestroyAtEndOfSimulation_old.size();i++)
        delete _callbackStructureToDestroyAtEndOfSimulation_old[i];
    _callbackStructureToDestroyAtEndOfSimulation_old.clear();
//  if (_initialValuesInitialized&&App::ct->simulation->getResetSceneAtSimulationEnd())
//  {
//  }
}

void CLuaScriptContainer::simulationAboutToEnd()
{
    for (size_t i=0;i<allScripts.size();i++)
        allScripts[i]->simulationAboutToEnd();
}

void CLuaScriptContainer::renderYour3DStuff(CViewableBase* renderingObject,int displayAttrib)
{

}

void CLuaScriptContainer::addCallbackStructureObjectToDestroyAtTheEndOfSimulation_new(SScriptCallBack* object)
{
    _callbackStructureToDestroyAtEndOfSimulation_new.push_back(object);
}
void CLuaScriptContainer::addCallbackStructureObjectToDestroyAtTheEndOfSimulation_old(SLuaCallBack* object)
{
    _callbackStructureToDestroyAtEndOfSimulation_old.push_back(object);
}
int CLuaScriptContainer::removeDestroyedScripts(int scriptType)
{
    FUNCTION_DEBUG;
    int retVal=0;
    for (int i=0;i<int(allScripts.size());i++)
    {
        if ( (allScripts[i]->getScriptType()==scriptType)&&allScripts[i]->getFlaggedForDestruction() )
        {
            if ( (!allScripts[i]->getThreadedExecution())||(!allScripts[i]->getThreadedExecutionIsUnderWay()) )
            {
                retVal++;
                CLuaScriptObject* it=allScripts[i];
                it->killLuaState(); // should not be done in the destructor!
                allScripts.erase(allScripts.begin()+i);
                i--;
                delete it;
            }
        }
    }
    return(retVal);
}

void CLuaScriptContainer::setInMainScriptNow(bool launched,int startTimeInMs)
{
    _inMainScriptNow=launched;
    _mainScriptStartTimeInMs=startTimeInMs;
}

bool CLuaScriptContainer::getInMainScriptNow() const
{
    return(_inMainScriptNow);
}

int CLuaScriptContainer::getMainScriptExecTimeInMs() const
{
    return(VDateTime::getTimeDiffInMs(_mainScriptStartTimeInMs));
}

void CLuaScriptContainer::checkIfContactCallbackFunctionAvailable()
{
    _objectIdContactCallbackFunctionAvailable=-1;
    for (size_t i=0;i<allScripts.size();i++)
    { // we prioritize child scripts:
        CLuaScriptObject* it=allScripts[i];
        if ( (it->getScriptType()==sim_scripttype_childscript)&&it->getContainsContactCallbackFunction()&&(!it->getThreadedExecution()))
            _objectIdContactCallbackFunctionAvailable=it->getObjectIDThatScriptIsAttachedTo_child();
    }
    if (_objectIdContactCallbackFunctionAvailable==-1)
    { // then we check customization scripts:
        for (size_t i=0;i<allScripts.size();i++)
        {
            CLuaScriptObject* it=allScripts[i];
            if ( (it->getScriptType()==sim_scripttype_customizationscript)&&it->getContainsContactCallbackFunction())
                _objectIdContactCallbackFunctionAvailable=it->getObjectIDThatScriptIsAttachedTo_customization();
        }
    }
}

void CLuaScriptContainer::checkAvailableDynCallbackFunctions()
{
    _objectIdsWhereDynCallbackFunctionsAvailable.clear();
    for (size_t i=0;i<App::ct->objCont->objectList.size();i++)
    {
        int objId=App::ct->objCont->objectList[i];
        bool addIt=false;
        CLuaScriptObject* it=getScriptFromObjectAttachedTo_child(objId);
        addIt=( (it!=NULL)&&it->getContainsDynCallbackFunction()&&(!it->getThreadedExecution()) );
        if (!addIt)
        {
            it=getScriptFromObjectAttachedTo_customization(objId);
            addIt=( (it!=NULL)&&it->getContainsDynCallbackFunction() );
        }
        if (addIt)
            _objectIdsWhereDynCallbackFunctionsAvailable.push_back(objId);
    }
}

int CLuaScriptContainer::getObjectIdContactCallbackFunctionAvailable() const
{
    return(_objectIdContactCallbackFunctionAvailable);
}

const std::vector<int>* CLuaScriptContainer::getObjectIdsWhereDynCallbackFunctionsAvailable() const
{
    return(&_objectIdsWhereDynCallbackFunctionsAvailable);
}

void CLuaScriptContainer::removeAllScripts()
{
    FUNCTION_DEBUG;
    while (allScripts.size()>0)
    {
        CLuaScriptObject* it=allScripts[0];
        it->killLuaState(); // should not be done in the destructor!
        allScripts.erase(allScripts.begin());
        delete it;
    }
}

void CLuaScriptContainer::killAllSimulationLuaStates()
{
    for (size_t i=0;i<allScripts.size();i++)
    {
        if ((allScripts[i]->getScriptType()==sim_scripttype_mainscript)||(allScripts[i]->getScriptType()==sim_scripttype_childscript)||(allScripts[i]->getScriptType()==sim_scripttype_jointctrlcallback)||(allScripts[i]->getScriptType()==sim_scripttype_contactcallback)) //||(allScripts[i]->getScriptType()==sim_scripttype_generalcallback)) // not the customization scripts!!||(allScripts[i]->getScriptType()==sim_scripttype_customizationscript))
            allScripts[i]->killLuaState();
    }
}

void CLuaScriptContainer::announceObjectWillBeErased(int objectID)
{ // Never called from copy buffer!
    size_t i=0;
    while (i<allScripts.size())
    {
        if (allScripts[i]->announce3DObjectWillBeErased(objectID,false))
        {
            if (removeScript(allScripts[i]->getScriptID()))
                i=0; // ordering may have changed
            else
                i++;
        }
        else
            i++;
    }
}

bool CLuaScriptContainer::removeScript_safe(int scriptId)
{ // removal may happen in a delayed fashion
    CLuaScriptObject* it=getScriptFromID_noAddOnsNorSandbox(scriptId);
    if (it==NULL)
        return(false);
    int res=it->flagScriptForRemoval();
    if (res==0)
        return(false);
    if (res==2)
        removeScript(scriptId);
    return(true);
}

bool CLuaScriptContainer::removeScript(int scriptID)
{
    FUNCTION_DEBUG;
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getScriptID()==scriptID)
        {
            CLuaScriptObject* it=allScripts[i];
            it->killLuaState(); // should not be done in the destructor!
            allScripts.erase(allScripts.begin()+i);
            delete it;
            App::ct->setModificationFlag(16384);
            break;
        }
    }
    App::setFullDialogRefreshFlag();
    return(true);
}

CLuaScriptObject* CLuaScriptContainer::getScriptFromID_alsoAddOnsAndSandbox(int scriptID) const
{
    CLuaScriptObject* retVal=App::ct->addOnScriptContainer->getAddOnScriptFromID(scriptID);
    if (retVal==NULL)
    {
        if (retVal==NULL)
            retVal=getScriptFromID_noAddOnsNorSandbox(scriptID);
        if ( (retVal==NULL)&&(App::ct->sandboxScript!=NULL)&&(App::ct->sandboxScript->getScriptID()==scriptID) )
            retVal=App::ct->sandboxScript;

    }
    return(retVal);
}

CLuaScriptObject* CLuaScriptContainer::getScriptFromID_noAddOnsNorSandbox(int scriptID) const
{
    CLuaScriptObject* retVal=NULL;
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getScriptID()==scriptID)
        {
            retVal=allScripts[i];
            break;
        }
    }
    return(retVal);
}

CLuaScriptObject* CLuaScriptContainer::getScriptFromObjectAttachedTo_child(int threeDObjectID) const
{ // used for child scripts
    if (threeDObjectID<0)
        return(NULL); // 10/1/2016
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getObjectIDThatScriptIsAttachedTo_child()==threeDObjectID)
            return(allScripts[i]);
    }
    return(NULL);
}

CLuaScriptObject* CLuaScriptContainer::getScriptFromObjectAttachedTo_customization(int threeDObjectID) const
{ // used for customization scripts
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getObjectIDThatScriptIsAttachedTo_customization()==threeDObjectID)
            return(allScripts[i]);
    }
    return(NULL);
}

int CLuaScriptContainer::getScriptsFromObjectAttachedTo(int threeDObjectID,std::vector<CLuaScriptObject*>& scripts) const
{
    scripts.clear();
    CLuaScriptObject* it=getScriptFromObjectAttachedTo_child(threeDObjectID);
    if (it!=NULL)
        scripts.push_back(it);
    it=getScriptFromObjectAttachedTo_jointCallback_OLD(threeDObjectID);
    if (it!=NULL)
        scripts.push_back(it);
    it=getScriptFromObjectAttachedTo_customization(threeDObjectID);
    if (it!=NULL)
        scripts.push_back(it);
    return(int(scripts.size()));
}

CLuaScriptObject* CLuaScriptContainer::getMainScript() const
{
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getScriptType()==sim_scripttype_mainscript)
            return(allScripts[i]);
    }
    return(NULL);
}

int CLuaScriptContainer::insertScript(CLuaScriptObject* script)
{
    // We make sure the id is unique:
    int newID=SIM_IDSTART_LUASCRIPT;
    while (getScriptFromID_noAddOnsNorSandbox(newID)!=NULL)
        newID++;
    script->setScriptID(newID);
    allScripts.push_back(script);
    App::ct->setModificationFlag(8192);
    return(newID);
}

int CLuaScriptContainer::insertDefaultScript_mainAndChildScriptsOnly(int scriptType,bool threaded)
{ 
    if (scriptType!=sim_scripttype_childscript)
        threaded=false; // just to make sure
    int retVal=-1;
    std::string filenameAndPath(App::directories->systemDirectory+VREP_SLASH);

    if (scriptType==sim_scripttype_mainscript)
        filenameAndPath+=DEFAULT_MAINSCRIPT_NAME;
    if (scriptType==sim_scripttype_childscript)
    {
        if (threaded)
            filenameAndPath+=DEFAULT_THREADEDCHILDSCRIPT_NAME;
        else
            filenameAndPath+=DEFAULT_NONTHREADEDCHILDSCRIPT_NAME;
    }

    if (VFile::doesFileExist(filenameAndPath))
    {
        try
        {
            VFile file(filenameAndPath,VFile::READ|VFile::SHARE_DENY_NONE);
            VArchive archive(&file,VArchive::LOAD);
            unsigned int archiveLength=(unsigned int)file.getLength();
            char* defaultScript=new char[archiveLength+1];
            for (int i=0;i<int(archiveLength);i++)
                archive >> defaultScript[i];
            defaultScript[archiveLength]=0;
            CLuaScriptObject* defScript=new CLuaScriptObject(scriptType);
            retVal=insertScript(defScript);
            defScript->setScriptText(defaultScript,NULL);
            defScript->setThreadedExecution(threaded);
            if (threaded)
                defScript->setExecuteJustOnce(true);
            delete[] defaultScript;     
            archive.close();
            file.close();
        }
        catch(VFILE_EXCEPTION_TYPE e)
        {
            VFile::reportAndHandleFileExceptionError(e);
            char defaultMessage[]="Default script file could not be found!"; // do not use comments ("--"), we want to cause an execution error!
            CLuaScriptObject* defScript=new CLuaScriptObject(scriptType);
            retVal=insertScript(defScript);
            defScript->setScriptText(defaultMessage,NULL);
            defScript->setThreadedExecution(threaded);
        }
    }
    else
    {
        char defaultMessage[]="Default script file could not be found!"; // do not use comments ("--"), we want to cause an execution error!
        CLuaScriptObject* defScript=new CLuaScriptObject(scriptType);
        retVal=insertScript(defScript);
        defScript->setScriptText(defaultMessage,NULL);
        defScript->setThreadedExecution(threaded);
    }
    App::setLightDialogRefreshFlag();
    return(retVal);
}

void CLuaScriptContainer::callAddOnMainChildCustomizationWithData(int callType,CInterfaceStack* inStack)
{
    FUNCTION_DEBUG;
    // 1. Add-ons
    App::ct->addOnScriptContainer->callAddOnWithData(callType,inStack);
    CLuaScriptObject* script=NULL;
    if (!App::ct->simulation->isSimulationStopped())
    {
        // 2. Main script:
        script=getMainScript();
        if (script!=NULL)
            script->runMainScript(callType,inStack,NULL);
        // 3. Non-threaded child scripts:
        handleNonThreadedChildScript_specialCall(callType,inStack,NULL);
    }
    // 4. Customization scripts:
    handleCustomizationScriptExecution(callType,inStack,NULL);
}

void CLuaScriptContainer::sceneOrModelAboutToBeSaved(int modelBase)
{
    C3DObject* obj=App::ct->objCont->getObject(modelBase);
    if (obj!=NULL)
    {
        std::vector<C3DObject*> toExplore;
        toExplore.push_back(obj);
        while (toExplore.size()!=0)
        {
            obj=toExplore[toExplore.size()-1];
            toExplore.pop_back();
            CLuaScriptObject* it=getScriptFromObjectAttachedTo_customization(obj->getID());
            if (it!=NULL)
            {
                if (it->getCustomizationScriptCleanupBeforeSave())
                    it->killLuaState();
            }
            for (size_t i=0;i<obj->childList.size();i++)
                toExplore.push_back(obj->childList[i]);
        }
    }
    else
    {
        for (size_t i=0;i<allScripts.size();i++)
        {
            CLuaScriptObject* it=allScripts[i];
            if (it->getScriptType()==sim_scripttype_customizationscript)
            {
                if (it->getCustomizationScriptCleanupBeforeSave())
                    it->killLuaState();
            }
        }
    }
}
void CLuaScriptContainer::handleNonThreadedChildScript_specialCall(int callType,CInterfaceStack* inStack,CInterfaceStack* outStack)
{
    // Do 3 loops, for the first, normal and last execution order.
    // We always go through the whole scripts, since some scripts might be destroyed
    // on the way:
    for (int priority=sim_scriptexecorder_first;priority<=sim_scriptexecorder_last;priority++)
    {
        for (size_t i=0;i<allScripts.size();i++)
        {
            CLuaScriptObject* it=allScripts[i];
            if ( (it->getScriptType()==sim_scripttype_childscript)&&(it->getExecutionOrder()==priority)&&(!it->getThreadedExecution()) )
            {
                C3DObject* obj=App::ct->objCont->getObject(it->getObjectIDThatScriptIsAttachedTo_child());
                if (obj!=NULL) // we could still run it in that situation, but is not desired, since the script itself will shortly be destroyed.. or is unattached!
                {
                    if (it->runNonThreadedChildScript(callType,inStack,outStack))
                    {
                        if (outStack!=NULL)
                            return;
                    }
                }
            }
        }
    }
}

bool CLuaScriptContainer::handleCustomizationScriptExecution_beforeMainScript()
{ // return true: run main script, otherwise, do not run main script
    bool retVal=true;

    // Do 3 loops, for the first, normal and last execution order.
    // We always go through the whole scripts, since some scripts might be destroyed
    // on the way:
    for (int priority=sim_scriptexecorder_first;priority<=sim_scriptexecorder_last;priority++)
    {
        for (size_t i=0;i<allScripts.size();i++)
        {
            CLuaScriptObject* it=allScripts[i];
            if ( (it->getScriptType()==sim_scripttype_customizationscript)&&(it->getExecutionOrder()==priority) )
            {
                C3DObject* obj=App::ct->objCont->getObject(it->getObjectIDThatScriptIsAttachedTo_customization());
                if (obj!=NULL) // we could still run it in that situation, but is not desired, since the script itself will shortly be destroyed.. or is unattached!
                {
                    CInterfaceStack outStack;
                    if (it->runCustomizationScript(sim_syscb_beforemainscript,NULL,&outStack))
                    {
                        bool doNotRunMainScript;
                        if (outStack.getStackMapBoolValue("doNotRunMainScript",doNotRunMainScript))
                        {
                            if (doNotRunMainScript)
                                retVal=false;
                        }
                    }
                }
            }
        }
    }
    return(retVal);
}


int CLuaScriptContainer::handleCustomizationScriptExecution(int callType,CInterfaceStack* inStack,CInterfaceStack* outStack)
{ // returns the number of different customization scripts executed
    int retVal=0;

    // Do 3 loops, for the first, normal and last execution order.
    // We always go through the whole scripts, since some scripts might be destroyed
    // on the way:
    bool breakOut=false;
    for (int priority=sim_scriptexecorder_first;priority<=sim_scriptexecorder_last;priority++)
    {
        for (size_t i=0;i<allScripts.size();i++)
        {
            CLuaScriptObject* it=allScripts[i];
            if ( (it->getScriptType()==sim_scripttype_customizationscript)&&(it->getExecutionOrder()==priority) )
            {
                C3DObject* obj=App::ct->objCont->getObject(it->getObjectIDThatScriptIsAttachedTo_customization());
                if (obj!=NULL) // we could still run it in that situation, but is not desired, since the script itself will shortly be destroyed.. or is unattached!
                {
                    if (it->runCustomizationScript(callType,inStack,outStack))
                    {
                        retVal++;
                        if (outStack!=NULL)
                        {
                            breakOut=true;
                            break;
                        }
                    }
                }
            }
        }
        if (breakOut)
            break;
    }
    return(retVal);
}

int CLuaScriptContainer::getScriptSimulationParameter_mainAndChildScriptsOnly(int scriptHandle,const char* parameterName,std::string& parameterValue) const
{
    int retVal=0;
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (!allScripts[i]->getFlaggedForDestruction())
        {
            if ( (scriptHandle==allScripts[i]->getScriptID())||
                (scriptHandle==sim_handle_all)||
                ( (scriptHandle==sim_handle_main_script)&&(allScripts[i]->getScriptType()==sim_scripttype_mainscript) ) )
            {
                if ((allScripts[i]->getScriptType()==sim_scripttype_mainscript)||(allScripts[i]->getScriptType()==sim_scripttype_childscript))
                { // We only access main script and child script1
                    bool res=allScripts[i]->getScriptParametersObject()->getParameterValue(parameterName,parameterValue);
                    if (res)
                        retVal++;
                }
            }
        }
    }
    return(retVal);
}

int CLuaScriptContainer::setScriptSimulationParameter_mainAndChildScriptsOnly(int scriptHandle,const char* parameterName,const char* parameterValue,int parameterValueLength)
{
    int retVal=0;
    for (int i=0;i<int(allScripts.size());i++)
    {
        if (!allScripts[i]->getFlaggedForDestruction())
        {
            if ( (scriptHandle==allScripts[i]->getScriptID())||
                (scriptHandle==sim_handle_all)||
                ( (scriptHandle==sim_handle_main_script)&&(allScripts[i]->getScriptType()==sim_scripttype_mainscript) ) )
            {
                if ((allScripts[i]->getScriptType()==sim_scripttype_mainscript)||(allScripts[i]->getScriptType()==sim_scripttype_childscript))
                { // We only access main script and child script1
                    bool res=allScripts[i]->getScriptParametersObject()->setParameterValue(parameterName,parameterValue,parameterValueLength);
                    if (res)
                        retVal++;
                }
            }
        }
    }
    return(retVal);
}

bool CLuaScriptContainer::addCommandToOutsideCommandQueues(int commandID,int auxVal1,int auxVal2,int auxVal3,int auxVal4,const float aux2Vals[8],int aux2Count)
{
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (!allScripts[i]->getFlaggedForDestruction())
            allScripts[i]->addCommandToOutsideCommandQueue(commandID,auxVal1,auxVal2,auxVal3,auxVal4,aux2Vals,aux2Count);
    }
    return(true);
}

CLuaScriptObject* CLuaScriptContainer::getScriptFromObjectAttachedTo_jointCallback_OLD(int threeDObjectID) const
{ // used for callback scripts
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getObjectIDThatScriptIsAttachedTo_callback_OLD()==threeDObjectID)
            return(allScripts[i]);
    }
    return(NULL);
}

CLuaScriptObject* CLuaScriptContainer::getCustomContactHandlingScript_callback_OLD() const
{
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getScriptType()==sim_scripttype_contactcallback)
            return(allScripts[i]);
    }
    return(NULL);
}

CLuaScriptObject* CLuaScriptContainer::getGeneralCallbackHandlingScript_callback_OLD() const
{
    for (size_t i=0;i<allScripts.size();i++)
    {
        if (allScripts[i]->getScriptType()==sim_scripttype_generalcallback)
            return(allScripts[i]);
    }
    return(NULL);
}
