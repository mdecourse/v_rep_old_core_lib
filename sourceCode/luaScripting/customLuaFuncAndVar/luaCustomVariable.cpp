
#include "vrepMainHeader.h"
#include "luaCustomVariable.h"
#include "app.h"

CLuaCustomVariable::CLuaCustomVariable(const char* theFullVariableName,const char* theVariableValue,int theVariableStackValue)
{
    _variableName=_getVariableNameFromFull(theFullVariableName);
    _pluginName=_getPluginNameFromFull(theFullVariableName);
    _noAutocompletion=false;

    if (theVariableValue!=NULL)
    { // we register a simple variable
        _variableValue=theVariableValue;
        _variableStackValue=0; // i.e. not using the stack
        _noAutocompletion=(theVariableStackValue==-1);
    }
    else
    {
        _variableValue="";
        _variableStackValue=theVariableStackValue;
    }
}

CLuaCustomVariable::~CLuaCustomVariable()
{
    if (_variableStackValue!=0)
        App::ct->interfaceStackContainer->destroyStack(_variableStackValue);
}

bool CLuaCustomVariable::getNoAutoCompletion()
{
    return(_noAutocompletion);
}

void CLuaCustomVariable::pushVariableOntoLuaStack(luaWrap_lua_State* L,bool handleOnlyRequireAssignments)
{
    if (_variableStackValue==0)
    { // simple variable
        bool doIt=false;
        if ( handleOnlyRequireAssignments&&(_variableValue.find("require")==0) )
            doIt=true;
        if ( (!handleOnlyRequireAssignments)&&(_variableValue.find("require")!=0) )
            doIt=true;
        if (doIt)
        {
            std::string tmp(_variableName);
            tmp+="="+_variableValue;
            luaWrap_luaL_dostring(L,tmp.c_str());
        }
    }
    else
    { // stack variable
        if (!handleOnlyRequireAssignments)
        {
            CInterfaceStack* stack=App::ct->interfaceStackContainer->getStack(_variableStackValue);
            stack->buildOntoLuaStack(L,true);
            luaWrap_lua_setglobal(L,_variableName.c_str());
        }
    }
}

bool CLuaCustomVariable::isVariableNameSame(const char* fullName)
{
    std::string varName(_getVariableNameFromFull(fullName));
    return(_variableName.compare(varName)==0);
}

bool CLuaCustomVariable::shouldBeDestroyed(const char* pluginName)
{
    return(_pluginName.compare(pluginName)==0);
}

std::string CLuaCustomVariable::getVariableName()
{
    return(_variableName);
}

std::string CLuaCustomVariable::_getVariableNameFromFull(const char* fullName)
{
    std::string f(fullName);
    size_t p=f.find('@');
    if (p==std::string::npos)
        return(f);
    std::string subF(f.begin(),f.begin()+p);
    return(subF);
}

std::string CLuaCustomVariable::_getPluginNameFromFull(const char* fullName)
{
    std::string f(fullName);
    size_t p=f.find('@');
    if (p==std::string::npos)
        return("");
    std::string subF(f.begin()+p+1,f.end());
    return(subF);
}
