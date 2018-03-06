
#pragma once

#include "luaWrapper.h"
#include "v_repTypes.h"
#include "luaScriptParameters.h"
#include "outsideCommandQueueForScript.h"
#include "vMutex.h"
#include "vThread.h"
#include "customData.h"
#include "interfaceStack.h"
#include "luaScriptObjectBase.h"

#define DEFAULT_MAINSCRIPT_NAME "dltmscpt.txt"
#define DEFAULT_NONTHREADEDCHILDSCRIPT_NAME "dltcscpt.txt"
#define DEFAULT_THREADEDCHILDSCRIPT_NAME "dlttscpt.txt"
#define DEFAULT_CUSTOMIZATIONSCRIPT_NAME "defaultCustomizationScript.txt"

class CLuaScriptObject: public CLuaScriptObjectBase
{
public:
    CLuaScriptObject(int scriptTypeOrMinusOneForSerialization);
    virtual ~CLuaScriptObject();

    void initializeInitialValues(bool simulationIsRunning);
    void simulationAboutToStart();
    void simulationAboutToEnd();
    void simulationEnded();

    int getScriptID() const;
    void setScriptID(int newID);
    bool isSceneScript() const;
    std::string getIncludeScriptFilePathAndName() const;

    std::string getDescriptiveName() const;
    std::string getShortDescriptiveName() const;
    std::string getScriptSuffixNumberString() const;
    std::string getScriptPseudoName() const;
    void setAddOnName(const char* name);
    std::string getAddOnName() const;

    CLuaScriptObject* copyYourself();
    void serialize(CSer& ar);
    void perform3DObjectLoadingMapping(std::vector<int>* map);
    bool announce3DObjectWillBeErased(int objectID,bool copyBuffer);
    int flagScriptForRemoval();
    int getObjectIDThatScriptIsAttachedTo_child() const; // for child scripts
    void setObjectIDThatScriptIsAttachedTo_child(int newObjectID); // for child scripts
    int getObjectIDThatScriptIsAttachedTo_customization() const; // for customization scripts
    void setObjectIDThatScriptIsAttachedTo_customization(int newObjectID); // for customization scripts


    void setScriptText(const char* scriptTxt,const std::vector<int>* scriptFoldingInfo);
    const char* getScriptText(std::vector<int>* scriptFoldingInfo);

    int runMainScript(int optionalCallType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    int runNonThreadedChildScript(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    int runThreadedChildScript();
    bool runCustomizationScript(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    void runAddOn(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);

    int callScriptFunction(const char* functionName, SLuaCallBack* pdata);
    int callScriptFunctionEx(const char* functionName,CInterfaceStack* stack);
    int setScriptVariable(const char* variableName,CInterfaceStack* stack);
    int clearVariable(const char* variableName);
    int executeScriptString(const char* scriptString,CInterfaceStack* stack);
    int appendTableEntry(const char* arrayName,const char* keyName,const char* data,const int what[2]); // deprecated

    void _displayScriptError(const char* errMsg,int errorType);

    void killLuaState();
    bool hasLuaState() const;
    int getNumberOfPasses() const;
    void setNumberOfPasses(int p);
    int setUserData(char* data);
    char* getUserData(int id) const;
    void releaseUserData(int id);
    void clearAllUserData();
    void setThreadedExecution(bool threadedExec);
    bool getThreadedExecution() const;
    bool getThreadedExecutionIsUnderWay() const;
    void setExecutionOrder(int order);
    int getExecutionOrder() const;
    void flagForDestruction();
    bool getFlaggedForDestruction() const;
    void setAddOnScriptAutoRun();
    int getScriptType() const;
    void setScriptIsDisabled(bool isDisabled);
    bool getScriptIsDisabled() const;
    void setDisableCustomizationScriptWithError(bool d);
    bool getDisableCustomizationScriptWithError() const;
    void setExecuteJustOnce(bool justOnce);
    bool getExecuteJustOnce() const;

    void getPreviousEditionWindowPosAndSize(int posAndSize[4]) const;
    void setPreviousEditionWindowPosAndSize(const int posAndSize[4]);

    void setObjectCustomData(int header,const char* data,int dataLength);
    int getObjectCustomDataLength(int header) const;
    void getObjectCustomData(int header,char* data) const;
    bool getObjectCustomDataHeader(int index,int& header) const;

    // Same as above, but data is not serialized (but copied):
    void setObjectCustomData_tempData(int header,const char* data,int dataLength);
    int getObjectCustomDataLength_tempData(int header) const;
    void getObjectCustomData_tempData(int header,char* data) const;
    bool getObjectCustomDataHeader_tempData(int index,int& header) const;

    CLuaScriptParameters* getScriptParametersObject();

    void setCustomizedMainScript(bool customized);
    bool isDefaultMainScript() const;

    std::string errorWithCustomizationScript();
    void setCustomizationScriptIsTemporarilyDisabled(bool disabled);
    bool getCustomizationScriptIsTemporarilyDisabled() const;
    void setCustScriptDisabledDSim_compatibilityMode(bool disabled);
    bool getCustScriptDisabledDSim_compatibilityMode() const;
    void setCustomizationScriptCleanupBeforeSave(bool doIt);
    bool getCustomizationScriptCleanupBeforeSave() const;

    bool hasCustomizationScripAnyChanceToGetExecuted(bool whenSimulationRuns,bool forCleanUpSection) const;

    int getScriptExecutionTimeInMs() const;

    int getErrorReportMode() const;
    void setErrorReportMode(int e);

    std::string getLastErrorString() const;
    void setLastErrorString(const char* txt);

    int getAddOnExecutionState() const;

    bool addCommandToOutsideCommandQueue(int commandID,int auxVal1,int auxVal2,int auxVal3,int auxVal4,const float aux2Vals[8],int aux2Count);
    int extractCommandFromOutsideCommandQueue(int auxVals[4],float aux2Vals[8],int& aux2Count);

    static bool emergencyStopButtonPressed;

    void fromFileToBuffer(); // when using an external editor
    void fromBufferToFile();

    void setInsideCustomLuaFunction(bool inside);
    bool getInsideCustomLuaFunction() const;

    bool getContainsJointCallbackFunction() const;
    bool getContainsContactCallbackFunction() const;
    bool getContainsDynCallbackFunction() const;


    std::string getFilenameForExternalScriptEditor() const;

    VTHREAD_ID_TYPE getThreadedScriptThreadId() const;

    void setAutomaticCascadingCallsDisabled_OLD(bool disabled);
    bool getAutomaticCascadingCallsDisabled_OLD() const;
    int getObjectIDThatScriptIsAttachedTo_callback_OLD() const; // for callback scripts
    void setObjectIDThatScriptIsAttachedTo_callback_OLD(int newObjectID); // for callback scripts
    void runJointCtrlCallback_OLD(const std::vector<bool>& inDataBool,const std::vector<int>& inDataInt,const std::vector<float>& inDataFloat,std::vector<float>& outDataFloat,std::vector<bool>& outDataValidity);
    int runContactCallback_OLD(const int inDataInt[3],int outDataInt[3],float outDataFloat[14]);
    int runGeneralCallback_OLD(int callbackId,int callbackTag,void* additionalData);
    bool checkAndSetWarningAboutSimHandleChildScriptAlreadyIssued_oldCompatibility_7_8_2014();
    bool checkAndSetWarning_simRMLPosition_oldCompatibility_30_8_2014();
    bool checkAndSetWarning_simRMLVelocity_oldCompatibility_30_8_2014();
    bool checkAndSetWarning_simGetMpConfigForTipPose_oldCompatibility_21_1_2016();
    bool checkAndSetWarning_simFindIkPath_oldCompatibility_2_2_2016();
    bool checkAndSetWarning_oldPathPlanningFunctionality_oldCompatibility_11_2_2016();
    bool checkAndSetWarning_oldMotionPlanningFunctionality_oldCompatibility_11_2_2016();

    static bool canCallSystemCallback(int scriptType,bool threaded,int callType);
    static std::string getSystemCallbackString(int calltype,bool callTips);
    static std::vector<std::string> getAllSystemCallbackStrings(int scriptType,bool threaded,bool callTips);

protected:
    bool _luaLoadBuffer(luaWrap_lua_State* luaState,const char* buff,size_t sz,const char* name);
    int _luaPCall(luaWrap_lua_State* luaState,int nargs,int nresult,int errfunc);

    int _runMainScript(int optionalCallType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    int _runMainScriptNow(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    int _runNonThreadedChildScript(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    int _runNonThreadedChildScriptNow(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    void _runThreadedChildScriptNow();
    bool _runCustomizationScript(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    void _runAddOn(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack);
    int _runScriptOrCallScriptFunction(int callType,const CInterfaceStack* inStack,CInterfaceStack* outStack,std::string* errorMsg,bool* hasJointCallbackFunc,bool* hasContactCallbackFunc,bool* hasDynCallbackFunc);

    bool _prepareLuaStateAndCallScriptInitSectionIfNeeded();
    bool _checkIfMixingOldAndNewCallMethods();

    void _runJointCtrlCallback_OLD(int callType,const std::vector<bool>& inDataBool,const std::vector<int>& inDataInt,const std::vector<float>& inDataFloat,std::vector<float>& outDataFloat,std::vector<bool>& outDataValidity);

    // Variables that need to be copied and serialized:
    int scriptID;
    int _scriptType; // sim_scriptproperty_mainscript, etc.
    bool _threadedExecution;
    bool _scriptIsDisabled;
    bool _executeJustOnce;
    bool _mainScriptIsDefaultMainScript;
    bool _disableCustomizationScriptWithError;
    int _executionOrder;
    int _objectIDAttachedTo_child;
    int _objectIDAttachedTo_callback;
    int _objectIDAttachedTo_customization;

    std::string _scriptText;
    std::string _scriptTextExec; // the one getting executed!
    bool _externalScriptText;

    std::vector<int> _scriptFoldingInfo;
    CLuaScriptParameters* scriptParameters;
    COutsideCommandQueueForScript* _outsideCommandQueue;
    CCustomData* _customObjectData;
    CCustomData* _customObjectData_tempData; // same as above, but is not serialized (but copied!)

    // Other variables that don't need serialization:
    luaWrap_lua_State* L;
    VTHREAD_ID_TYPE _threadedScript_associatedFiberOrThreadID;
    int _numberOfPasses;
    bool _threadedExecutionUnderWay;
    int _insideCustomLuaFunction;
    bool _inExecutionNow;
    int _loadBufferResult;

    bool _flaggedForDestruction;

    bool _customizationScriptIsTemporarilyDisabled;
    bool _custScriptDisabledDSim_compatibilityMode;
    bool _customizationScriptCleanupBeforeSave;
    int _scriptExecStartTime;
    int _errorReportMode;
    std::string _lastErrorString;
    bool _compatibilityModeOrFirstTimeCall_sysCallbacks;
    bool _containsJointCallbackFunction;
    bool _containsContactCallbackFunction;
    bool _containsDynCallbackFunction;

    int _messageReportingOverride;

    VMutex _localMutex;
    std::string _addOnName;
    int _addOn_executionState;

    std::string _filenameForExternalScriptEditor;


    std::vector<char*> _userData;
    std::vector<int> _userDataIds;

    bool _initialValuesInitialized;

    int _previousEditionWindowPosAndSize[4];

    bool _warningAboutSimHandleChildScriptAlreadyIssued_oldCompatibility_7_8_2014;
    bool _warning_simRMLPosition_oldCompatibility_30_8_2014;
    bool _warning_simRMLVelocity_oldCompatibility_30_8_2014;
    bool _warning_simGetMpConfigForTipPose_oldCompatibility_21_1_2016;
    bool _warning_simFindIkPath_oldCompatibility_2_2_2016;
    bool _warning_oldPathPlanningFunctionality_oldCompatibility_11_2_2016;
    bool _warning_oldMotionPlanningFunctionality_oldCompatibility_11_2_2016;
    bool _automaticCascadingCallsDisabled_OLD; // reset to false at simulation start!

    static int _nextIdForExternalScriptEditor;
    static VMutex _globalMutex;
    static std::vector<CLuaScriptObject*> toBeCalledByThread;
    static VTHREAD_RETURN_TYPE _startAddressForThreadedScripts(VTHREAD_ARGUMENT_TYPE lpData);
};
