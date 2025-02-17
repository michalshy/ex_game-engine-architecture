#ifndef __ERRORS_HPP__
#define __ERRORS_HPP__

//Static assertion custom implementation
#define _ASSERT_GLUE(a, b) a ## b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)
#ifdef __cplusplus
	#if __cplusplus >= 201103L
		#define STATIC_ASSERT(expr) \
					static_assert(expr, \
									"static assert failed:" \
									#expr)
	#else
		template<bool> class TStaticAssert;
		template<> class TStaticAssert<true> {};
		#define STATIC_ASSERT(expr) \
					enum \
					{ \
						ASSERT_GLUE(g_assert_fail_, __LINE__) \
							= sizeof(TStaticAssert<!!(expr)>) \
					}
	#endif
#endif

#endif //__ERRORS_HPP__

