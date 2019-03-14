#pragma once

#include <serialization/detail/config.h>

#include <serialization/detail/rapidxml/rapidxml.hpp>
#include <serialization/detail/rapidxml/rapidxml_print.hpp>
#include <serialization/detail/rapidjson/internal/itoa.h>
#include <serialization/detail/rapidjson/internal/dtoa.h>

namespace serialization
{

class xml_oarchive
{
public:
    xml_oarchive()
    {
    }

    const char * data() const
    {
        return str_.data();
    }

    std::size_t size() const
    {
        return str_.size();
    }

    void save_key_start(const char * key)
    {
        serialization_trace trace(__func__, key);
        auto node = document_.allocate_node(rapidxml::node_element, key);
        current_->append_node(node);
        current_ = node;
    }

    void save_key_end(const char * key)
    {
        serialization_trace trace(__func__, key);
        current_ = current_->parent();
    }

    void save_object_start()
    {
    }

    void save_object_end()
    {
    }

    void save_sequence_start(size_type size)
    {
        serialization_trace trace(__func__, "xml");
        auto node = document_.allocate_node(rapidxml::node_element, "sequence");
        auto attr = document_.allocate_attribute("size", document_.allocate_string(std::to_string(size).c_str()));
        node->append_attribute(attr);
        current_->append_node(node);
        current_ = node;
    }

    void save_sequence_end()
    {
        serialization_trace trace(__func__, "xml");
        current_ = current_->parent();
    }

    void save_sequence_item_start()
    {
        serialization_trace trace(__func__, "xml");
        auto node = document_.allocate_node(rapidxml::node_element, "item");
        current_->append_node(node);
        current_ = node;
    }

    void save_sequence_item_end()
    {
        serialization_trace trace(__func__, "xml");
        current_ = current_->parent();
    }

	void save(int v)
	{
		char buffer[64];
		const char* end = rapidjson::internal::i64toa(v, buffer);
        current_->value(document_.allocate_string(buffer, end - buffer), end - buffer);
	}

	void save(uint v)
	{
		char buffer[64];
		const char* end = rapidjson::internal::u64toa(v, buffer);
        current_->value(document_.allocate_string(buffer, end - buffer), end - buffer);
	}

	void save(int64_t v)
	{
		char buffer[64];
		const char* end = rapidjson::internal::i64toa(v, buffer);
        current_->value(document_.allocate_string(buffer, end - buffer), end - buffer);
	}

	void save(uint64_t v)
	{
		char buffer[64];
		const char* end = rapidjson::internal::u64toa(v, buffer);
        current_->value(document_.allocate_string(buffer, end - buffer), end - buffer);
	}

	void save(double v)
    {
		char buffer[64];
		const char* end = rapidjson::internal::dtoa(v, buffer);
        current_->value(document_.allocate_string(buffer, end - buffer), end - buffer);
    }

    void save(const std::string & v)
    {
        current_->value(v.c_str(), v.size());
    }

    void save(bool v)
    {
        if(v)
        {
            current_->value("true");
        }
        else
        {
            current_->value("false");
        }
    }

    void serialize_start()
    {
        serialization_trace trace(__func__, "xml");
        document_.clear();
        current_ = document_.allocate_node(rapidxml::node_element, "serialization");
        document_.append_node(current_);
        str_.clear();
    }

    void serialize_end()
    {
        serialization_trace trace(__func__, "xml");
		rapidxml::print(std::back_inserter<std::string>(str_), document_, rapidxml::print_no_indenting);
    }

private:
    rapidxml::xml_document<> document_;

    rapidxml::xml_node<> *current_;

    std::string str_;
};

} // namespace serialization
