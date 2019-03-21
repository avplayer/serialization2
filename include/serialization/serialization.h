#pragma once

#include <serialization/detail/meta.h>
#include <serialization/detail/serialization_error.h>
#include <serialization/detail/serialization_unpack.h>
#include <serialization/detail/binary_iarchive.h>
#include <serialization/detail/binary_oarchive.h>
#include <serialization/detail/xml_iarchive.h>
#include <serialization/detail/xml_oarchive.h>
#include <serialization/detail/config.h>
#include <serialization/detail/json_iarchive.h>
#include <serialization/detail/json_oarchive.h>
#include <iostream>


#define SERIALIZATION_DEFINE_META_FUNC(...) \
template<typename Archive> \
void serialize_impl(Archive & ar) const \
{ \
    serialization::serialization_trace trace(__func__, __FILE__, __LINE__); \
    static_assert(std::decay<decltype(serialization_meta())>::type().size() == decltype(serialization::detail::make_argument_count(__VA_ARGS__))::value , "serialization meta data size error"); \
    serialization::serialize_unpack(serialization_meta().begin(), ar, ##__VA_ARGS__); \
} \
\
template<typename Archive> \
void unserialize_impl(const Archive & ar) \
{ \
    serialization::serialization_trace trace(__func__, __FILE__, __LINE__); \
    static_assert(std::decay<decltype(serialization_meta())>::type().size() == decltype(serialization::detail::make_argument_count(__VA_ARGS__))::value , "serialization meta data size error"); \
    serialization::unserialize_unpack(serialization_meta().begin(), ar, ##__VA_ARGS__);\
}

#define SERIALIZATION_DEFINE(...) \
    SERIALIZATION_DEFINE_META_DATA_HELPER(__VA_ARGS__) \
    SERIALIZATION_DEFINE_META_FUNC(__VA_ARGS__)

namespace serialization
{

template <typename Archive, typename Value>
void serialize(Archive & ar, const Value & v)
{
    ar.serialize_start();
    serialization::serialize_helper(ar, v);
    ar.serialize_end();
}

template <typename Archive, typename Value>
void unserialize(const Archive & ar, Value & v)
{
    ar.unserialize_start();
    serialization::unserialize_helper(ar, v);
    ar.unserialize_end();
}

} // namespace serialization

