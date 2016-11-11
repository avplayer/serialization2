#pragma once

#include <string>
#include <sstream>
#include <cstring>
#include <stack>

#include <serialization/detail/rapidxml/rapidxml.hpp>
#include <serialization/detail/rapidxml/rapidxml_print.hpp>

namespace serialization
{

class xml_oarchive
{
    typedef rapidxml::xml_node<> xml_node_type;
    typedef rapidxml::xml_document<> xml_document_type;
    typedef std::string::const_iterator const_iterator;
    typedef std::string::size_type size_type;
public:
    xml_oarchive(const std::string & root = "", size_t size = 0)
        : root_(root)
	{
        if(size != 0)
            data_.reserve(size);
	}

    const char * data() const
    {
        return data_.data();
    }

    size_type size() const
    {
        return data_.size();
    }

	const_iterator begin() const
	{
		return data_.begin();
	}

	const_iterator end() const
	{
		return data_.end();
    }

    void save_start(const char * name)
    {
        current_ = create_node(name);
        parent_->append_node(current_);
        stack_.push(parent_);
        parent_ = current_;
    }

    void save_end(const char * name)
    {
        parent_ = stack_.top();
        stack_.pop();
        current_ = 0;
    }

    template<typename T>
    void save(const T & v)
    {
        if(!current_)
		  throw_serialization_error("save", "xml node null");
        current_->value(get_string(v, doc_));
    }

    void serialize_start()
    {
        doc_.clear();
        stack_ = decltype(stack_)();
        parent_ = doc_.allocate_node(rapidxml::node_element, doc_.allocate_string(root_.c_str()));
        current_ = 0;
        doc_.append_node(parent_);
    }

    void serialize_end()
    {
        rapidxml::print(std::back_inserter(data_), doc_);
    }

private:
    template<typename Argument, typename Container>
    bool write_container(const Argument & arg, const Container & in, const char * type, std::size_t size);

    xml_node_type * create_node(const char * name, const char * value = 0, std::size_t value_size = 0)
    {
        auto node = doc_.allocate_node(rapidxml::node_element, name, value, 0, value_size);
        //parent_->append_node(node);
        return node;
    }

    template<typename T>
    char * get_string(const T & in, xml_document_type & doc)
    {
        std::stringstream ss;
        ss << in;
        return doc.allocate_string(ss.str().c_str());
    }

    char * get_string(const std::string & str, xml_document_type & doc)
    {
        return doc.allocate_string(str.c_str());
    }

    std::string data_;

    xml_document_type doc_;

    std::string root_;

    xml_node_type * current_;
    xml_node_type * parent_;
    std::stack<xml_node_type *> stack_;
};

} // namespace serialization
