#pragma once

#include <string>

namespace settings_manager
{
    enum settings_type
    {
        type_none, // Note: support for saving and loading to JSON files will be unavailable if this is the active type
        type_bool,
        type_int,
        type_float
    };

    union settings_value {
        bool bool_value;
        int int_value;
        float float_value;
    };

    struct settings_node
    {
    public:
        settings_type m_type;

        settings_node(settings_type type) : m_type(type)
        {
        }

        settings_node() = default;

        ~settings_node() = default;

    private:
        union {
            bool m_value_bool;
            int m_value_int;
            float m_value_float;
        };

    public:
        bool &value_bool()
        {
            m_type = type_bool;
            return m_value_bool;
        }

        int &value_int()
        {
            m_type = type_int;
            return m_value_int;
        }

        float &value_float()
        {
            m_type = type_float;
            return m_value_float;
        }
    };

    void add(const std::string &name, settings_type type); // Register setting
    settings_node &get(const std::string &name);           // Get setting

    void load_json(const std::string &file_path);
    void save_json(const std::string &file_path);
} // namespace settings_manager