#include <sita_core/base/UnitTest.h>
#include <sita_core/base/BinSerializer.h>
#include <sita_core/base/BinDeserializer.h>

namespace sita { namespace test {

class Test_BinSerializer : public UnitTestBase {
public:

	template<class T>
	void test_fixed(u64 n, u64 step) {
		Vector<u8> buf;
		{
			BinSerializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t = static_cast<T>(i);
				se.io_fixed(t);
			}
		}

//		SITA_DUMP_HEX(buf);

		{
			BinDeserializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t;
				se.io_fixed(t);
				const T expected = static_cast<T>(i);
				SITA_TEST_CHECK_SLIENT(t == expected);
			}
		}
	}

	template<class T>
	void test_vary(u64 n, u64 step) {
		Vector<u8> buf;
		{
			BinSerializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t = static_cast<T>(i);
				se.io_vary(t);
			}
		}

//		SITA_DUMP_HEX(buf);

		{
			BinDeserializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t;
				se.io_vary(t);
				const T expected = static_cast<T>(i);
				SITA_TEST_CHECK_SLIENT(t == expected);
			}
		}
	}

	template<class T>
	void test_float(u64 n, T step) {
		Vector<u8> buf;
		{
			BinSerializer se(buf);
			for (u64 i = 0; i < n; i++) {
				T t = static_cast<T>(i) * step;
				se.io(t);
			}
		}

//		SITA_DUMP_HEX(buf);

		{
			BinDeserializer se(buf);
			for (u64 i = 0; i < n; i++) {
				T t;
				se.io(t);
				const T expected = static_cast<T>(i) * step;
//				SITA_DUMP_VAR(t, expected);
				SITA_TEST_CHECK_SLIENT(t == expected);
			}
		}
	}

	class MyData {
	public:
		int i = 0;
		float f = 0;

		template<class SE>
		void io(SE& se) {
			se << i << f;
		}
	};

	void test_class() {
		Vector<u8> buf;

		MyData src;
		src.i = 100;
		src.f = 0.2f;

		BinSerializer(buf) << src;
		SITA_DUMP_HEX(buf);

		MyData dst;
		BinDeserializer(buf) << dst;

		SITA_TEST_CHECK_SLIENT(src.i == dst.i);
		SITA_TEST_CHECK_SLIENT(src.f == dst.f);
	};
};

}} // namespace 

void test_BinSerializer() {
	SITA_TEST_CASE(Test_BinSerializer, test_class());

#if 1
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u64>(0xFFFFFFFFFFFFFFFFULL, 1));

	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i64>(0xFFFFFFFFFFFFFFFFULL, 1));
#endif

#if 1
	SITA_TEST_CASE(Test_BinSerializer, test_vary<u8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<u16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<u32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_vary<u64>(0xFFFFFFFFFFFFFFFFULL, 1));

	SITA_TEST_CASE(Test_BinSerializer, test_vary<i8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<i16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<i32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_vary<i64>(0xFFFFFFFFFFFFFFFFULL, 1));
#endif

#if 1
	SITA_TEST_CASE(Test_BinSerializer, test_float<f32>(0x100000,       110.3f));
	SITA_TEST_CASE(Test_BinSerializer, test_float<f64>(0x100000,       110.3 ));

	SITA_TEST_CASE(Test_BinSerializer, test_float<f32>(0x100000,       0.3f));
	SITA_TEST_CASE(Test_BinSerializer, test_float<f64>(0x100000,       0.3 ));
#endif
}
