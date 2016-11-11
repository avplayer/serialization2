#pragma once

#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cassert>

#include <serialization/detail/endian.h>
#include <serialization/detail/config.h>

namespace serialization
{

class binary_oarchive
{
    typedef std::vector<uint8_t>::const_iterator const_iterator;

public:
    binary_oarchive()
    {
    }

    template<typename Argument>
    void save_start(Argument & arg) const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
    }

    template<typename Argument>
    void save_end(Argument & arg) const
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
    }

    template<typename T>
    void save(const T & v)
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        size_t size = data_.size();
        data_.resize(size + sizeof(v));
        save_at(v, size);
    }

    void save(const std::string & v)
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        size_type size = v.size();
        save(size);
        save_raw(v.data(), v.size());
    }

#if __cplusplus > 201402L
    template<typename T>
    void save(const std::optional<T> & v)
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
		uint8_t hasValue = v ? 1 : 0;
		save(hasValue);
		if(hasValue)
		{
			save(*v);
		}
	}
#endif

    void serialize_start()
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        data_.clear();
        size_type size = 0;
        save(size);
    }

    void serialize_end()
    {
		serialization_trace trace(__func__, __FILE__, __LINE__);
        size_type size = data_.size();
        save_at(size, 0);
    }

    const uint8_t * data() const
    {
        return data_.data();
    }

    size_t size() const
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

private:
    template<typename T>
    void save_at(const T & v, size_t offset)
    {
		T val = (v);
        const uint8_t * p = reinterpret_cast<const uint8_t *>(&val);
        assert(data_.size() >= offset + sizeof(val));
        std::copy(p, p + sizeof(val), data_.begin() + offset);
    }

    void save_raw(const char * data, size_t size)
    {
        data_.reserve(data_.size() + size);
        data_.insert(data_.end(), data, data+size);
    }

    std::vector<uint8_t> data_;
};

} // namespace serialization
