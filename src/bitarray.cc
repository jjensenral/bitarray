#include <algorithm>
#include <numeric>
#include <bit>
#include <iomanip>
#include <span>

namespace bitarrays {

namespace _private {

void
bitarray_do_shift_left(std::span<u_int32_t> const &data, size_t k) noexcept
{
    // Special case 0 - do nothing
    if(k == 0) {
    }
    // Special case 1 - a shift too far
    else if(k >= (data.size() << 5))
	std::fill(data.begin(), data.end(), 0);
    else {
	size_t delta = k >> 5, shift = k & 0x1f;
	// Special case 2 - exact dword boundaries, shift delta dwords up
	if(shift == 0) {
	    auto first = data.begin();
	    auto middle = first+delta;
	    // We shift right to, er, shift left because we store least signficant word first
	    std::shift_right(data.begin(), data.end(), delta);
	    std::fill(first, middle, 0);
	} else {
	    // General case
	    size_t last = data.size() - delta - 1;
	    size_t other = 32 - shift;
	    // size_t is unsigned, so we have to stop at 0
	    for( size_t y = last; y > 0; --y ) {
		data[y+delta] = (data[y] << shift) | (data[y-1] >> other);
	    }
	    data[delta] = data[0] << shift;
	    // And set the lowest delta entries to zero (up to but not including delta)
	    std::fill_n(data.begin(), delta, 0);
	}
    }
}



void
bitarray_do_shift_right(std::span<u_int32_t> const &data, size_t k) noexcept
{
    // Special case 0 - do nothing
    if(k == 0) {
    }
    // Special case 1 - a shift too far
    else if(k >= (data.size() << 5))
	std::fill(data.begin(), data.end(), 0);
    else {
	size_t delta = k >> 5, shift = k & 0x1f;
	// Special case 2 - exact dword boundaries, shift delta dwords down
	if(shift == 0) {
	    auto last = data.end();
	    // We shift left to, er, shift right because we store least signficant word first
	    std::shift_left(data.begin(), last, delta);
	    std::fill(last-delta, last, 0);
	} else {
	    // General case
	    size_t last = data.size() - delta - 1;
	    size_t other = 32 - shift;
	    for( size_t y = 0; y < last; ++y ) {
		data[y] = (data[y+delta+1] << other) | (data[y+delta] >> shift);
	    }
	    data[last] = data[last+delta] >> shift;
	    // And set the highest delta entries to zero (up to but not including delta)
	    std::fill_n(data.end()-delta, delta, 0);
	}
    }
}



/** Create a bit index, recording the lsb 1 and the width to the msb 1 */
int16_t
bitarray_do_lsb(std::span<const u_int32_t> const &data) noexcept
{
    uint16_t lo = 0;
    for(auto const u : data) {
	if(u == 0)
	    lo += 32;
	else {
	    lo += std::countr_zero(u);
	    break;
	}
    }
    if(lo == data.size() * 32)
	return -1;
    return lo;
}



int16_t
bitarray_do_msb(std::span<const u_int32_t> const &data) noexcept
{
    // Initialise to index of the highest bit
    uint16_t hi = data.size() * 32 - 1;
    auto end = data.rend();
    for(auto p = data.rbegin(); p != end; ++p) {
	if(*p == 0)
	    hi -= 32;
	else {
	    hi -= std::countl_zero(*p);
	    break;
	}
    }
    return hi < 0 ? -1 : hi;
}



void
bitarray_do_fill_from(size_t nmax, std::span<const u_int32_t> const &source, std::span<u_int32_t> const &target) noexcept
{
    if(source.size() < nmax) {
	auto top = std::copy( source.rbegin(), source.rend(), target.begin() );
	std::fill( top, target.end(), 0 );
    } else {
	std::copy_n( source.rbegin(), nmax, target.begin() );
    }
}



size_t
bitarray_do_popcount(std::span<const u_int32_t> const &data) noexcept
{
    return std::accumulate(data.begin(), data.end(), 0,
			   [](size_t const lhs, const u_int32_t rhs) -> size_t { return lhs+std::popcount(rhs); });
}



void
bitarray_do_ostream(std::ostream &os, std::span<const u_int32_t> const &data)
{
    os << std::hex;
    for( auto y = data.rbegin(); y != data.rend(); ++y )
	// two digits per byte
	os << std::setfill('0') << std::setw(sizeof(u_int32_t)*2) << *y << ' ';
}

} // _private

} // bitarrays


// Local Variables:
// c-basic-offset: 4
// fill-column: 120
// comment-column: 56
// End:
