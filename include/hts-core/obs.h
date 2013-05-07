namespace HTS_NAMESPACE {

	class CObserver;
	class CObservable;
	class CNotification;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef unsigned int NOTIFICATION_CODE;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CNotification {

		friend class CObservable;

	protected:
		CNotification(const CObservable * pSender, NOTIFICATION_CODE code, const CVariant & v = CVariant());
		~CNotification() {}

	public:
		const CObservable * sender() const { return m_pSender; }
		NOTIFICATION_CODE code() const { return m_code; }
		CVariant getData() const { return m_data; }

	private:
		const CObservable     * m_pSender;
		const NOTIFICATION_CODE m_code;
		const CVariant          m_data;


	};


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CObserver
	{
	protected:
		CObserver();

	public:
		virtual ~CObserver();

	public:
		virtual void onNotifyAddedTo( const CObservable * ) {}
		virtual void onNotify(CNotification & n) = 0;

	private:
		CObserver(const CObserver &) {}

	private:
		friend class CObservable;
		typedef std::list< CObservable* > CObservables;
		CObservables m_observables;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CObservable
	{
	public:
		enum { NOTIFY_CHANGED = 1 };

	public:
		CObservable();
		virtual ~CObservable();
		void notify(NOTIFICATION_CODE code = NOTIFY_CHANGED, const CVariant & v = CVariant()) const;
		void notify(CNotification & n) const;

	public:
		bool isObserver(const CObserver* pObserver) const;
		bool addObserver(CObserver* pObserver);
		bool removeObserver(CObserver* pObserver);

	public:
		std::list< CObserver* > getObservers() const;

	private:
		friend class CObserver;
		friend void dumpObservable(std::ostream & , const CObservable * );// for debug only
		CObservable(const CObservable &) {}
		typedef std::list< CObserver* > CObservers;
		CObservers m_observers;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	class CTObservableProperty
	:	public CObservable
	{
	public:
		typedef T TYPE;

	protected:
		TYPE m_value;

	public:
		CTObservableProperty() {}
		explicit CTObservableProperty(const TYPE v) : m_value(v) {}
		virtual ~CTObservableProperty() {}

		operator T() const { return m_value; }
		void operator=(const TYPE v) { set(v); }
		bool operator==(const TYPE v) { return m_value==v; }
		TYPE get() const { return m_value; }
		void set( const TYPE & val )
		{
			if( m_value != val )
			{
				m_value = val;
				callNotify();
			}
		}

	private:
		void callNotify() {
			notify(NOTIFY_CHANGED, CVariant(m_value) );
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef CTObservableProperty<bool> CObservableBool;
	typedef CTObservableProperty<int> CObservableInt;
	typedef CTObservableProperty<float> CObservableFloat;
	typedef CTObservableProperty<double> CObservableDouble;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#ifdef NDEBUG

		inline CObserver::CObserver() {}
		inline CObservable::CObservable() {}
		inline void setName( const CObserver * ) {}
		inline void setName( const CObservable * ) {}
		inline std::string getName( const CObserver * ) { return ""; }
		inline std::string getName( const CObservable * ) { return ""; }
		inline void dumpObservable(std::ostream &, const CObservable * ) {}
		inline void dumpObservers(std::ostream & ) {}

	#else

		void setName( const CObserver * , const std::string &);
		void setName( const CObservable * , const std::string &);
		std::string getName( const CObserver * );
		std::string getName( const CObservable * );
		void dumpObservable(std::ostream & out, const CObservable * p);
		void dumpObservers(std::ostream & out);

	#endif // NDEBUG


}
