#pragma once

#include <string>
#include <stack>
#include <sstream>
#include <cstring>

#include <serialization/detail/rapidxml/rapidxml.hpp>

namespace serialization
{

class xml_iarchive
{
    typedef rapidxml::xml_node<> xml_node_type;
    typedef rapidxml::xml_document<> xml_document_type;
public:
    xml_iarchive(const std::string & root = "", size_t size = 0)
        : root_(root)
        , index_(0)
	{
	}

    bool load_from_string(std::string & xml)
    {
        doc_.clear();
        try
        {
            doc_.parse<0>(&xml[0]);
            return true;
        }
        catch(const rapidxml::parse_error & )
        {
            return false;
        }
    }

    void load_start(const char * name) const
    {
        if(!check_node_name(current_, name))
		  throw_serialization_error("load start", name);
        stack_.push(current_);
        current_ = current_->first_node();
    }

    void load_end(const char * name) const
    {
        assert(stack_.size() != 0);
        current_ = stack_.top();
        stack_.pop();
        current_ = current_->next_sibling();
    }

    template<typename T>
    void load(T & out) const
    {
        assert(stack_.size() != 0);
        current_ = stack_.top();
        get_value(current_->value(), out);
    }

    void unserialize_start() const
    {
        stack_ = decltype(stack_)();
        current_ = doc_.first_node();
        if(!check_node_name(current_, root_.data()))
		  throw_serialization_error("unserialize start", "check node name");
        if(!current_)
		  throw_serialization_error("unserialize start", "check node name");
        current_ = current_->first_node();
    }

    void unserialize_end() const
    {
    }

private:
    template<typename T>
    void get_value(const char * str, T & dst) const
    {
        if(!str)
		  throw_serialization_error("get value", "null value");
        std::stringstream ss;
        ss << str;
        ss >> dst;
		if(!ss.eof())
		  throw_serialization_error("get value", "eof");
    }

    void get_value(const char * str, std::string & dst) const
    {
        if(!str)
		  throw_serialization_error("get value", "null value");
        dst = str;
    }

    bool check_node_name(xml_node_type * node, const char * str) const
    {
        if(node == 0 || std::strcmp(node->name(), str) != 0)
            return false;
        return true;
    }

    xml_document_type doc_;

    std::string root_;

    mutable xml_node_type * current_;

    mutable std::stack<xml_node_type *> stack_;

	mutable size_t index_;
};

} // namespace serialization
