#ifndef _components_plugins_inc_plugin_h
#define _components_plugins_inc_plugin_h

#include "../../../common.h"
#include <map>
#include <vector>
#include <metamod_oslink.h>
#include <filesystem>

#ifndef _WIN32
#include <dlfcn.h>
#endif

const std::string funcsToLoad[] = {
    "RegisterPlayer",
    "OnProgramLoad",
    "OnPluginStart",
    "OnPluginStop",
    "OnClientConnected",
    "OnClientConnect",
    "OnPlayerSpawn",
    "OnGameTick",
    "OnPlayerChat",
};

class Plugin
{
private:
    std::string m_path;
    std::map<std::string, void *> functions;
    HINSTANCE m_hModule;
    std::string m_pluginName;
    bool isPluginLoaded = false;

    void RegisterFunction(std::string function)
    {
        if (this->functions.find(function) != this->functions.end())
            return;

        void *func = reinterpret_cast<void *>(dlsym(this->m_hModule, function.c_str()));
        if (func == nullptr)
            return;

        this->functions.insert(std::make_pair(function, func));
    }

public:
    Plugin(std::string path) : m_path(path)
    {
        std::vector<std::string> exploded = explode(path, WIN_LINUX("\\", "/"));
        this->m_pluginName = explode(exploded[exploded.size() - 1], ".")[0];
    }
    ~Plugin()
    {
        this->m_path.clear();
        dlclose(this->m_hModule);
    }

    void LoadPlugin()
    {
#ifdef _WIN32
        this->m_hModule = dlmount(this->m_path.c_str());
#else
        this->m_hModule = dlopen(string_format("%s/%s", std::filesystem::current_path().string().c_str(), this->m_path.c_str()).c_str(), RTLD_LAZY);

        if (!this->m_hModule)
        {
            PRINTF("LoadPlugin", "Failed to load module: %s\n", std::string(dlerror()).c_str());
        }
#endif
        PRINT("LoadPlugin", string_format("%s/%s\nhModule addr: %p\n", std::filesystem::current_path().string().c_str(), this->m_path.c_str(), this->m_hModule).c_str());

        for (uint16 i = 0; i < ARR_SIZE(funcsToLoad); i++)
            this->RegisterFunction("Internal_" + funcsToLoad[i]);

        this->RegisterFunction("GetPluginAuthor");
        this->RegisterFunction("GetPluginVersion");
        this->RegisterFunction("GetPluginName");
        this->RegisterFunction("GetPluginWebsite");
    }

    void StartPlugin();
    void StopPlugin();
    inline void SetPluginLoaded(bool loaded) { this->isPluginLoaded = loaded; };
    inline bool IsPluginLoaded() { return this->isPluginLoaded; };
    std::string GetName() { return this->m_pluginName; }

    void *FetchFunction(std::string function)
    {
        if (this->functions.find(function) == this->functions.end())
            return nullptr;

        return this->functions.at(function);
    }
};

extern std::map<std::string, Plugin *> pluginsMap;
extern std::vector<Plugin *> plugins;

#endif