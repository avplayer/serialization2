#pragma once

#include <string>
#include <stack>
#include <sstream>
#include <cstring>

#include <serialization/detail/config.h>
#include <serialization/detail/serialization_error.h>
#include <serialization/detail/rapidxml/rapidxml.hpp>

namespace serialization
{

class xml_iarchive
{
public:
    xml_iarchive()
    {
    }

    void load_data(char * data)
    {
        document_.clear();
        try
        {
            document_.parse<0>(data);
        }
        catch(const rapidxml::parse_error & e)
        {
            throw_serialization_error("xml parse", e.what());
        }
    }

    void load_key_start(const char * key) const
    {
        serialization_trace trace(__func__, key);

        auto node = stack_.top()->first_node(key);
        if(node == nullptr)
            throw_serialization_error("expect key", key);
        stack_.push(node);
    }

    bool load_key_start_optional(const char * key) const
    {
        serialization_trace trace(__func__, key);

        auto node = stack_.top()->first_node(key);
        if(node == nullptr)
        {
            return false;
        }
        stack_.push(node);
        return true;
    }

    void load_key_end(const char * key) const
    {
        serialization_trace trace(__func__, key);
        stack_.pop();
    }

    void load_object_start() const
    {
    }

    void load_object_end() const
    {
    }

    size_type load_sequence_start() const
    {
        serialization_trace trace(__func__, "xml");
        auto node = stack_.top()->first_node("sequence");
        if(node == nullptr)
        {
            throw_serialization_error(__func__, "expect sequence");
        }
        auto attr = node->first_attribute("size");
        if(attr == nullptr)
        {
            throw_serialization_error(__func__, "expect sequence size");
        }

        char *end = attr->value();
        size_type size = std::strtoull(attr->value(), &end, 10);
        if(end - attr->value() != attr->value_size())
        {
            throw_serialization_error(__func__, "bad sequence size");
        }

        stack_.push(node);
        node = (size == 0 ? nullptr : stack_.top()->first_node("item"));
        stack_.push(node);

        return size;
    }

    void load_sequence_end() const
    {
        serialization_trace trace(__func__, "xml");
        stack_.pop();
        stack_.pop();
    }

    bool load_sequence_item_start(size_type i) const
    {
        serialization_trace trace(__func__, "xml");

        if(stack_.top() == nullptr)
        {
            throw_serialization_error("load sequence item", "xml");
        }
        return true;
    }

    void load_sequence_item_end() const
    {
        serialization_trace trace(__func__, "json");
        stack_.top() = stack_.top()->next_sibling("item");
    }

    template<typename T>
    void load(T & v) const
    {
        std::stringstream ss;
        ss << stack_.top()->value();
        ss >> v;
        if(!ss.eof())
        {
            throw_serialization_error("load ", "xml");
        }
    }

    void load(std::string & v) const
    {
        v = stack_.top()->value();
    }

    void load(bool & v) const
    {
        if(std::strcmp(stack_.top()->value(), "true"))
        {
            v = true;
        }
        else if(std::strcmp(stack_.top()->value(), "false"))
        {
            v = false;
        }
        throw_serialization_error("load ", "xml");
    }

    void unserialize_start() const
    {
        serialization_trace trace(__func__, "xml");
        stack_ = std::stack<const rapidxml::xml_node<> *> {};
        auto node = document_.first_node("serialization");
        if(node == nullptr)
            throw_serialization_error("serilization root", "xml");
        stack_.push(node);
    }

    void unserialize_end() const
    {
        serialization_trace trace(__func__, "xml");
    }

private:
    rapidxml::xml_document<> document_;

    mutable std::stack<const rapidxml::xml_node<> * > stack_;
};

} // namespace serialization
