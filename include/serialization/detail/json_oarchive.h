#pragma once

#include <serialization/detail/config.h>

#include <serialization/detail/rapidjson/rapidjson.h>
#include <serialization/detail/rapidjson/document.h>
#include <serialization/detail/rapidjson/writer.h>

#include <type_traits>

namespace serialization
{

class json_oarchive
{
public:
    json_oarchive()
    {
    }

    const char * data() const
    {
        return buffer_.GetString();
    }

    size_t size() const
    {
        return buffer_.GetSize();
    }

    void save_key_start(const char * key)
    {
        serialization_trace trace(__func__, key);
        writer_.Key(key);
    }

    void save_key_end(const char * key)
    {
        serialization_trace trace(__func__, key);
    }

    void save_object_start()
    {
        serialization_trace trace(__func__, "json");
        writer_.StartObject();
    }

    void save_object_end()
    {
        serialization_trace trace(__func__, "json");
        writer_.EndObject();
    }

    void save_sequence_start(std::size_t size)
    {
        serialization_trace trace(__func__, "json");
        writer_.StartArray();
    }

    void save_sequence_end()
    {
        serialization_trace trace(__func__, "json");
        writer_.EndArray();
    }

    void save_sequence_item_start()
    {
    }

    void save_sequence_item_end()
    {
    }

    void save(bool v)
    {
        writer_.Bool(v);
	}

	void save(int v)
	{
		writer_.Int(v);
	}

	void save(uint v)
	{
		writer_.Uint(v);
	}

	void save(int64_t v)
	{
		writer_.Int64(v);
	}

	void save(uint64_t v)
	{
		writer_.Uint64(v);
	}

	void save(double v)
    {
        writer_.Double(v);
    }

    void save(const std::string & v)
    {
        writer_.String(v.c_str(), v.length());
    }

    void serialize_start()
    {
        serialization_trace trace(__func__, "json");
        buffer_.Clear();
        writer_.Reset(buffer_);
    }

    void serialize_end()
    {
        serialization_trace trace(__func__, "json");
    }

private:
    rapidjson::StringBuffer buffer_;

    rapidjson::Writer<rapidjson::StringBuffer> writer_;
};

} // namespace serialization
