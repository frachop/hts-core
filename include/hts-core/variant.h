namespace HTS_NAMESPACE {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CVariant
	{
	public:
		CVariant() {}
		CVariant(const CVariant & src) : m_data(src.m_data), m_str( src.m_str) {}
		CVariant(bool b) { m_data.b = b; }
		CVariant(int v) { m_data.i = v; }
		CVariant(short v) { m_data.s = v; }
		CVariant(char v) { m_data.c = v; }
		CVariant(unsigned int v) { m_data.ui = v; }
		CVariant(size_t v) { m_data.st = v; }
		CVariant(float v) { m_data.f = v; }
		CVariant(double v) { m_data.d = v; }
		CVariant(const void * v) { m_data.cp = v; }
		CVariant(void * v) { m_data.p = v; }
		CVariant(int64_t i) { m_data.i64 = i; }
	//	CVariant(uint64_t u) { m_data.u64 = u; }
		CVariant(const std::string & s) : m_str(s) {}
		CVariant & operator=(const CVariant & src) { if (this != &src) { m_data = src.m_data; m_str= src.m_str; } return (*this); }

	public:
		operator bool() const { return m_data.b; }
		operator int() const { return m_data.i; }
		operator short() const { return m_data.s; }
		operator char() const { return m_data.c; }
		operator unsigned int() const { return m_data.ui; }
		operator size_t ()const { return m_data.st; }
		operator unsigned short() const { return m_data.us; }
		operator unsigned char() const { return m_data.uc; }
		operator float() const { return m_data.f; }
		operator double() const { return m_data.d; }
		operator void*() const { return (void*) m_data.p; }
		operator const void*() const { return m_data.cp; }
		operator int64_t() const { return m_data.i64; }
		operator uint64_t() const { return m_data.u64; }
		operator std::string() const { return m_str; }

	public:
		int getBool() const { return m_data.b; }
		int getInt() const { return m_data.i; }
		short getShort() const { return m_data.s; }
		char getChar() const { return m_data.c; }
		unsigned int getUInt() const { return m_data.ui; }
		size_t getSizeT()const { return m_data.st; }
		unsigned short getUShort() const { return m_data.us; }
		unsigned char getUChar() const { return m_data.uc; }
		float getFloat() const { return m_data.f; }
		double getDouble() const { return m_data.d; }
		void* getPtr() const { return (void*) m_data.p; }
		const void* getCPtr() const { return m_data.p; }
		int64_t getInt64() const { return m_data.i64; }
		uint64_t getUInt64() const { return m_data.u64; }
		std::string getStr() const { return m_str; }

	private:
		union _data {
			bool           b;
			int            i;
			short          s;
			char           c;
			unsigned int   ui;
			size_t         st;
			unsigned short us;
			unsigned char  uc;
			float          f;
			double         d;
			void          *p;
			const void    *cp;
			int64_t        i64;
			uint64_t       u64;
		} m_data;

		std::string m_str;
	};

}
