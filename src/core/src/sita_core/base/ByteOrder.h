#pragma once

#include "sita_base.h"

namespace sita {

class ByteOrder {
public:
	
	class Straight {
	public:
		constexpr static u8  get(u8  v) { return v; }
		constexpr static u16 get(u16 v) { return v; }
		constexpr static u32 get(u32 v) { return v; }
		constexpr static u64 get(u64 v) { return v; }

		constexpr static i8  get(i8  v) { return v; }
		constexpr static i16 get(i16 v) { return v; }
		constexpr static i32 get(i32 v) { return v; }
		constexpr static i64 get(i64 v) { return v; }
	};

	class ByteSwap {
	public:
		constexpr static u8  get(u8  v) { return v; }
		constexpr static u16 get(u16 v) { return (u16)(v >> 8) | (v << 8); }
		constexpr static u32 get(u32 v) { 
			v = ((v<<8) & 0xFF00FF00UL) | ((v>>8) & 0x00FF00FFUL); 
			return (v>>16) | (v<<16);
		}
		constexpr static u64 get(u64 v) { 
			v = ((v<< 8) & 0xFF00FF00FF00FF00ULL) | ((v>> 8) & 0x00FF00FF00FF00FFULL);
			v = ((v<<16) & 0xFFFF0000FFFF0000ULL) | ((v>>16) & 0x0000FFFF0000FFFFULL);
			return (v>>32) | (v<<32);
		}

		constexpr static i8  get(i8  v) { return static_cast<i8 >(get(static_cast<u8 >(v))); }
		constexpr static i16 get(i16 v) { return static_cast<i16>(get(static_cast<u16>(v))); }
		constexpr static i32 get(i32 v) { return static_cast<i32>(get(static_cast<u32>(v))); }
		constexpr static i64 get(i64 v) { return static_cast<i64>(get(static_cast<u64>(v))); }
	};

#if SITA_CPU_ENDIAN_LITTLE
	using LEtoHost   = Straight;
	using LEfromHost = Straight;

	using BEtoHost   = ByteSwap;
	using BEfromHost = ByteSwap;
#endif

#if SITA_CPU_ENDIAN_BIG
	using LEtoHost   = ByteSwap;
	using LEfromHost = ByteSwap;

	using BEtoHost   = Straight;
	using BEfromHost = Straight;
#endif

};

} // namespace