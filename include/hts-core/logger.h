#include <syslog.h>

namespace HTS_NAMESPACE {
namespace logger {

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CBase {
	public:
		CBase() {}
		virtual ~CBase() {}

	public:
		void log (const char * format, ...);
		void log (int facility, const char * format, ...);
		void vlog(const std::string & format, va_list argList, int facility = LOG_INFO);

	protected:
		virtual void logString( const std::string & logStr, int facility);
	};

}}




