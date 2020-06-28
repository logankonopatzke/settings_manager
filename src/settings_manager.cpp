#include "settings_manager.hpp"

#include <unordered_map>
#include <fstream>

#include "json/json.h"

namespace settings_manager
{
    static std::unordered_map<std::string, settings_node> map;

    void add(const std::string &name, settings_type type)
    {
        map[name] = settings_node(type);
    }

    settings_node &get(const std::string &name)
    {
        return map[name];
    }

    void load_json(const std::string &file_path)
    {
        Json::Value json;
        std::ifstream fin(file_path);
        if (!fin.good())
            return;

        fin >> json;

        for (auto &it : map)
        {
            switch (it.second.m_type)
            {
            case type_none:
                // We can't do anything because we don't know the active type in the union
                break;
            case type_bool:
                if (json.isMember(it.first))
                    it.second.value_bool() = json[it.first].asBool();
                break;

            case type_int:
                if (json.isMember(it.first))
                    it.second.value_int() = json[it.first].asInt();
                break;

            case type_float:
                if (json.isMember(it.first))
                    it.second.value_float() = json[it.first].asFloat();
                break;
            }
        }
    }

    void save_json(const std::string &file_path)
    {
        Json::Value json;

        for (auto &it : map)
        {
            switch (it.second.m_type)
            {
            case type_none:
                // We can't do anything because we don't know the active type in the union
                break;
            case type_bool:
                json[it.first] = it.second.value_bool();
                break;

            case type_int:
                json[it.first] = it.second.value_int();
                break;

            case type_float:
                json[it.first] = it.second.value_float();
                break;
            }
        }

        std::ofstream fout(file_path);
        if (!fout.good())
            return;

        fout << json;
    }
} // namespace settings_manager