#pragma once

#include "serialization_helper.h"

#if __cplusplus > 201402L
#include <optional>
#endif

namespace serialization
{

template<typename Argument, typename Archive>
inline void serialize_unpack(Argument && arg, Archive& ar)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
}

template<typename Argument, typename Archive, typename T, typename ...Args>
inline void serialize_unpack(Argument && arg, Archive & ar, T && front, Args && ... next)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);

    auto key = *arg;
    ar.save_key_start(key);
    serialize_helper(ar, front);
    ar.save_key_end(key);

    serialize_unpack(++arg, ar, std::forward<Args>(next)...);
}

#if __cplusplus > 201402L
// optional
template<typename Argument, typename Archive, typename T, typename ...Args>
inline void serialize_unpack(Argument && arg, Archive & ar, const std::optional<T> & front, Args && ... next)
{
    serialization_trace trace(__func__, __FILE__, __LINE__);
    if(front)
    {
        auto key = *arg;
        ar.save_key_start(key);
        serialize_helper(ar, *front);
        ar.save_key_end(key);
    }
    serialize_unpack(++arg, ar, std::forward<Args>(next)...);
}
#endif

template<typename Argument, typename Archive>
inline void unserialize_unpack(Argument && arg, const Archive& ar)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);
}

template<typename Argument, typename Archive, typename T, typename ...Args>
inline void unserialize_unpack(Argument && arg, const Archive & ar, T && front, Args&& ... next)
{
	serialization_trace trace(__func__, __FILE__, __LINE__);

    auto key = *arg;
    ar.load_key_start(key);
    unserialize_helper(ar, front);
    ar.load_key_end(key);

    unserialize_unpack(++arg, ar, std::forward<Args>(next)...);
}

#if __cplusplus > 201402L
// optional
template<typename Argument, typename Archive, typename T, typename ...Args>
inline void unserialize_unpack(Argument && arg, const Archive & ar, std::optional<T> & front, Args&& ... next)
{
    serialization_trace trace(__func__, __FILE__, __LINE__);

    auto key = *arg;

    if(ar.load_key_start_optional(key))
    {
        T tmp;
        unserialize_helper(ar, tmp);
        front.emplace(std::move(tmp));
        ar.load_key_end(key);
    }

    unserialize_unpack(++arg, ar, std::forward<Args>(next)...);
}
#endif

} //namespace serialization

