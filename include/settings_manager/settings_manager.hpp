#pragma once

#include <string>
#include <array>
#include <cassert>
#include <cstdint>

namespace settings_manager
{
    enum settings_type
    {
        type_none, // Note: support for saving and loading to JSON files will be unavailable if this is the active type
        type_bool,
        type_int,
        type_float,
        type_color
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
        union
        {
            bool m_value_bool;
            int m_value_int;
            float m_value_float;
            std::array<float, 3> m_value_color;
        };

    public:
        bool &value_bool() const
        {
            assert(m_type == type_bool && "Type different from value getter" v);
            return m_value_bool;
        }

        int &value_int() const
        {
            assert(m_type == type_int && "Type different from value getter");
            return m_value_int;
        }

        float &value_float() const
        {
            assert(m_type == type_float && "Type different from value getter");
            return m_value_float;
        }

        std::array<float, 3> &value_color() const
        {
            assert(m_type == type_color && "Type different from value getter");
            return m_value_color;
        }
    };

    void add(std::uint32_t name, settings_type type); // Register setting
    settings_node &get(std::uint32_t name);           // Get setting

    void load_json(const std::string &file_path);
    void save_json(const std::string &file_path);
} // namespace settings_manager