#pragma once

#include "../../common.h"
#include "../../entrypoint.h"
#include "../Plugin.h"
#include "../PluginManager.h"
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