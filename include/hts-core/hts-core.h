/*
 * hts-core.h
 *
 *  Created on: 3 mai 2013
 *      Author: Franck
 */

#ifndef HTS_CORE_H_
#define HTS_CORE_H_

/////////////////////////////////////////////////////////////////////////
/// global includes

#include <cstdarg>
#include <cstring>
#include <cwchar>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cassert>

/*/////////////////////////////////////////////////////////////////////*/
/*! @name Assertion statique

	voir <a href="http://h-deb.clg.qc.ca/Sujets/Divers--cplusplus/Metaprogrammation.html#assertion-statique" target="_blank">une doc claire</a>.
	exemple :
\code
// cette ligne ne compilera pas
STATIC_ASSERT< sizeof(short) == sizeof(int) > ();

// cette ligne compilera
STATIC_ASSERT< sizeof(short) != sizeof(int) > ();
\endcode

*/

template <bool> struct STATIC_ASSERT;
template <> struct STATIC_ASSERT<true> {};

//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HTS_NAMESPACE hts

//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "debug.h"
#include "color.h"
#include "point.h"
#include "size.h"
#include "rect.h"
#include "matrix.h"
#include "variant.h"
#include "obs.h"
#include "chrono.h"
#include "thread.h"

#ifndef __HTSCORE_NO_STRINGS
#	include "strings.h"
#	include "logger.h"

#	ifndef __HTSCORE_NO_XML
#		include "xml.h"
#	endif
#endif




#endif /* HTS_CORE_H_ */
