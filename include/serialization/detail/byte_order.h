#include <serialization/detail/endian.h>
#include <serialization/detail/config.h>

namespace serialization
{

template<typename T>
inline T ntoh(const T & v)
{
#if defined(USE_LITTLE_ENDIAN)
	return endian::letoh(v);
#elif defined(USE_BIG_ENDIAN)
	return endian::betoh(v);
#else
	return v;
#endif
}

template<typename T>
inline T hton(const T & v)
{
#if defined(USE_LITTLE_ENDIAN)
	return endian::htole(v);
#elif defined(USE_BIG_ENDIAN)
	return endian::htobe(v);
#else
	return v;
#endif
}

}
