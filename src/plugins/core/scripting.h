#pragma once

#include "../../common.h"
#include "../../entrypoint.h"
#include "../Plugin.h"
#include "../PluginManager.h"
#include "../../database/DatabaseManager.h"
#include "../../types/LogType.h"
#include "../../logs/Logger.h"

#include <any>

//////////////////////////////////////////////////////////////
/////////////////            Logger            //////////////
////////////////////////////////////////////////////////////

class PluginLogger
{
private:
    std::string plugin_name;

public:
    PluginLogger(std::string m_plugin_name);

    void Write(int level, std::string message);
};

//////////////////////////////////////////////////////////////
/////////////////        Player Manager        //////////////
////////////////////////////////////////////////////////////

class PluginPlayerManager
{
private:
    std::string plugin_name;

public:
    PluginPlayerManager(std::string m_plugin_name);

    uint16_t GetPlayerCount();
    uint16_t GetPlayerCap();
    void SendMsg(int destination, std::string text);
};

//////////////////////////////////////////////////////////////
/////////////////         HTTP Client          //////////////
////////////////////////////////////////////////////////////

class PluginHTTP
{
private:
    std::string plugin_name;

public:
    PluginHTTP(std::string m_plugin_name);

    std::string PerformHTTP(std::string receivedData);
};

//////////////////////////////////////////////////////////////
/////////////////         Event System         //////////////
////////////////////////////////////////////////////////////

class PluginEvent
{
private:
    std::string plugin_name;
    IGameEvent *gameEvent;
    void *hookPtr;

    std::any returnValue;

public:
    PluginEvent(std::string m_plugin_name, IGameEvent *m_gameEvent, void *m_hookPtr);
    ~PluginEvent();

    std::string GetInvokingPlugin();
    bool IsGameEvent();
    bool IsHook();

    void SetBool(std::string key, bool value);
    void SetInt(std::string key, int value);
    void SetUint64(std::string key, uint64_t value);
    void SetFloat(std::string key, float value);
    void SetString(std::string key, std::string value);

    bool GetBool(std::string key);
    int GetInt(std::string key);
    uint64_t GetUint64(std::string key);
    float GetFloat(std::string key);
    std::string GetString(std::string key);

    void FireEvent(bool dontBroadcast);
    void FireEventToClient(int slot);

    void SetReturn(std::any value);
    void SetReturnLua(luabridge::LuaRef value);
    std::any GetReturnValue();
};

//////////////////////////////////////////////////////////////
/////////////////            Commands          //////////////
////////////////////////////////////////////////////////////

class PluginCommands
{
private:
    std::string plugin_name;

public:
    PluginCommands(std::string m_plugin_name);

    void RegisterCommand(std::string commandName, void *callback);
    void UnregisterCommand(std::string commandName);

    void RegisterAlias(std::string commandName, std::string aliasName);
    void UnregisterAlias(std::string aliasName);

    void RegisterCommandLua(std::string commandName, luabridge::LuaRef callback);
};

//////////////////////////////////////////////////////////////
/////////////////           Database           //////////////
////////////////////////////////////////////////////////////

class PluginDatabase
{
private:
    std::string connection_name;
    Database *db;
    bool dbConnected;

public:
    PluginDatabase(std::string m_connection_name);

    bool IsConnected();
    std::string EscapeString(std::string str);

    void QueryLua(std::string query, luabridge::LuaRef callback, lua_State *L);
};

//////////////////////////////////////////////////////////////
/////////////////         Configuration        //////////////
////////////////////////////////////////////////////////////

class PluginConfiguration
{
private:
    std::string plugin_name;

public:
    PluginConfiguration(std::string m_plugin_name);

    bool Exists(std::string key);
    void Reload(std::string key);
    uint64_t FetchArraySize(std::string key);
    std::any Fetch(std::string key);

    luabridge::LuaRef FetchLua(std::string key, lua_State *L);
};

//////////////////////////////////////////////////////////////
/////////////////             Menus            //////////////
////////////////////////////////////////////////////////////

class PluginMenus
{
private:
    std::string plugin_name;

public:
    PluginMenus(std::string m_plugin_name);

    void Register(std::string custom_id, std::string title, std::string color, std::vector<std::pair<std::string, std::string>> options);
    void RegisterTemporary(std::string custom_id, std::string title, std::string color, std::vector<std::pair<std::string, std::string>> options);
    void Unregister(std::string id);
};

//////////////////////////////////////////////////////////////
/////////////////             Files            //////////////
////////////////////////////////////////////////////////////

class PluginFiles
{
private:
    std::string plugin_name;

public:
    PluginFiles(std::string m_plugin_name);

    void Append(std::string path, std::string content, bool hasdate);
    bool CreateDirectory(std::string path);
    void Delete(std::string path);
    bool ExistsPath(std::string path);
    std::vector<std::string> FetchDirectories(std::string path);
    std::vector<std::string> FetchFileNames(std::string path);
    std::string GetBase(std::string path);
    bool IsDirectory(std::string path);
    std::string Read(std::string path);
    void Write(std::string path, std::string content, bool hasdate);
};

//////////////////////////////////////////////////////////////
/////////////////           Precacher          //////////////
////////////////////////////////////////////////////////////

class PluginPrecacher
{
private:
    std::string plugin_name;

public:
    PluginPrecacher(std::string m_plugin_name);

    void PrecacheModel(std::string model);
    void PrecacheSound(std::string path);
};

//////////////////////////////////////////////////////////////
/////////////////         Translations         //////////////
////////////////////////////////////////////////////////////

std::string scripting_FetchTranslation(Plugin *plugin, std::string key);