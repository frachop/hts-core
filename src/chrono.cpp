#include "common.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>

#define TS_TO_NANO(ts) ( ((unsigned long long)(ts).tv_sec * (1000000000ULL) ) + (unsigned long long)(ts).tv_nsec )

static inline unsigned long long DeltaNano(const timespec & start, const timespec & end) {

	const unsigned long long nStarted = TS_TO_NANO(start    );
	const unsigned long long nPaused  = TS_TO_NANO(end);

	if (nPaused > nStarted)
		return (nPaused - nStarted);

	return (unsigned long long) 0;
}

namespace HTS_NAMESPACE {

#	ifdef __APPLE__

		void CChrono::clockGetTime(timespec & ts)
		{
			using namespace boost::posix_time;
			ptime p(microsec_clock::universal_time());
			time_duration td(p - ptime(min_date_time));
			const time_duration::tick_type nano = td.total_nanoseconds();
			ts.tv_sec  = nano / (1000000000ULL);
			ts.tv_nsec = nano - (ts.tv_sec * (1000000000ULL));
/*
			const uint64_t now =  mach_absolute_time();
			const Nanoseconds elapsedNano = AbsoluteToNanoseconds( *(AbsoluteTime *) &now );
			const uint64_t nano= * (uint64_t *) &elapsedNano;
			ts.tv_sec  = nano / (1000000000ULL);
			ts.tv_nsec = nano - (ts.tv_sec * (1000000000ULL));
*/
		}

#	else

		void CChrono::clockGetTime(timespec & ts)
		{
			clock_gettime(CLOCK_REALTIME, &ts);
		}

#	endif


	/////////////////////////////////////////////////////////////////////////
	/// Chronometre

	CChrono::CChrono()
	:	m_bStarted  ( false )
	,	m_bRunning  ( false )
	,	m_runCumul  (     0 )
	,	m_pauseCumul(     0 )
	{
		memset(&m_ts, 0, sizeof(m_ts));
	}

	CChrono::CChrono( const CChrono & src) {
		(*this) = src;
	}

	CChrono & CChrono::operator=( const CChrono & src) {
		if (this != &src) {
			m_bStarted  = src.m_bStarted  ;
			m_bRunning  = src.m_bRunning  ;
			m_runCumul  = src.m_runCumul  ;
			m_pauseCumul= src.m_pauseCumul;
			m_ts	= src.m_ts	;
		}
		return (*this);
	}

	CChrono::~CChrono() {
	}

	CChrono & CChrono::operator+=( const CChrono & src) {
		if ((!m_bStarted) && src.m_runCumul) {
			*this = src;
		} else {
			m_runCumul  += src.getRunningNano();
			m_pauseCumul+= src.getPausingNano();
		}
		return *this;
	}


	void CChrono::start () {
		assert( !m_bStarted );
		if (!m_bStarted) {
			clockGetTime(m_ts);
			m_bStarted = true;
			m_bRunning = true;
		}
	}

	void CChrono::pause () {
		assert( m_bStarted );
		if (!m_bStarted) return;

		if (m_bRunning) {

			timespec tsPaused;
			clockGetTime(tsPaused);

			m_runCumul += DeltaNano(m_ts, tsPaused);

			m_ts = tsPaused;
			m_bRunning = false;
		}
	}

	void CChrono::resume() {
		if (!m_bStarted)
			start();

		else if (!m_bRunning) {

			timespec tsResume;
			clockGetTime(tsResume);

			m_pauseCumul += DeltaNano(m_ts, tsResume);

			m_ts = tsResume;
			m_bRunning = true;
		}
	}

	unsigned long long CChrono::getRunningNano() const {
		if (!m_bStarted)
			return 0;

		unsigned long long result = m_runCumul;

		if (m_bRunning) {
			timespec tsNow;
			clockGetTime(tsNow);
			result += DeltaNano(m_ts, tsNow);
		}

		return result;
	}

	unsigned long long CChrono::getPausingNano() const {
		if (!m_bStarted)
			return 0;

		unsigned long long result = m_pauseCumul;

		if (!m_bRunning) {
			timespec tsNow;
			clockGetTime(tsNow);
			result += DeltaNano(m_ts, tsNow);
		}

		return result;
	}

	void CChrono::reset () {
		m_bStarted = false;
		m_bRunning = false;

		bzero(&m_ts, sizeof(m_ts));
		m_runCumul  = 0;
		m_pauseCumul= 0;
	}

	/////////////////////////////////////////////////////////////////////////
	/// CTransfertSpeed
	CTransfertSpeed::CTransfertSpeed()
	:	m_byteCount(0)
	{
	}

	CTransfertSpeed::CTransfertSpeed(const CTransfertSpeed & src)
	:	CChrono(src)
	,	m_byteCount (src.m_byteCount)
	{
	}

	CTransfertSpeed::CTransfertSpeed(const CChrono & src, unsigned long long byteCount)
	:	CChrono(src)
	,	m_byteCount (byteCount)
	{
	}

	CTransfertSpeed & CTransfertSpeed::operator=(const CTransfertSpeed & src) {
		if (this != &src) {
			CChrono::operator=(src);
			m_byteCount = src.m_byteCount;
		}
		return *this;
	}

	void CTransfertSpeed::reset () {
		CChrono::reset();
		m_byteCount = 0;
	}

	CTransfertSpeed::~CTransfertSpeed() {
	}

	CTransfertSpeed & CTransfertSpeed::operator+=( const CTransfertSpeed & src) {
		CChrono::operator+=(src);
		m_byteCount += src.m_byteCount;
		return (*this);
	}
	

}

