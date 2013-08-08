#include "common.h"

#ifndef NDEBUG
#define DUMPOK 0
#include <typeinfo>
#else
#define DUMPOK 0
#endif

namespace HTS_NAMESPACE {

	static void onDestroy( const CObserver  * );
	static void onDestroy( const CObservable* );
	static void dumpOnNotify( const CObserver *, const CNotification &);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CNotification::CNotification(const CObservable * pSender, NOTIFICATION_CODE code, const CVariant & v)
	:	m_pSender(pSender)
	,	m_code(code)
	,	m_data(v)
	{

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CObserver::~CObserver()
	{
		// s'enlever de tous ceux que j'observe
		for (CObservables::iterator it = m_observables.begin(); it != m_observables.end(); ++it) {
			(*it)->m_observers.remove( this );
		}
		onDestroy( this );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CObservable::~CObservable() {
		// enlever tous ceux qui m'observent
		for (CObservers::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
			(*it)->m_observables.remove( this );

		onDestroy( this );
	}

	bool CObservable::isObserver(const CObserver* pObserver) const {
		if( pObserver == NULL )
			return false;

		for (CObservers::const_iterator it = m_observers.begin(); it != m_observers.end(); ++it)
			if (*it == pObserver)
				return true;

		return false;
	}

	std::list< CObserver* > CObservable::getObservers() const
	{
		return m_observers;
	}

	void CObservable::notify(NOTIFICATION_CODE code, const CVariant & v) const {

		CNotification n(this, code, v);
		notify( n );
	}

	void CObservable::notify(CNotification & n) const
	{
		ASSERT( n.sender() == this );

		// Attention aux cas ou le notify modifierait la list des observers
		CObservers l = m_observers;

		// Notifier tous les observers
		for (CObservers::iterator it = l.begin(); it != l.end(); ++it) {
			dumpOnNotify((*it), n);
			(*it)->onNotify(n);

		}
	}

	bool CObservable::addObserver(CObserver* pObserver) {
		if ( pObserver == NULL )
			return false;

		if (CObservable::isObserver(pObserver))
			return false;

		m_observers.push_back(pObserver);
		pObserver->m_observables.push_back(this);

		// Notifier l'observer de l'ajout ( initialisation, ... )
		pObserver->onNotifyAddedTo( this );

		return true;
	}

	bool CObservable::removeObserver( CObserver* pObserver) {

		for (CObservers::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
			if (*it == pObserver) {
				pObserver->m_observables.remove(this);
				m_observers.erase( it );
				return true;
			}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NDEBUG

	static inline void onDestroy( const CObserver  * ) {}
	static inline void onDestroy( const CObservable* ) {}
	static inline void dumpOnNotify( const CObserver *, const CNotification &) {}

#else


	static std::set< const CObserver  * > _observers;
	static std::set< const CObservable* > _observables;
	static std::map< const CObserver*  , std::string > _observerNames;
	static std::map< const CObservable*, std::string > _observableNames;

	CObserver::CObserver() { _observers.insert(this); }
	CObservable::CObservable() { _observables.insert(this); }

	static inline void onDestroy( const CObserver  * o) {
		_observers.erase(o); _observerNames.erase(o);
	#if DUMPOK
		TRACEFN("destroying '%s'\n", getName(o).c_str());
	#endif
	}
	static inline void onDestroy( const CObservable* o) {
		_observables.erase(o); _observableNames.erase(o);
	#if DUMPOK
		TRACEFN("destroying '%s'\n", getName(o).c_str());
	#endif
	}

	void setName( const CObserver * o, const std::string & name) { if (o) _observerNames[o] = name; }
	void setName( const CObservable * o, const std::string & name) { if (o) _observableNames[o] = name; }

	std::string getName( const CObserver * o) {
		std::map< const CObserver*  , std::string >::const_iterator it= _observerNames.find( o );
		if (it != _observerNames.end())
			return it->second;
		return "";
	}
	std::string getName( const CObservable * o) {
		std::map< const CObservable* , std::string >::const_iterator it= _observableNames.find( o );
		if (it != _observableNames.end())
			return it->second;
		return "";
	}

	void dumpObservable(std::ostream & out, const CObservable * pObservable)
	{
		if (pObservable == NULL) {
			out << "NULL" << std::endl;
			return;
		}

		out << "["<< getName(pObservable) << "] " << typeid(*pObservable).name() << " observed by " << pObservable->m_observers.size() << " observer(s)" << std::endl;
		for (CObservable::CObservers::const_iterator itE= pObservable->m_observers.begin(); itE != pObservable->m_observers.end(); ++itE) {
			const CObserver * pObserver = *itE;
			out << "    ["<< getName(pObserver) << "] " << typeid(*pObserver).name() << std::endl;
		}
	}

	void dumpObservers(std::ostream & out)
	{
		out << _observables.size() << " observables and " << _observers.size() << " observers" << std::endl;
		for (std::set< const CObservable* >::const_iterator it= _observables.begin(); it != _observables.end(); ++it)
			dumpObservable(out, *it);
	}

	static void dumpOnNotify( const CObserver * o, const CNotification & n)
	{
	#if DUMPOK
		TRACEFN("'%s'->onNotify(%i by '%s')\n", getName(o).c_str(), n.code(), getName(n.sender()).c_str() );
	#else
		(void)o; (void)n;
	#endif
	}

#endif

}
