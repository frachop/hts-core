#include "common.h"

namespace HTS_NAMESPACE {
namespace logger {

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CBase::log (const char * format, ...) {
		va_list  pa;
		va_start(pa, format);
		vlog(format, pa);
		va_end(pa);
	}

	void CBase::log (int facility, const char * format, ...) {
		va_list  pa;
		va_start(pa, format);
		vlog(format, pa, facility);
		va_end(pa);
	}
	
	void CBase::vlog(const std::string & format, va_list pa, int facility) {
		char buffer[2*1024]; bzero(buffer, sizeof(buffer));
		vsnprintf (buffer, sizeof(buffer)-1, format.c_str(), pa);
		logString( buffer, facility);
	}

	void CBase::logString( const std::string & logStr, int facility) {
		std::cout << facility << " : " << logStr << std::endl;
	}

 
}}
