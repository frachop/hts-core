#ifndef NDEBUG
	#ifndef _DEBUG
		#define _DEBUG 1
	#endif
#endif

#ifndef NDEBUG

	#define DBG_BRK asm("int $3")

	#define TRACE HTS_NAMESPACE::trace
	namespace HTS_NAMESPACE {
		inline void trace(const char * pszFormat, ...) {
			va_list argList;
			va_start( argList, pszFormat );
			vprintf (pszFormat, argList);
			va_end( argList );
		}
	}
	#define TRACEFL         TRACE("%s:%i : ", __FILE__, __LINE__); TRACE
	#define TRACEFL0        TRACE("%s:%i\n", __FILE__, __LINE__)
	#define TRACEFN         TRACE("%s : ", __PRETTY_FUNCTION__); TRACE
	#define TRACEFN0        TRACE("%s\n", __PRETTY_FUNCTION__)

	#ifndef ASSERT
		#define ASSERT assert
	#endif

	#ifndef VERIFY
		#define VERIFY assert
	#endif

#else

	#ifndef ASSERT
		#define ASSERT(b) (void)0
	#endif

	#ifndef VERIFY
		#define VERIFY(a) a
	#endif

	#define DBG_BRK (void)0

	#define TRACE   1 ? (void)0 : HTS_NAMESPACE::trace
	#define TRACEFL 1 ? (void)0 : HTS_NAMESPACE::trace
	#define TRACEFN 1 ? (void)0 : HTS_NAMESPACE::trace
	#define TRACEFL0
	#define TRACEFN0

	namespace HTS_NAMESPACE {
		inline void trace(const void * , ...) {}
	}

#endif // EZ_NO_DEBUG_NEW





