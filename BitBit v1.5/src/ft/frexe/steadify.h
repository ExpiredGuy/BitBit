#ifndef _STDENDIAN_H_
#define _STDENDIAN_H_

#include <stdint.h>

#if defined(__linux__) || defined(__GLIBC__) || defined(__CYGWIN__)
#include <endian.h>
#include <byteswap.h>
#define bswap16(x) bswap_16(x)
#define bswap32(x) bswap_32(x)
#define bswap64(x) bswap_64(x)
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)
#include <sys/endian.h>
#elif defined(_WIN32) || defined(_MSC_VER) || defined(__EMSCRIPTEN__)
#define _LITTLE_ENDIAN 1234
#define _BIG_ENDIAN 4321
#define _BYTE_ORDER _LITTLE_ENDIAN
#elif defined(__APPLE__)
#include <machine/endian.h>
#define _BYTE_ORDER BYTE_ORDER
#define _LITTLE_ENDIAN LITTLE_ENDIAN
#define _BIG_ENDIAN BIG_ENDIAN
#endif

#ifndef BYTE_ORDER
#define BYTE_ORDER _BYTE_ORDER
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN _LITTLE_ENDIAN
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN _BIG_ENDIAN
#endif

#if !defined(bswap16)
#if defined(__GNUC__)
#define bswap16(x) __builtin_bswap16(x)
#else
static inline uint16_t bswap16(uint16_t x) { return (x >> 8) | (x << 8); }
#endif
#endif

#if !defined(bswap32)
#if defined(__GNUC__)
#define bswap32(x) __builtin_bswap32(x)
#else
static inline uint32_t bswap32(uint32_t x) {
    return (x >> 24) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | (x << 24);
}
#endif
#endif

#if !defined(bswap64)
#if defined(__GNUC__)
#define bswap64(x) __builtin_bswap64(x)
#else
static inline uint64_t bswap64(uint64_t x) {
    return (x >> 56) | ((x >> 40) & 0x000000000000FF00ULL) | ((x >> 24) & 0x0000000000FF0000ULL) |
           ((x >> 8) & 0x00000000FF000000ULL) | ((x << 8) & 0x000000FF00000000ULL) |
           ((x << 24) & 0x0000FF0000000000ULL) | ((x << 40) & 0x00FF000000000000ULL) | (x << 56);
}
#endif
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#define htobe16(x) bswap16(x)
#define htole16(x) (x)
#define be16toh(x) bswap16(x)
#define le16toh(x) (x)
#define htobe32(x) bswap32(x)
#define htole32(x) (x)
#define be32toh(x) bswap32(x)
#define le32toh(x) (x)
#define htobe64(x) bswap64(x)
#define htole64(x) (x)
#define be64toh(x) bswap64(x)
#define le64toh(x) (x)
#else
#define htobe16(x) (x)
#define htole16(x) bswap16(x)
#define be16toh(x) (x)
#define le16toh(x) bswap16(x)
#define htobe32(x) (x)
#define htole32(x) bswap32(x)
#define be32toh(x) (x)
#define le32toh(x) bswap32(x)
#define htobe64(x) (x)
#define htole64(x) bswap64(x)
#define be64toh(x) (x)
#define le64toh(x) bswap64(x)
#endif

#endif
