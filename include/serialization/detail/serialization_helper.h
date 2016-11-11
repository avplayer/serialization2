#pragma once
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include "config.h"
#include "serialization_error.h"

namespace serialization
{

namespace detail
{

template<typename Archive, typename Object>
inline auto serialize_helper(Archive & ar, const Object & obj, int) -> decltype(obj.serialize_impl(ar), void())
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    ar.save_object_start();
	obj.serialize_impl(ar);
    ar.save_object_end();
}

template<typename Archive, typename Object>
inline auto serialize_helper(Archive & ar, const Object & obj, long) -> decltype(void())
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    ar.save(obj);
}

template<typename Archive, typename Object>
inline auto unserialize_helper(const Archive & ar, Object & obj, int) -> decltype(obj.unserialize_impl(ar), void())
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    ar.load_object_start();
    obj.unserialize_impl(ar);
    ar.load_object_end();
}

template<typename Archive, typename Object>
inline auto unserialize_helper(const Archive & ar, Object & obj, long) -> decltype(void())
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    ar.load(obj);
}

} // namespace detail

template<typename Archive, typename Object>
inline void serialize_helper(Archive & ar, const Object & obj)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    detail::serialize_helper(ar, obj, 0);
}

template<typename Archive, typename Object>
inline void unserialize_helper(const Archive & ar, Object & obj)
{
    serialization_trace trace(__func__, __FILE__, __LINE__);
    detail::unserialize_helper(ar, obj, 0);
}


// for container

template<typename Archive, typename T>
inline void serialize_container_helper(Archive & ar, const T & v)
{
    serialization_trace trace(__func__, __FILE__, __LINE__);

    ar.save_sequence_start();
    for(const auto & i : v)
    {
        serialize_helper(ar, i);
    }
    ar.save_sequence_end();
}

//pair
template<typename Archive, typename Key, typename Value>
inline void serialize_helper(Archive & ar, const std::pair<Key, Value> & in)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    ar.save_object_start();

    ar.save_key_start("first");
    serialize_helper(ar, in.first);
    ar.save_key_end("first");

    ar.save_key_start("second");
    serialize_helper(ar, in.second);
    ar.save_key_end("first");

    ar.save_object_end();
}

//list
template<typename Archive, typename T>
inline void serialize_helper(Archive & ar, const std::list<T> & v)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    serialize_container_helper(ar, v);
}

//vector
template<typename Archive, typename T>
inline void serialize_helper(Archive & ar, const std::vector<T> & v)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    serialize_container_helper(ar, v);
}

//array
template<typename Archive, typename T, size_t N>
inline void serialize_helper(Archive & ar, const T(&v)[N])
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    serialize_container_helper(ar, v);
}

//map
template<typename Archive, typename Key, typename Value>
inline void serialize_helper(Archive & ar, const std::map<Key, Value> & v)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
    serialize_container_helper(ar, v);
}

//pair
template<typename Archive, typename Key, typename Value>
inline void unserialize_helper(const Archive & ar, std::pair<Key, Value> & v)
{
    serialization_trace trace(__func__, __FILE__, __LINE__);
    ar.load_object_start();

    ar.load_key_start("first");
    unserialize_helper(ar, v.first);
    ar.load_key_end("first");

    ar.load_key_start("second");
    unserialize_helper(ar, v.second);
    ar.load_key_end("second");

    ar.load_object_end();
}

//list
template<typename Archive, typename T>
inline void unserialize_helper(const Archive & ar, std::list<T> & v)
{
    serialization_trace trace(__func__, __FILE__, __LINE__);

    for(size_type i = 0; ar.load_sequence_start(i); ++i)
    {
        T tmp;
        unserialize_helper(ar, tmp);
        v.push_back(std::move(tmp));
        ar.load_sequence_end();
    }
}

//vector
template<typename Archive, typename T>
inline void unserialize_helper(const Archive & ar, std::vector<T> & v)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);

    v.reserve(ar.sequence_size());

    for(size_type i = 0; ar.load_sequence_start(i); ++i)
    {
        T tmp;
        unserialize_helper(ar, tmp);
        v.push_back(std::move(tmp));
        ar.load_sequence_end();
    }
}

//array
template<typename Archive, typename T, size_t N>
inline void unserialize_helper(const Archive & ar, T (&v)[N])
{
	serialization_trace trace(__func__, __FILE__, __LINE__);

    if(ar.sequence_size() != N)
      throw_serialization_error("array", "size error");

    for(size_type i = 0; ar.load_sequence_start(i); ++i)
    {
        unserialize_helper(ar, v[i]);
        ar.load_sequence_end();
    }

}

//map
template<typename Archive, typename Key, typename Value>
inline void unserialize_helper(const Archive & ar, std::map<Key, Value> & v)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);

    for(size_type i = 0; ar.load_sequence_start(i); ++i)
    {
        typename std::pair<Key, Value> tmp;
        unserialize_helper(ar, tmp);
        *std::insert_iterator<std::map<Key, Value>>(v, v.end()) = std::move(tmp);
        ar.load_sequence_end();
    }

}

} // namespace serialization
