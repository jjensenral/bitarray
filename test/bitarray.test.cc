// Testing bitarray.hh
// This test file is generated with mktest.lisp
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <span>
// guarded shift tests the underlying shift implementation, that it does not alter memory outside of the words it shifts
bool guarded_shift(std::initializer_list<u_int32_t> const &, signed int shift, std::initializer_list<u_int32_t> const &);
#include "bitarray.hh"
using namespace bitarrays;
TEST_CASE( "constructor", "[constructor]" ) {
    REQUIRE( bitarray<1>{0x1234567} == bitarray<1>{0x01234567} ); //leading zero
    REQUIRE( bitarray<3>{0x1234567} == bitarray<3>{0x00000000,0x00000000,0x01234567} ); //short number
    REQUIRE( bitarray<3>{0xff001122,0x33445566,0x778899aa,0xbbccddee} == bitarray<3>{0x33445566,0x778899aa,0xbbccddee} ); //trunc lead digits
} // constructor
TEST_CASE( "guard left shift", "[guard left shift]" ) {
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},0,{0x8AA0E223,0x1256AFFE,0x10F9E3A1}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},1,{0x1541C446,0x24AD5FFC,0x21F3C742}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},2,{0x2A83888C,0x495ABFF8,0x43E78E84}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},3,{0x55071118,0x92B57FF0,0x87CF1D08}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},23,{0x11892B57,0xFF087CF1,0xD0800000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},32,{0x1256AFFE,0x10F9E3A1,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},33,{0x24AD5FFC,0x21F3C742,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},34,{0x495ABFF8,0x43E78E84,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},60,{0xE10F9E3A,0x10000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},64,{0x10F9E3A1,0x00000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},65,{0x21F3C742,0x00000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},66,{0x43E78E84,0x00000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},95,{0x80000000,0x00000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},96,{0x00000000,0x00000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},100,{0x00000000,0x00000000,0x00000000}) );
} // guard left shift
TEST_CASE( "left shift", "[left shift]" ) {
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 0 ) == bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 1 ) == bitarray<3>{0x1541C446,0x24AD5FFC,0x21F3C742} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 2 ) == bitarray<3>{0x2A83888C,0x495ABFF8,0x43E78E84} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 3 ) == bitarray<3>{0x55071118,0x92B57FF0,0x87CF1D08} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 23 ) == bitarray<3>{0x11892B57,0xFF087CF1,0xD0800000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 32 ) == bitarray<3>{0x1256AFFE,0x10F9E3A1,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 33 ) == bitarray<3>{0x24AD5FFC,0x21F3C742,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 34 ) == bitarray<3>{0x495ABFF8,0x43E78E84,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 60 ) == bitarray<3>{0xE10F9E3A,0x10000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 64 ) == bitarray<3>{0x10F9E3A1,0x00000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 65 ) == bitarray<3>{0x21F3C742,0x00000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 66 ) == bitarray<3>{0x43E78E84,0x00000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 95 ) == bitarray<3>{0x80000000,0x00000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 96 ) == bitarray<3>{0x00000000,0x00000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} << 100 ) == bitarray<3>{0x00000000,0x00000000,0x00000000} );
} // left shift
TEST_CASE( "guard right shift", "[guard right shift]" ) {
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},0,{0x8AA0E223,0x1256AFFE,0x10F9E3A1}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-1,{0x45507111,0x892B57FF,0x087CF1D0}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-2,{0x22A83888,0xC495ABFF,0x843E78E8}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-3,{0x11541C44,0x624AD5FF,0xC21F3C74}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-23,{0x00000115,0x41C44624,0xAD5FFC21}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-32,{0x00000000,0x8AA0E223,0x1256AFFE}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-33,{0x00000000,0x45507111,0x892B57FF}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-34,{0x00000000,0x22A83888,0xC495ABFF}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-60,{0x00000000,0x00000008,0xAA0E2231}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-64,{0x00000000,0x00000000,0x8AA0E223}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-65,{0x00000000,0x00000000,0x45507111}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-66,{0x00000000,0x00000000,0x22A83888}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-95,{0x00000000,0x00000000,0x00000001}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-96,{0x00000000,0x00000000,0x00000000}) );
    REQUIRE( guarded_shift({0x8AA0E223,0x1256AFFE,0x10F9E3A1},-100,{0x00000000,0x00000000,0x00000000}) );
} // guard right shift
TEST_CASE( "right shift", "[right shift]" ) {
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 0 ) == bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 1 ) == bitarray<3>{0x45507111,0x892B57FF,0x087CF1D0} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 2 ) == bitarray<3>{0x22A83888,0xC495ABFF,0x843E78E8} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 3 ) == bitarray<3>{0x11541C44,0x624AD5FF,0xC21F3C74} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 23 ) == bitarray<3>{0x00000115,0x41C44624,0xAD5FFC21} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 32 ) == bitarray<3>{0x00000000,0x8AA0E223,0x1256AFFE} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 33 ) == bitarray<3>{0x00000000,0x45507111,0x892B57FF} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 34 ) == bitarray<3>{0x00000000,0x22A83888,0xC495ABFF} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 60 ) == bitarray<3>{0x00000000,0x00000008,0xAA0E2231} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 64 ) == bitarray<3>{0x00000000,0x00000000,0x8AA0E223} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 65 ) == bitarray<3>{0x00000000,0x00000000,0x45507111} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 66 ) == bitarray<3>{0x00000000,0x00000000,0x22A83888} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 95 ) == bitarray<3>{0x00000000,0x00000000,0x00000001} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 96 ) == bitarray<3>{0x00000000,0x00000000,0x00000000} );
    REQUIRE( (bitarray<3>{0x8AA0E223,0x1256AFFE,0x10F9E3A1} >> 100 ) == bitarray<3>{0x00000000,0x00000000,0x00000000} );
} // right shift
TEST_CASE( "and or xor", "[and or xor]" ) {
    REQUIRE( (bitarray<12>{0xD091BB5C,0x22AE9EF6,0xE7E1FAEE,0xD5C31F79,0x2082352C,0xF807B7DF,0xE9D30005,0x3895AFE1,0xA1E24BBA,0x4EE4092B,0x18F86863,0x8C16A625} & bitarray<12>{0x474BA8C4,0x3039CD1A,0x8C006D5F,0xFE2D7810,0xF51F2AE7,0xFF1816E4,0xF702EF59,0xF7BADAFA,0x285954A1,0xB9D09511,0xF878C4B3,0xFB2A0137}) == bitarray<12>{0x4001A844,0x20288C12,0x8400684E,0xD4011810,0x20022024,0xF80016C4,0xE1020001,0x30908AE0,0x204040A0,0x08C00101,0x18784023,0x88020025} );
    REQUIRE( (bitarray<12>{0xD091BB5C,0x22AE9EF6,0xE7E1FAEE,0xD5C31F79,0x2082352C,0xF807B7DF,0xE9D30005,0x3895AFE1,0xA1E24BBA,0x4EE4092B,0x18F86863,0x8C16A625} | bitarray<12>{0x474BA8C4,0x3039CD1A,0x8C006D5F,0xFE2D7810,0xF51F2AE7,0xFF1816E4,0xF702EF59,0xF7BADAFA,0x285954A1,0xB9D09511,0xF878C4B3,0xFB2A0137}) == bitarray<12>{0xD7DBBBDC,0x32BFDFFE,0xEFE1FFFF,0xFFEF7F79,0xF59F3FEF,0xFF1FB7FF,0xFFD3EF5D,0xFFBFFFFB,0xA9FB5FBB,0xFFF49D3B,0xF8F8ECF3,0xFF3EA737} );
    REQUIRE( (bitarray<12>{0xD091BB5C,0x22AE9EF6,0xE7E1FAEE,0xD5C31F79,0x2082352C,0xF807B7DF,0xE9D30005,0x3895AFE1,0xA1E24BBA,0x4EE4092B,0x18F86863,0x8C16A625} ^ bitarray<12>{0x474BA8C4,0x3039CD1A,0x8C006D5F,0xFE2D7810,0xF51F2AE7,0xFF1816E4,0xF702EF59,0xF7BADAFA,0x285954A1,0xB9D09511,0xF878C4B3,0xFB2A0137}) == bitarray<12>{0x97DA1398,0x129753EC,0x6BE197B1,0x2BEE6769,0xD59D1FCB,0x071FA13B,0x1ED1EF5C,0xCF2F751B,0x89BB1F1B,0xF7349C3A,0xE080ACD0,0x773CA712} );
} // and or xor
    bitarray<7> fred;
TEST_CASE( "one bit", "[one bit]" ) {
    REQUIRE( fred.is_zero() );
    fred.bitset(0);
    REQUIRE( !fred.is_zero() );
    for(int i = 0; i < 7*32-1; ++i) {
	REQUIRE( fred.lsb() == i );
	REQUIRE( fred.msb() == i );
	fred <<= 1;
	REQUIRE( !fred.is_zero() );
    }
    fred.bitclr(7*32-1);
    REQUIRE( fred.is_zero() );
} // one bit
TEST_CASE( "bit test", "[bit test]" ) {
    fred.zero();
    REQUIRE( fred.is_zero() );
    for(int i = 0; i < 7*32; ++i) {
	REQUIRE( fred.bit(i) == 0 );
	fred.bitset(i);
	REQUIRE( fred.bit(i) == 1 );
	fred.bitclr(i);
	REQUIRE( fred.bit(i) == 0 );
    }
    REQUIRE( fred.is_zero() );
} // bit test

bool guarded_shift(std::initializer_list<u_int32_t> const &mydata, signed int shift, std::initializer_list<u_int32_t> const &mytarget)
{
    std::span data(mydata), target(mytarget);
    std::array<u_int32_t, 5> in, out;
    const u_int32_t lo_guard = 0xa0ef09af;
    const u_int32_t hi_guard = 0xaee91fe9;
    /* Note the initializer has msw first, lsw last, so we fill the sandwich values backwards */
    in[0] = lo_guard; in[1] = data[2]; in[2] = data[1]; in[3] = data[0]; in[4] = hi_guard;
    out[0] = lo_guard; out[1] = target[2]; out[2] = target[1]; out[3] = target[0]; out[4] = hi_guard;
    std::span shiftme{in.begin()+1, 3};
    if(shift < 0)
	_private::bitarray_do_shift_right(shiftme, -shift);
    else
	_private::bitarray_do_shift_left(shiftme, shift);
    return in == out;
}
 // End of bitarray.test.cc

