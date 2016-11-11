#pragma once

#include <vector>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cassert>
#if __cplusplus > 201402L
#include <optional>
#endif

#include <stdint.h>
#include <stddef.h>

#include <serialization/detail/serialization_helper.h>
#include <serialization/detail/endian.h>
#include <serialization/detail/byte_order.h>

namespace serialization
{

class binary_iarchive
{
public:
    binary_iarchive(const uint8_t* data, size_t size)
        : data_(data, data+size)
		, index_(0)
    {
    }

    template<typename Argument>
    inline void load_start(Argument & arg) const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
    }

    template<typename Argument>
    inline void load_end(Argument & arg) const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
    }

    template<typename T>
    void load(T & out) const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        if(!can_read(sizeof(out)))
		  throw_serialization_error("load", "data size too short");
        const T * p = reinterpret_cast<const T *>(&data_[index_]);
		out = *p;
        index_ += sizeof(out);
    }

    void load(std::string & out) const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        size_type size;
        load(size);
        if(!can_read(size))
		  throw_serialization_error("load", "data size too short");
        out = std::string(reinterpret_cast<const char *>(&data_[index_]), size);
        index_ += size;
    }

#if __cplusplus > 201402L
	template<typename T>
	void load(std::optional<T> & out) const
	{
		std::cout << "load optional" << std::endl;
		serialization_trace trace(__func__, __FILE__, __LINE__);
		uint8_t hasValue;
		load(hasValue);
		if(hasValue)
		{
			T value;
			load(value);
			out = value;
		}
	}
#endif

    void unserialize_start() const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        index_ = 0;
        size_type size;
        load(size);
        if(size != data_.size())
		  throw_serialization_error("unserialize start", "size error");
    }

    void unserialize_end() const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
    }

private:
    bool can_read(size_t size) const
    {
        return index_ + size <= data_.size();
    }

    std::vector<uint8_t> data_;

    mutable size_t index_;
};

} // namespace serialization
