#pragma once

#include <serialization/detail/config.h>
#include <serialization/detail/serialization_error.h>

#include <serialization/detail/rapidjson/rapidjson.h>
#include <serialization/detail/rapidjson/document.h>
#include <serialization/detail/rapidjson/writer.h>

#include <stack>
#include <cassert>

namespace serialization
{

class json_iarchive
{
public:
    json_iarchive()
    {
    }

    void load_data(const char * data)
    {
        doc_.Parse(data);
        if(doc_.GetParseError())
        {
            throw_serialization_error("json", "parse");
        }
    }

    void load_key_start(const char * key) const
    {
        serialization_trace trace(__func__, key);

        if(stack_.empty() || (!stack_.top()->IsObject()))
            throw_serialization_error("expect key", key);

        auto iter = stack_.top()->FindMember(key);
        if(iter == stack_.top()->MemberEnd())
            throw_serialization_error("expect key", key);

        stack_.push(&iter->value);
    }

    bool load_key_start_optional(const char * key) const
    {
        serialization_trace trace(__func__, key);

        if(stack_.empty() || (!stack_.top()->IsObject()))
            throw_serialization_error("expect key", key);

        auto iter = stack_.top()->FindMember(key);
        if(iter == stack_.top()->MemberEnd())
            return false;

        stack_.push(&iter->value);
        return true;
    }

    void load_key_end(const char * key) const
    {
        serialization_trace trace(__func__, key);
        assert(!stack_.empty());
        stack_.pop();
    }

    void load_object_start() const
    {
        serialization_trace trace(__func__, "json");
        assert(!stack_.empty());
        if(stack_.empty() || (!stack_.top()->IsObject()))
            throw_serialization_error("expect object", "");
    }

    void load_object_end() const
    {
        serialization_trace trace(__func__, "json");
    }

    size_type sequence_size() const
    {
        if(stack_.empty() || (!stack_.top()->IsArray()))
            return 0;
        return stack_.top()->GetArray().Size();
    }

    bool load_sequence_start(size_type i) const
    {
        serialization_trace trace(__func__, "json");
        assert(!stack_.empty());
        if(stack_.empty() || (!stack_.top()->IsArray()))
            throw_serialization_error("expect array", "");
        if(i >= stack_.top()->GetArray().Size())
            return false;
        stack_.push(&stack_.top()->GetArray()[i]);
        return true;
    }

    void load_sequence_end() const
    {
        serialization_trace trace(__func__, "json");
        assert(!stack_.empty());
        stack_.pop();
    }

    void load(bool & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsBool()))
            throw_serialization_error("json", "expect bool");
        v = stack_.top()->GetBool();
    }

    void load(int & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsInt()))
            throw_serialization_error("json", "expect int");
        v = stack_.top()->GetInt();
    }

    void load(uint & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsUint()))
            throw_serialization_error("json", "expect uint");
        v = stack_.top()->GetUint();
    }

    void load(int64_t & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsInt64()))
            throw_serialization_error("json", "expect int64");
        v = stack_.top()->GetInt64();
    }

    void load(uint64_t & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsUint64()))
            throw_serialization_error("json", "expect uint64");
        v = stack_.top()->GetUint64();
    }

    void load(double & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsNumber()))
            throw_serialization_error("json", "expect number");
        v = stack_.top()->GetDouble();
    }

    void load(std::string & v) const
    {
        if(stack_.empty() || (!stack_.top()->IsString()))
            throw_serialization_error("json", "expect string");
        v = stack_.top()->GetString();
    }

    void unserialize_start() const
    {
        serialization_trace trace(__func__, "json");
        stack_ = std::stack<const rapidjson::Value *> {};
        stack_.push(&doc_);
    }

    void unserialize_end() const
    {
        serialization_trace trace(__func__, "json");
    }

private:
    rapidjson::Document doc_;

    mutable std::stack<const rapidjson::Value *> stack_;
};

} // namespace serialization
