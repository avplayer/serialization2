#pragma once

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
#define __WINDOWS__
#endif

#if defined(__linux__) || defined(__CYGWIN__)

#	include <endian.h>

#elif defined(__APPLE__)

#	include <libkern/OSByteOrder.h>

#	define htobe16(x) OSSwapHostToBigInt16(x)
#	define htole16(x) OSSwapHostToLittleInt16(x)
#	define be16toh(x) OSSwapBigToHostInt16(x)
#	define le16toh(x) OSSwapLittleToHostInt16(x)

#	define htobe32(x) OSSwapHostToBigInt32(x)
#	define htole32(x) OSSwapHostToLittleInt32(x)
#	define be32toh(x) OSSwapBigToHostInt32(x)
#	define le32toh(x) OSSwapLittleToHostInt32(x)

#	define htobe64(x) OSSwapHostToBigInt64(x)
#	define htole64(x) OSSwapHostToLittleInt64(x)
#	define be64toh(x) OSSwapBigToHostInt64(x)
#	define le64toh(x) OSSwapLittleToHostInt64(x)

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN

#elif defined(__OpenBSD__)

#	include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#	include <sys/endian.h>

#	define be16toh(x) betoh16(x)
#	define le16toh(x) letoh16(x)

#	define be32toh(x) betoh32(x)
#	define le32toh(x) letoh32(x)

#	define be64toh(x) betoh64(x)
#	define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)

#	include <winsock2.h>

#	if !defined(_MSC_VER)
#		include <sys/param.h>
#	endif // _MSC_VER

#	if BYTE_ORDER == LITTLE_ENDIAN

#		define htobe16(x) htons(x)
#		define htole16(x) (x)
#		define be16toh(x) ntohs(x)
#		define le16toh(x) (x)

#		define htobe32(x) htonl(x)
#		define htole32(x) (x)
#		define be32toh(x) ntohl(x)
#		define le32toh(x) (x)

#		define htobe64(x) htonll(x)
#		define htole64(x) (x)
#		define be64toh(x) ntohll(x)
#		define le64toh(x) (x)

#	elif BYTE_ORDER == BIG_ENDIAN

        /* that would be xbox 360 */
#		define htobe16(x) (x)
#		define htole16(x) __builtin_bswap16(x)
#		define be16toh(x) (x)
#		define le16toh(x) __builtin_bswap16(x)

#		define htobe32(x) (x)
#		define htole32(x) __builtin_bswap32(x)
#		define be32toh(x) (x)
#		define le32toh(x) __builtin_bswap32(x)

#		define htobe64(x) (x)
#		define htole64(x) __builtin_bswap64(x)
#		define be64toh(x) (x)
#		define le64toh(x) __builtin_bswap64(x)

#	else

#		error byte order not supported

#	endif

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN

#else

#	error platform not supported

#endif

namespace serialization
{
namespace detail
{

template<size_t size>
struct endian
{
    template<typename T>
    inline static T htobe(T t)
    {
        return t;
    }

    template<typename T>
    inline static T betoh(T t)
    {
        return t;
    }

    template<typename T>
    inline static T htole(T t)
    {
        return t;
    }

    template<typename T>
    inline static T letoh(T t)
    {
        return t;
    }
};

template<>
struct endian<2>
{
    template<typename T>
    inline static T htobe(T t)
    {
        return htobe16(t);
    }

    template<typename T>
    inline static T betoh(T t)
    {
        return be16toh(t);
    }

    template<typename T>
    inline static T htole(T t)
    {
        return htole16(t);
    }

    template<typename T>
    inline static T letoh(T t)
    {
        return le16toh(t);
    }
};

template<>
struct endian<4>
{
    template<typename T>
    inline static T htobe(T t)
    {
        return htobe32(t);
    }

    template<typename T>
    inline static T betoh(T t)
    {
        return be32toh(t);
    }

    template<typename T>
    inline static T htole(T t)
    {
        return htole32(t);
    }

    template<typename T>
    inline static T letoh(T t)
    {
        return le32toh(t);
    }
};

template<>
struct endian<8>
{
    template<typename T>
    inline static T htobe(T t)
    {
        return htobe64(t);
    }

    template<typename T>
    inline static T betoh(T t)
    {
        return be64toh(t);
    }

    template<typename T>
    inline static T htole(T t)
    {
        return htole64(t);
    }

    template<typename T>
    inline static T letoh(T t)
    {
        return le64toh(t);
    }
};

} // namespace detail

struct endian
{
    template<typename T>
    inline static T betoh(T t)
    {
        return detail::endian<sizeof(t)>::betoh(t);
    }

    template<typename T>
    inline static T htobe(T t)
    {
        return detail::endian<sizeof(t)>::htobe(t);
    }

    template<typename T>
    inline static T htole(T t)
    {
        return detail::endian<sizeof(t)>::htole(t);
    }

    template<typename T>
    inline static T letoh(T t)
    {
        return detail::endian<sizeof(t)>::letoh(t);
    }
};

} // namespace serialization
