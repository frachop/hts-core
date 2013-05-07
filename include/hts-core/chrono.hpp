
namespace HTS_NAMESPACE {

	inline CChrono		operator+(const CChrono&		a,
						  const CChrono&		b)
	{
	  CChrono res(a);

	  res += b;
	  return res;
	}

	inline CTransfertSpeed		operator+(const CTransfertSpeed&	a,
						  const CTransfertSpeed&	b)
	{
	  CTransfertSpeed res(a);

	  res += b;
	  return res;
	}


	inline void			CChrono::stop ()
	{
	  pause ();
	}

	inline bool			CChrono::isStarted() const
	{
	  return m_bStarted;
	}

	inline bool			CChrono::isRunning() const
	{
	  return m_bRunning;
	}

	inline double			CChrono::getRunningSeconds() const
	{
	  return (double)getRunningNano() / 1000000000.0;
	}

	inline double			CChrono::getPausingSeconds() const
	{
	  return (double)getPausingNano() / 1000000000.0;
	}

	inline CTransfertSpeed&		CTransfertSpeed::parallelCumul(const CTransfertSpeed&	src)
	{
	  m_byteCount += src.m_byteCount;
	  m_runCumul   = (src.m_runCumul   > m_runCumul   ) ? src.m_runCumul   : m_runCumul  ;
	  m_pauseCumul = (src.m_pauseCumul > m_pauseCumul ) ? src.m_pauseCumul : m_pauseCumul;
	  if ((!m_bStarted) && m_runCumul)
		m_bStarted= true;
	  return (*this);
	}

}
