
namespace HTS_NAMESPACE {
namespace thread {

	/////////////////////////////////////////////////////////////////////////
	/// typedefs

	typedef pthread_mutex_t * ppthread_mutex_t;

	/////////////////////////////////////////////////////////////////////////
	/// CMutex

	class CMutex {
	protected:
		pthread_mutex_t m_mutex;

	public:
		CMutex(const pthread_mutexattr_t * attr = NULL);
		virtual ~CMutex();

	public:
		operator pthread_mutex_t () const { return m_mutex; }
		operator ppthread_mutex_t() const { return (ppthread_mutex_t) &m_mutex; }

	public:
		int lock	() const { return pthread_mutex_lock   ((ppthread_mutex_t)&m_mutex); }
		int trylock	() const { return pthread_mutex_trylock((ppthread_mutex_t)&m_mutex); }
		int unlock	() const { return pthread_mutex_unlock ((ppthread_mutex_t)&m_mutex); }
	};

	/////////////////////////////////////////////////////////////////////////
	/// CMutexAuto

	class CAutoMutex {
	private:
		const CMutex * m_pMutex;

	public:
		CAutoMutex(const CMutex * pMutex) : m_pMutex(pMutex) { if (m_pMutex) m_pMutex->lock(); }
		virtual ~CAutoMutex() { if (m_pMutex) m_pMutex->unlock(); }
	};

	/////////////////////////////////////////////////////////////////////////
	/// Mutexed data item
	
	template<typename T>
	class CTMutexedData {
	public:
		CTMutexedData() {}
		CTMutexedData(const T & data) : m_data(data) {}
	
	public:
		void operator=( const T & src) { set(src); }
	
	public:
		T get() const { CAutoMutex a(&m_mutex); return m_data; }
		void set(const T & src) { CAutoMutex a(&m_mutex); m_data = src; }
		T operator++(   ) { CAutoMutex a(&m_mutex); return ++m_data; }
		T operator++(int) { CAutoMutex a(&m_mutex); return m_data++; }
		T operator--(   ) { CAutoMutex a(&m_mutex); return --m_data; }
		T operator--(int) { CAutoMutex a(&m_mutex); return m_data--; }

	public:
		void setNoLock(const T & src) { m_data = src; }
		T getNoLock() const { return m_data; }

	public:
		operator CMutex*() { return &m_mutex; }
		int lock  () { return m_mutex.lock(); }
		int unlock() { return m_mutex.unlock(); }

	private:
		CMutex m_mutex;
		volatile T m_data;
	};
	
	/////////////////////////////////////////////////////////////////////////
	///
	
	class CMutexedString {
	public:
		CMutexedString();
		CMutexedString(const std::string & s);
		virtual ~CMutexedString();
	
	public:
		void operator=( const std::string & s) { set(s); }
	
	public:
		std::string get();
		void set(const std::string & src);

	public:
		void setNoLock(const std::string & src);
		std::string getNoLock() const;

	public:
		operator CMutex*() { return &m_mutex; }
		int lock  () { return m_mutex.lock(); }
		int unlock() { return m_mutex.unlock(); }

	protected:
		CMutex m_mutex;
		char * m_pData;
	};
	
	/////////////////////////////////////////////////////////////////////////
	/// CThreadAttr
	
	class CThreadAttr {
	protected:
		pthread_attr_t m_attr;

	public:
		CThreadAttr(bool bCreateJoinable= true);
		virtual ~CThreadAttr();
		operator pthread_attr_t() const { return (pthread_attr_t) m_attr; }
		operator pthread_attr_t*() const { return (pthread_attr_t*) &m_attr; }
	
	public:
		/* Set the `detachstate' attribute in *ATTR according to DETACHSTATE.  */
		int setdetachstate (int detachstate);
		
		/* Return in *DETACHSTATE the `detachstate' attribute in *ATTR.  */
		int getdetachstate (int *detachstate);
	};

	/////////////////////////////////////////////////////////////////////////
	/// CJob
	
	class CJob
	{
	public:
		CJob();
		virtual ~CJob();
		int start(pthread_attr_t * pAttr= NULL);

	public:
		bool isRunning   () const;
		bool isTerminated() const;
		pthread_t threadId() const { return m_thread; }
	
	public:
		void join( bool bDetachAfterJoin = true );
		void detach();

	public:
		bool setPriority(int priority);
		int  getPriority();

	protected:
		virtual void run(void) = 0;
	
	private:
		static void * entryPoint(void*);
		void instanceEntryPoint();
		pthread_t   m_thread;
		CMutex      m_mutex;
		bool m_bRunning;
		bool m_bTerminated;
	};
	
}}
