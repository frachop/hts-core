/*
 * strings.cpp
 *
 *  Created on: 3 mai 2013
 *      Author: Franck
 */
#include "common.h"

namespace HTS_NAMESPACE {
namespace string {

	enum { _defaultBufferSize = 12 * 1024 };

	int formatv(std::string &result, const char* frmt, va_list ap, int bufSize)
	{
		char * pBuf = new char[bufSize + 1]; bzero( pBuf, 1+bufSize );
		const int res = vsnprintf(pBuf, bufSize + 1, frmt, ap);
		result = std::string(pBuf);
		delete[] pBuf;
		return res;
	}

	std::string format(const std::string & frmt, ...)
	{
		va_list  pa;
		std::string result;

		int bufSize = _defaultBufferSize;

		va_start(pa, frmt);
		int res= formatv(result, frmt.c_str(), pa, bufSize);
		if (res >= bufSize) {
			va_end(pa);
			va_start(pa, frmt);
			formatv(result, frmt.c_str(), pa, 1 + res);
		}
		va_end(pa);
		return result;
	}

	std::string format(const char * frmt, ...)
	{
		va_list  pa;
		std::string result;

		int bufSize = _defaultBufferSize;

		va_start(pa, frmt);
		int res= formatv(result, frmt, pa, bufSize);
		if (res >= bufSize) {
			va_end(pa);
			va_start(pa, frmt);
			formatv(result, frmt, pa, 1 + res);
		}
		va_end(pa);
		return result;
	}

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string trimR(const std::string & str, const std::string & match) {

		const int mlg = match.length();
		if (mlg == 0) return str;

		const int  lg = str.length();
		if (lg == 0) return std::string("");

		int i;
		std::set<char> s;
		for( i=0; i<mlg; ++i) s.insert(match[i]);

		std::string result = str;
		std::set<char>::iterator it;
		i = lg - 1;
		while (i>=0) {
			it = s.find( result[i] );
			if (it == s.end())
				return result;

			result.erase(i--, 1);
		}

		return std::string("");

	}

	std::string trimL(const std::string & str, const std::string & match) {

		const int mlg = match.length();
		if (mlg == 0) return str;

		const int  lg = str.length();
		if (lg == 0) return std::string("");

		int i;
		std::set<char> s;
		for( i=0; i<mlg; ++i) s.insert(match[i]);

		std::string result = str;
		std::set<char>::iterator it;
		while ( result.length() ) {
			it = s.find( result[0] );
			if (it == s.end())
				return result;

			result.erase(0, 1);
		}

		return std::string("");

	}

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string replace(const std::string & dans_ceci, const std::string & ceci, const std::string & par_cela) {

		if (ceci.empty() || dans_ceci.empty() || (ceci == par_cela))
			return dans_ceci;

		if (dans_ceci == ceci)
			return par_cela;

		const std::string::size_type lgCeci = ceci.length();

		std::string resultat = dans_ceci;
		while (true) {

			std::string::size_type found = resultat.find( ceci );
			if (found == std::string::npos)
				break;

			resultat.replace(found, lgCeci, par_cela);

		};

		return resultat;
	}

}}
