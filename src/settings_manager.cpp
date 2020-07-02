#include "settings_manager/settings_manager.hpp"

#include <unordered_map>
#include <fstream>

#include "json/json.h"

namespace settings_manager
{
    static std::unordered_map<std::string, settings_node> map;

    void add(const std::string &name, settings_type type)
    {
        settings_node node{type};
        switch (type)
        {
        case type_none:
            // We can't do anything because we don't know the active type in the union
            break;
        case type_bool:
            node.value_bool() = false;
            break;

        case type_int:
            node.value_int() = 0;
            break;

        case type_float:
            node.value_float() = 0.0f;
            break;

        case type_color:
            node.value_color() = {0.0f, 0.0f, 0.0f};
            break;
        }
        map[name] = std::move(node);
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

            case type_color:
                if (json.isMember(it.first))
                {
                    auto &color = json[it.first];
                    if (color.isMember("r") && color.isMember("g") && color.isMember("b"))
                    {
                        it.second.value_color()[0] = color["r"].asFloat();
                        it.second.value_color()[1] = color["g"].asFloat();
                        it.second.value_color()[2] = color["b"].asFloat();
                    }
                }
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

            case type_color:
                auto &color = json[it.first];
                color["r"] = it.second.value_color()[0];
                color["g"] = it.second.value_color()[1];
                color["b"] = it.second.value_color()[2];
                break;
            }
        }

        std::ofstream fout(file_path);
        if (!fout.good())
            return;

        fout << json;
    }
} // namespace settings_manager