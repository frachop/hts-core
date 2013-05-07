//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace HTS_NAMESPACE {
namespace string {

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string format(const char * frmt, ...);
	std::string format(const std::string & frmt, ...);
	int formatv(std::string &result, const char* frmt, va_list ap, int bufSize);

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string trimR(const std::string & str, const std::string & match = " \t\f\v\n\r");
	std::string trimL(const std::string & str, const std::string & match = " \t\f\v\n\r");
	std::string trim (const std::string & str, const std::string & match = " \t\f\v\n\r");

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string toLower( const std::string & src );
	std::string toUpper( const std::string & src );

	void toLower( std::string & src );
	void toUpper( std::string & src );


	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string replace(const std::string & dans_ceci, const std::string & ceci, const std::string & par_cela);

	//- INLINES /////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline std::string trim (const std::string & str, const std::string & match) { return trimR(trimL(str, match), match); }
	inline std::string toLower( const std::string & src ) {
		std::string result = src;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	inline std::string toUpper( const std::string & src ) {
		std::string result = src;
		std::transform(result.begin(), result.end(), result.begin(), ::toupper);
		return result;
	}

	inline void toLower( std::string & src ) { std::transform(src.begin(), src.end(), src.begin(), ::tolower); }
	inline void toUpper( std::string & src ) { std::transform(src.begin(), src.end(), src.begin(), ::toupper); }

}}



