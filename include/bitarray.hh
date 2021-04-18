#ifndef __LIBPIECES_BITARRAY_HH
#define __LIBPIECES_BITARRAY_HH

#include <array>
#include <cstdlib>
#include <iosfwd>
#include <span>
#include <algorithm>
#include <cstddef>

// the name has to be different from the class...
namespace bitarrays {

    namespace _private {
    /* Not part of the API; this is the implementation */
        void bitarray_do_shift_left(std::span < u_int32_t > const &, size_t) noexcept;
        void bitarray_do_shift_right(std::span < u_int32_t > const &, size_t) noexcept;
        void bitarray_do_fill_from(size_t, std::span<const u_int32_t> const &, std::span <u_int32_t> const &) noexcept;
        size_t bitarray_do_popcount(std::span<const u_int32_t> const &) noexcept;
        int16_t bitarray_do_lsb(std::span<const u_int32_t> const &) noexcept;
        int16_t bitarray_do_msb(std::span<const u_int32_t> const &) noexcept;
        void bitarray_do_ostream(std::ostream &, std::span<const u_int32_t> const &);
        void bitarray_do_istream(std::ostream &, std::span <u_int32_t> const &);
    }


/** Bitarray - a semi-optimised/optimisable POD bit vector of fixed length.
 *
 * Note that no array bounds are checked.
 *
 * Note there is no copying from a similar structure with a different size.
 *
 * Indexing the bits relies on 15 bit integers in this implementation, so the maximal bit size is 32767, and the largest
 * value of SIZE in the type is 1023.
 *
 * Implementation note: it is built as an array of 32 bit integers, so relies on dividing an index into (in binary)
 * iiiiiiiiiiibbbbb where the upper i's are the index into the array and the lower b's the bit index in the integer.  In
 * particular the least significant uint32 is stored first.
 *
 * Using a template is a somewhat annoying way of specifying the constant size.
 *
 * Other implementations: Qt:QBitArray, boost:dynamic_bitset, sgi::bitset, std::vector<bool>
 */

template<size_t SIZE>
requires( SIZE < 1024 && SIZE > 0 )
class bitarray {

    //    std::array<u_int32_t, ((SIZE + 0x1f) >>5)> data_;
    std::array<u_int32_t, SIZE> data_;

public:

    using uint = u_int32_t;
    static constexpr size_t wordsize = 32;

    static constexpr size_t lobits(size_t k) noexcept { return k & 0x1f; }
    static constexpr size_t hibits(size_t k) noexcept { return k >> 5; }
    static constexpr size_t tohibits(size_t k) noexcept { return k << 5; }

    constexpr zero() noexcept
    {
        std::fill(data_.begin(), data_.end(), 0);
    }
    constexpr bitarray() noexcept {
	    zero();
    }

    /** Initialise from unsigned integers */
    constexpr explicit bitarray(explicit u_int8_t b) noexcept { zero(); data_[0] = b; }
    constexpr explicit bitarray(explicit u_int16_t w) noexcept { zero(); data_[0] = w; }
    constexpr explicit bitarray(explicit u_int32_t d) noexcept { zero(); data_[0] = d; }
    constexpr explicit bitarray(explicit u_int64_t q) noexcept
    {
        zero();
        data_[0] = static_cast<u_int32_t>(q);
        if constexpr(SIZE > 1)
            data_[1] = static_cast<u_int32_t>(q >> 32);
    }

    /** Initialise from list, least signficiant dword first.
     * The bit array is zero extended if initializer_list is shorter; extra values are ignored if it is longer.
     */
    constexpr bitarray(std::initializer_list<u_int32_t> const &vals) noexcept
    {
	_private::bitarray_do_fill_from( SIZE, vals, data_ );
    }

    /** Equality */
    constexpr bool operator==(bitarray<SIZE> const &other) const noexcept { return data_ == other.data_; }

    /** Test bit */
    [[nodiscard]] constexpr bool bit( size_t k ) const noexcept { return (data_[hibits(k)] & 1U << lobits(k)) != 0; }
    /** Set bit to 1 */
    constexpr void bitset( size_t k ) noexcept { data_[hibits(k)] |= (1 << lobits(k)); }
    /** Set bit to 0 */
    constexpr void bitclr( size_t k ) noexcept { data_[hibits(k)] &= ~(1 << lobits(k)); }

    /** Return true if all the bits in the other are set in *this */
    constexpr bool contains(bitarray const &o) const noexcept
    {
	for(size_t i = 0; i < SIZE; ++i)
	    if((o.data_[i] & ~data_[i]) != 0) return false;
	return true;
    }

    /** Index of least significant bit or -1 if the array is zero */
    [[nodiscard]] constexpr int16_t lsb() const noexcept { return _private::bitarray_do_lsb(data_); }

    /** Index of least significant bit or -1 if the array is zero */
    [[nodiscard]] constexpr int16_t msb() const noexcept { return _private::bitarray_do_msb(data_); }

    /** Is zero? */
    [[nodiscard]] constexpr bool is_zero() const noexcept { return std::all_of( data_.cbegin(), data_.cend(), [](auto x) { return x == 0; } ); }

    /** Left bit shift; returns self */
    bitarray &operator<<=(size_t k) noexcept { _private::bitarray_do_shift_left(data_, k); return *this; }
    /** Right (logical) bit shift; returns self */
    bitarray &operator>>=(size_t k) noexcept { _private::bitarray_do_shift_right(data_, k); return *this; };

    // CTAD in action...
    constexpr bitarray operator<<(size_t k) { bitarray m(*this); m <<= k; return m; }
    constexpr bitarray operator>>(size_t k) { bitarray m(*this); m >>= k; return m; }

    /** logical or */
    bitarray &operator|=(bitarray<SIZE> const &other) noexcept { std::transform( data_.begin(), data_.end(), other.data_.cbegin(), data_.begin(),
										 [](auto a, auto b) { return a|b; } ); return *this; }
    /** logical or */
    bitarray operator|(bitarray<SIZE> const &other) const noexcept { bitarray m{*this}; m |= other; return m; }

    /** logical and */
    bitarray &operator&=(bitarray<SIZE> const &other) noexcept { std::transform( data_.begin(), data_.end(), other.data_.cbegin(), data_.begin(),
										 [](auto a, auto b) { return a&b; } ); return *this; }
    /** logical and */
    bitarray operator&(bitarray<SIZE> const &other) const noexcept { bitarray m{*this}; m &= other; return m; }

    /** logical xor */
    bitarray &operator^=(bitarray<SIZE> const &other) noexcept { std::transform( data_.begin(), data_.end(), other.data_.cbegin(), data_.begin(),
										 [](auto a, auto b) { return a^b; } ); return *this; }
    /** logical xor */
    bitarray operator^(bitarray<SIZE> const &other) const noexcept { bitarray m{*this}; m ^= other; return m; }

    /** Count the number of bits */
    [[nodiscard]] size_t popcount() const noexcept { return _private::bitarray_do_popcount(data_); }

    template <size_t MYSIZE>
    friend std::ostream &operator<<(std::ostream &os, bitarray<MYSIZE> const &w);

    template <size_t MYSIZE>
    friend std::istream &operator>>(std::istream &os, bitarray<MYSIZE> &w);

};



template <size_t MYSIZE>
std::ostream &
operator<<(std::ostream &os, bitarray<MYSIZE> const &w)
{
    _private::bitarray_do_ostream(os, w.data_);
    return os;
}
template <size_t MYSIZE>
std::istream &
operator>>(std::istream &os, bitarray<MYSIZE> &w)
{
    _private::bitarray_do_istream(os, w.data_);
    return os;
}


} // namespace bitarrays


#endif


// Local Variables:
// c-basic-offset: 4
// fill-column: 120
// comment-column: 56
// End:
