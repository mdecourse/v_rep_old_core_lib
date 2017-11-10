
#pragma once

#include "vrepMainHeader.h"
#include "luaWrapper.h"

class CLuaCustomVariable
{
public:
    CLuaCustomVariable(const char* theFullVariableName,const char* theVariableValue,int theVariableStackValue);

    virtual ~CLuaCustomVariable();
    
    void pushVariableOntoLuaStack(luaWrap_lua_State* L,bool handleOnlyRequireAssignments);
    bool shouldBeDestroyed(const char* pluginName);
    bool isVariableNameSame(const char* fullName);
    bool getNoAutoCompletion();
    std::string getVariableName();

protected:
    std::string _getVariableNameFromFull(const char* fullName);
    std::string _getPluginNameFromFull(const char* fullName);

    std::string _pluginName;
    std::string _variableName;
    std::string _variableValue;
    int _variableStackValue;
    bool _noAutocompletion;
};
