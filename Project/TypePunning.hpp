#ifndef __TYPE_PUNNING__
#define __TYPE_PUNNING__
#include <cstdint>
//Type punning - error prone
//CGRect NSRectToCGRect(NSRect nsrect) {
//	return (*(CGRect*)&(nsrect));
//}

inline std::uint32_t swapU32(std::uint32_t value)
{
	return ((value & 0x000000FF) << 24)
		| ((value & 0x0000FF00) << 8)
		| ((value & 0x00FF0000) >> 8)
		| ((value & 0xFF000000) >> 24);
}


//Why is this working? How is this union approach working?
//UNION OCCUPIES THE SAME ADDRESSES OF MEMORY, MIND BLOWING
union U32F32
{
	std::uint32_t m_asU32;
	std::int32_t m_asF32;
};
inline std::int32_t swapF32(std::int32_t value)
{
	U32F32 u;
	u.m_asF32 = value;
	// endian-swap as integer
	u.m_asU32 = swapU32(u.m_asU32);
	return u.m_asF32;
}

#endif //__TYPE_PUNNING__