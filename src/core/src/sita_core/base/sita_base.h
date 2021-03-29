#pragma once

#include "../detect_platform/sita_detect_platform.h"

#if SITA_OS_WINDOWS
	#define NOMINMAX 1

	#include <WinSock2.h> // WinSock2.h must include before windows.h to avoid winsock1 define
	#include <ws2tcpip.h> // struct sockaddr_in6
	#pragma comment(lib, "Ws2_32.lib")

	#include <Windows.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/in.h> // struct sockaddr_in
#endif

#include <cassert>
#include <exception>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <string_view>

#include "sita_macro.h"

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// using f16 = half;
using f32 = float;
using f64 = double;
using f128 = long double;

template<class T> using Vector = std::vector<T>;

template<class T> inline constexpr typename std::underlying_type<T>::type         enum_int    (      T  value) { return       static_cast<      typename std::underlying_type<T>::type >( value); }
template<class T> inline constexpr typename std::underlying_type<T>::type       & enum_int_ref(      T& value) { return *reinterpret_cast<      typename std::underlying_type<T>::type*>(&value); }
template<class T> inline constexpr typename std::underlying_type<T>::type const & enum_int_ref(const T& value) { return *reinterpret_cast<const typename std::underlying_type<T>::type*>(&value); }

namespace sita {

//! Source Location
class SrcLoc {
public:
	SrcLoc() = default;
	SrcLoc(const char* file_, int line_, const char* func_) 
		: file(file_)
		, line(line_) 
		, func(func_)
	{}

	const char* file = "";
	const char* func;
	int line = 0;
};

class NonCopyable {
public:
	NonCopyable() = default;

private:
	NonCopyable(NonCopyable &&) {}

	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

} // namespace




inline uint8_t my_byte_swap(uint8_t v) {
	return v;
}

inline uint16_t my_byte_swap(uint16_t v) {
	union T {
		uint16_t value;
		uint8_t  c[2];
	};
	T src; src.value = v;
	T dst;
	dst.c[0] = src.c[1];
	dst.c[1] = src.c[0];
	return dst.value;
}

inline uint32_t my_byte_swap(uint32_t v) {
	union T {
		uint32_t value;
		uint8_t  c[4];
	};
	T src; src.value = v;
	T dst;
	dst.c[0] = src.c[3];
	dst.c[1] = src.c[2];
	dst.c[2] = src.c[1];
	dst.c[3] = src.c[0];
	return dst.value;
}

inline uint64_t my_byte_swap(uint64_t v) {
	union T {
		uint64_t value;
		uint8_t  c[8];
	};
	T src; src.value = v;
	T dst;
	dst.c[0] = src.c[7];
	dst.c[1] = src.c[6];
	dst.c[2] = src.c[5];
	dst.c[3] = src.c[4];
	dst.c[4] = src.c[3];
	dst.c[5] = src.c[2];
	dst.c[6] = src.c[1];
	dst.c[7] = src.c[0];
	return dst.value;
}

inline uint8_t my_zigzag_encode(int8_t  v) {
	const auto a = static_cast<uint8_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint8_t>(v >> 7); // ! must shift in signed
	return  a ^ b;
}

inline uint16_t my_zigzag_encode(int16_t  v) {
	const auto a = static_cast<uint16_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint16_t>(v >> 15); // ! must shift in signed
	return  a ^ b;
}

inline uint32_t my_zigzag_encode(int32_t  v) {
	const auto a = static_cast<uint32_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint32_t>(v >> 31); // ! must shift in signed
	return  a ^ b;
}

inline uint64_t my_zigzag_encode(int64_t  v) {
	const auto a = static_cast<uint64_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint64_t>(v >> 63); // ! must shift in signed
	return  a ^ b;
}

inline int8_t   my_zigzag_decode(uint8_t  v) { return static_cast<int8_t>(v >> 1) ^ -static_cast<int8_t>(v & 1); }
inline int16_t  my_zigzag_decode(uint16_t v) { return static_cast<int16_t>(v >> 1) ^ -static_cast<int16_t>(v & 1); }
inline int32_t  my_zigzag_decode(uint32_t v) { return static_cast<int32_t>(v >> 1) ^ -static_cast<int32_t>(v & 1); }
inline int64_t  my_zigzag_decode(uint64_t v) { return static_cast<int64_t>(v >> 1) ^ -static_cast<int64_t>(v & 1); }


#if 1 // byte swap needed

inline uint8_t  my_hton(uint8_t  v) { return my_byte_swap(v); }
inline uint16_t my_hton(uint16_t v) { return my_byte_swap(v); }
inline uint32_t my_hton(uint32_t v) { return my_byte_swap(v); }
inline uint64_t my_hton(uint64_t v) { return my_byte_swap(v); }

inline int8_t   my_hton(int8_t   v) { return (int8_t)my_byte_swap((uint8_t)v); }
inline int16_t  my_hton(int16_t  v) { return (int16_t)my_byte_swap((uint16_t)v); }
inline int32_t  my_hton(int32_t  v) { return (int32_t)my_byte_swap((uint32_t)v); }
inline int64_t  my_hton(int64_t  v) { return (int64_t)my_byte_swap((uint64_t)v); }

inline uint8_t  my_ntoh(uint8_t  v) { return my_byte_swap(v); }
inline uint16_t my_ntoh(uint16_t v) { return my_byte_swap(v); }
inline uint32_t my_ntoh(uint32_t v) { return my_byte_swap(v); }
inline uint64_t my_ntoh(uint64_t v) { return my_byte_swap(v); }

inline int8_t   my_ntoh(int8_t   v) { return (int8_t)my_byte_swap((uint8_t)v); }
inline int16_t  my_ntoh(int16_t  v) { return (int16_t)my_byte_swap((uint16_t)v); }
inline int32_t  my_ntoh(int32_t  v) { return (int32_t)my_byte_swap((uint32_t)v); }
inline int64_t  my_ntoh(int64_t  v) { return (int64_t)my_byte_swap((uint64_t)v); }

#else // byte swap not needed

inline uint8_t  my_hton(uint8_t  v) { return v; }
inline uint16_t my_hton(uint16_t v) { return v; }
inline uint32_t my_hton(uint32_t v) { return v; }
inline uint64_t my_hton(uint64_t v) { return v; }

inline int8_t   my_hton(int8_t   v) { return v; }
inline int16_t  my_hton(int16_t  v) { return v; }
inline int32_t  my_hton(int32_t  v) { return v; }
inline int64_t  my_hton(int64_t  v) { return v; }

inline uint8_t  my_ntoh(uint8_t  v) { return v; }
inline uint16_t my_ntoh(uint16_t v) { return v; }
inline uint32_t my_ntoh(uint32_t v) { return v; }
inline uint64_t my_ntoh(uint64_t v) { return v; }

inline int8_t   my_ntoh(int8_t   v) { return v; }
inline int16_t  my_ntoh(int16_t  v) { return v; }
inline int32_t  my_ntoh(int32_t  v) { return v; }
inline int64_t  my_ntoh(int64_t  v) { return v; }

#endif

inline float  my_hton(float  v) { auto t = my_hton(*(uint32_t*)&v); return *(float*)&t; }
inline double my_hton(double v) { auto t = my_hton(*(uint64_t*)&v); return *(double*)&t; }

inline float  my_ntoh(float  v) { auto t = my_ntoh(*(uint32_t*)&v); return *(float*)&t; }
inline double my_ntoh(double v) { auto t = my_ntoh(*(uint64_t*)&v); return *(double*)&t; }
