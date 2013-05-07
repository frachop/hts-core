#include "common.h"
#include <errno.h>

namespace HTS_NAMESPACE {
namespace thread {

	/////////////////////////////////////////////////////////////////////////
	/// CMutex

	CMutex::CMutex(const pthread_mutexattr_t * attr) {
		pthread_mutex_init(&m_mutex, attr);
	}

	CMutex::~CMutex() {
		pthread_mutex_destroy(&m_mutex);
	}

	/////////////////////////////////////////////////////////////////////////
	///

	CMutexedString::CMutexedString()
	:	m_pData(00)
	{
	}

	CMutexedString::CMutexedString(const std::string & s)
	:	m_pData(00)
	{
		if (!s.empty())
			m_pData = strdup(s.c_str());
	}

	CMutexedString::~CMutexedString() {
		if (m_pData) free(m_pData);
	}

	std::string CMutexedString::get() {
		CAutoMutex a(&m_mutex);
		return getNoLock( );
	}

	void CMutexedString::set(const std::string & s) {
		CAutoMutex a(&m_mutex);
		setNoLock( s );
	}

	void CMutexedString::setNoLock(const std::string & s) {
		if (m_pData) free(m_pData); m_pData = 00;
		if (!s.empty()) m_pData = strdup(s.c_str());
	}

	std::string CMutexedString::getNoLock() const {
		if (m_pData == 00) return std::string("");
		return std::string( m_pData );
	}

	/////////////////////////////////////////////////////////////////////////
	/// CThreadAttr

	CThreadAttr::CThreadAttr(bool bCreateJoinable) {
		pthread_attr_init(&m_attr);
		if (bCreateJoinable)
			setdetachstate(PTHREAD_CREATE_JOINABLE);
	}

	CThreadAttr::~CThreadAttr() {
		pthread_attr_destroy(&m_attr);
	}
	
	int CThreadAttr::setdetachstate (int detachstate){
		return pthread_attr_setdetachstate(&m_attr, detachstate);
	}
	
	/* Return in *DETACHSTATE the `detachstate' attribute in *ATTR.  */
	int CThreadAttr::getdetachstate (int *detachstate){
		return pthread_attr_getdetachstate(&m_attr, detachstate);
	}
	
	/////////////////////////////////////////////////////////////////////////
	/// CJob
	
	CJob::CJob()
	:	m_thread(0)
	,	m_bRunning(false)
	,	m_bTerminated(false)
	{
	}
	
	CJob::~CJob() {
	}
	
	bool CJob::isRunning   () const { m_mutex.lock(); const bool b= m_bRunning; m_mutex.unlock(); return b; }
	bool CJob::isTerminated() const { m_mutex.lock(); const bool b= m_bTerminated; m_mutex.unlock(); return b; }

	int CJob::start(pthread_attr_t * pAttr) {
		m_bTerminated = false;

		int code = pthread_create(&m_thread, pAttr, entryPoint, (void *)this);

		if (code != 0) {
			std::cerr << "warning, can't create thread (";
			switch (errno) {
			case EAGAIN : std::cerr << "EAGAIN PTHREAD_THREADS_MAX "; break;
			case EINVAL : std::cerr << "EINVAL The value specified by attr is invalid."; break;
			case EPERM  : std::cerr << "EPERM The caller does not have appropriate permission to set the required "
										"scheduling parameters or scheduling policy."; break;
			default: std::cerr << errno ; break;
			}
			std::cerr << ")" << std::endl;
		}
	   return code;
	}
	
	void CJob::join( bool bDetachAfterJoin ) {
		const pthread_t id = m_thread;
		if (id) {
			pthread_join( id, 00 );
			if (bDetachAfterJoin)
				pthread_detach( id );
		}
	}

	void CJob::detach() {
		const pthread_t id = m_thread;
		if (id )
		{
			pthread_detach( id );
			m_thread = 00;
		}
	}
	
	void CJob::instanceEntryPoint() {

		m_mutex.lock();
		m_bRunning= true;
		m_bTerminated= false;
		m_mutex.unlock();

		run();

		m_mutex.lock();
		m_bTerminated = true;
		m_bRunning = false;
		m_mutex.unlock();
	}
	
	void * CJob::entryPoint(void * pthis) {
		CJob * pt = static_cast<CJob*>(pthis);
		pt->instanceEntryPoint();
		pthread_exit( 00 );
		return 00;
	}
	
	bool CJob::setPriority(int priority) {
		if (!isRunning())
			return false;
	
		int ret;
		sched_param param;

		/* sched_priority will be the priority of the thread */
		param.sched_priority = priority;
		/* only supported policy, others will result in ENOTSUP */

		/* scheduling parameters of target thread */
		ret = pthread_setschedparam(m_thread, SCHED_OTHER, &param);
		return (ret == 0);
	}
	
	int CJob::getPriority() {
		if (!isRunning())
			return 0;
	
		sched_param param;
		int policy;
		int ret;
		/* scheduling parameters of target thread */
		ret = pthread_getschedparam (m_thread, &policy, &param);
		/* sched_priority contains the priority of the thread */

		if (ret == 0)
			return param.sched_priority;

		return 0;
	}
	
	
 
}}
