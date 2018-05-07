
#include "vrepMainHeader.h"
#include "luaCustomFunction.h"

CLuaCustomFunction::CLuaCustomFunction(const char* theFullFunctionName,const char* theCallTips,std::vector<int>& theInputArgTypes,void(*callBack)(struct SLuaCallBack* p))
{ // the old way, called through simRegisterCustomLuaFunction
    useStackToExchangeData=false;
    if (theFullFunctionName!=NULL)
    {
        functionName=_getFunctionNameFromFull(theFullFunctionName);
        pluginName=_getPluginNameFromFull(theFullFunctionName);
    }
    if (theCallTips!=NULL)
        callTips=theCallTips;
    for (size_t i=0;i<theInputArgTypes.size();i++)
        inputArgTypes.push_back(theInputArgTypes[i]);
    callBackFunction_new=NULL;
    callBackFunction_old=callBack;
}

CLuaCustomFunction::CLuaCustomFunction(const char* theFullFunctionName,const char* theCallTips,void(*callBack)(struct SScriptCallBack* cb))
{ // the new way, called through simRegisterScriptCallbackFunction
    useStackToExchangeData=true;
    if (theFullFunctionName!=NULL)
    {
        functionName=_getFunctionNameFromFull(theFullFunctionName);
        pluginName=_getPluginNameFromFull(theFullFunctionName);
    }
    if (theCallTips!=NULL)
        callTips=theCallTips;
    callBackFunction_old=NULL;
    callBackFunction_new=callBack;
}

CLuaCustomFunction::~CLuaCustomFunction()
{
}

bool CLuaCustomFunction::hasCallback()
{
    return( (callBackFunction_old!=NULL)||(callBackFunction_new!=NULL) );
}

void CLuaCustomFunction::registerCustomLuaFunction(luaWrap_lua_State* L,luaWrap_lua_CFunction func)
{
    if ( (callBackFunction_old!=NULL)||(callBackFunction_new!=NULL) )
    {
        size_t p=functionName.find(".");
        if (p!=std::string::npos)
        { // this is the new notation, e.g. simUI.create()
            std::string prefix(functionName.begin(),functionName.begin()+p);

            luaWrap_lua_pushnumber(L,functionID+1);
            luaWrap_lua_pushcclosure(L,func,1);
            luaWrap_lua_setfield(L,luaWrapGet_LUA_GLOBALSINDEX(),"__iuafkjsdgoi158zLK");

            std::string tmp("if not ");
            tmp+=prefix;
            tmp+=" then ";
            tmp+=prefix;
            tmp+="={} end ";
            tmp+=functionName;
            tmp+="=__iuafkjsdgoi158zLK __iuafkjsdgoi158zLK=nil";
            luaWrap_luaL_dostring(L,tmp.c_str());
        }
        else
        {
            luaWrap_lua_pushnumber(L,functionID+1);
            luaWrap_lua_pushcclosure(L,func,1);
            luaWrap_lua_setfield(L,luaWrapGet_LUA_GLOBALSINDEX(),functionName.c_str());
        }
    }

/* Following until 21/5/2017:
    luaWrap_lua_pushnumber(L,functionID+1);
    luaWrap_lua_pushcclosure(L,func,1);
    luaWrap_lua_setfield(L,luaWrapGet_LUA_GLOBALSINDEX(),functionName.c_str());
*/

/* removed on 31/5/2016
#ifdef LIN_VREP
    luaWrap_lua_setglobal(L,functionName.c_str());
#else
    luaWrap_lua_setfield(L,luaWrapGet_LUA_GLOBALSINDEX(),functionName.c_str());
#endif
*/
}

bool CLuaCustomFunction::getUsesStackToExchangeData()
{
    return(useStackToExchangeData);
}

bool CLuaCustomFunction::isFunctionNameSame(const char* fullName)
{
    std::string funcName(_getFunctionNameFromFull(fullName));
    return(functionName.compare(funcName)==0);
}

bool CLuaCustomFunction::isPluginNameSame(const char* plugName)
{
    return(pluginName.compare(plugName)==0);
}

std::string CLuaCustomFunction::getFunctionName()
{
    return(functionName);
}

std::string CLuaCustomFunction::getPluginName()
{
    return(pluginName);
}

std::string CLuaCustomFunction::getCallTips()
{
    return(callTips);
}

int CLuaCustomFunction::getFunctionID()
{
    return(functionID);
}

void CLuaCustomFunction::setFunctionID(int newID)
{
    functionID=newID;
}

std::string CLuaCustomFunction::_getFunctionNameFromFull(const char* fullName)
{
    std::string f(fullName);
    size_t p=f.find('@');
    if (p==std::string::npos)
        return(f);
    std::string subF(f.begin(),f.begin()+p);
    return(subF);
}

std::string CLuaCustomFunction::_getPluginNameFromFull(const char* fullName)
{
    std::string f(fullName);
    size_t p=f.find('@');
    if (p==std::string::npos)
        return("");
    std::string subF(f.begin()+p+1,f.end());
    return(subF);
}
