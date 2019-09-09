#include <cstdio>
#include <cstdint>
#include <cmath>
#include <xmmintrin.h>

typedef uint8_t u8;
typedef int32_t s32;
typedef uint32_t u32;
typedef float_t f32;

f32 divByZero() 
{
	// Consider turning off optimization if your compiler is smart.
	u8 zeroBuffer[4] = {};
	return 1 / (f32)*(s32*)(zeroBuffer);
}

int main()
{
#if defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64)
	/*
	* MXCSR register
	* 
	* Rounding flags
	*   FZ  [15]: Flush to Zero on underflow (only if UM is set).
	*   RC  [13-14]:
	*       RN  00 : Round to Nearest
	*       R-  01 : Round to Negative(-inf)
	*       R+  10 : Round to Positive(+inf)
	*       RZ  11 : Round to Zero
	* 
	* Exception masks (set 1 to mask, 0 to trap)
	*   PM  [12]: Precision
	*   UM  [11]: Underflow
	*   OM  [10]: Overflow
	*   ZM  [9] : Divide by Zero
	*   DM  [8] : Denormal 
	*   IM  [7] : Invalid Operation
	*   DAZ [6] : Clamp Denormals to Zero
	* 
	* Status flags
	*   PE  [5] : Precision
	*   UE  [4] : Underflow
	*   OE  [3] : Overflow
	*   ZE  [2] : Devide by Zero
	*   DE  [1] : Denormal
	*   IE  [0] : Invalid Operation
	*/
	const u32 csr = _mm_getcsr();

	// Unset ZM bit to trap division by zero.
	const u32 ZM = 0x200;/* MXCSR[9] -> 0b1000000000 */
	_mm_setcsr(csr & ~ZM);
#elif defined(__i386__) || defined(_M_IX86)
	// to be implemented.
#endif

	std::printf("%f\n", divByZero());// This line should raise an exception.

	return 0;
}
