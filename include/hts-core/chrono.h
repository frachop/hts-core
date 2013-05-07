namespace HTS_NAMESPACE {


	/////////////////////////////////////////////////////////////////////////
	/// Chronometre

	class CChrono {
	public:
		static void clockGetTime(timespec & ts);

	public:
		CChrono();
		CChrono(const CChrono & src);
		CChrono& operator = (const CChrono & src);
		virtual ~CChrono();

	public:
		virtual void start ();
		virtual void pause ();
		virtual void resume ();
		virtual void stop ();
		virtual void reset ();

	public:
		bool isStarted() const;
		bool isRunning() const;

	public:
		unsigned long long getRunningNano() const;
		unsigned long long getPausingNano() const;
		double             getRunningSeconds() const;
		double             getPausingSeconds() const;

	public:
		CChrono& operator += (const CChrono & src);

	protected:
		bool				    m_bStarted;
		bool				    m_bRunning;
		timespec			    m_ts;
		unsigned long long m_runCumul;
		unsigned long long m_pauseCumul;
	};

	/////////////////////////////////////////////////////////////////////////
	/// CTransfertSpeed

	class CTransfertSpeed :	public CChrono {
	public:
		CTransfertSpeed ();
		CTransfertSpeed (const CTransfertSpeed & src);
		CTransfertSpeed (const CChrono & src,
						 unsigned long long	  byteCount);
		virtual ~CTransfertSpeed();

		CTransfertSpeed& operator = (const CTransfertSpeed & src);
	
	public:
		unsigned long long byteCount() const { return m_byteCount; }
		unsigned long long addByteCount(unsigned long long count) { m_byteCount += count; return m_byteCount; }
		virtual void reset ();

	public:
		double kiloCount() const { return (double)m_byteCount / 1024.0; }
		double megaCount() const { return (double)m_byteCount / (1024.0 * 1024.0); }
		double gigaCount() const { return (double)m_byteCount / (1024.0 * 1024.0 * 1024.0); }
	
	public:
		CTransfertSpeed & operator+=( const CTransfertSpeed & src);
		CTransfertSpeed & parallelCumul(const CTransfertSpeed & src );

	protected:
		unsigned long long m_byteCount;
	};
	
	CChrono operator+(const CChrono& a, const CChrono& b);
	CTransfertSpeed operator+(const CTransfertSpeed& a, const CTransfertSpeed& b);

}

// inlines
#include "chrono.hpp"

