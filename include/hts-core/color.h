namespace HTS_NAMESPACE {

	template<typename T>
	class CTColor {
	public:
		typedef T TYPE;

	public:
		TYPE r,g,b,a;

	public:
		CTColor( ) {}
		CTColor(TYPE _r, TYPE _g, TYPE _b, TYPE _a) : r(_r), g(_g), b(_b), a(_a) {}
		CTColor( const CTColor & src ) : r(src.r), g(src.g),b(src.b), a(src.a) {}
		CTColor & operator=( const CTColor & src ) { if (this != &src) { r = src.r; g = src.g; b = src.b; a = src.a; } return (*this); }
		virtual ~CTColor() {}

	public:
		TYPE red  () const { return r; }
		TYPE green() const { return g; }
		TYPE blue () const { return b; }
		TYPE alpha() const { return a; }

	public:
		void setR(TYPE _r) { r = _r; }
		void setG(TYPE _g) { g = _g; }
		void setB(TYPE _b) { b = _b; }
		void setA(TYPE _a) { a = _a; }

	public:
		TYPE operator[](int c) const { return (c==0) ? r : (c==1) ? g : (c==2) ? b : a; }
		TYPE & operator[](int c) { return (c==0) ? r : (c==1) ? g : (c==2) ? b : a; }

	public:
		bool operator==( const CTColor & src ) const { return (src.r == r) && (src.g == g) && (src.b == b) && (src.a == a); }
		bool operator!=( const CTColor & src ) const { return (src.r != r) || (src.g != g) || (src.b != b) || (src.a != a); }

	public:
		CTColor<TYPE> operator-() const { return CTColor<TYPE>(-r,-g,-b,-a); }
		CTColor<TYPE> & operator*=(const TYPE & k) { r *= k; g *= k; b *= k; a *= k; return (*this); }
		CTColor<TYPE> & operator/=(const TYPE & k) { r /= k; g /= k; b /= k; a /= k; return (*this); }
	};

	template<typename TYPE> CTColor<TYPE> operator*(const TYPE & k, const CTColor<TYPE> & c) { return CTColor<TYPE>( k*c.r, k*c.g, k*c.b, k*c.a ); }
	template<typename TYPE> CTColor<TYPE> operator*(const CTColor<TYPE> & c, const TYPE & k) { return CTColor<TYPE>( k*c.r, k*c.g, k*c.b, k*c.a ); }
	template<typename TYPE> CTColor<TYPE> operator/(const CTColor<TYPE> & c, const TYPE & k) { return CTColor<TYPE>( c.r/k, c.g/k, c.b/k, c.a/k ); }
	template<typename TYPE> CTColor<TYPE> operator+(const CTColor<TYPE> & a, const CTColor<TYPE> & b) { return CTColor<TYPE>( a.r+b.r, a.g+b.g, a.b+b.b, a.a+b.a ); }
	template<typename TYPE> CTColor<TYPE> operator-(const CTColor<TYPE> & a, const CTColor<TYPE> & b) { return CTColor<TYPE>( a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a ); }
	
	
	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//
	
	typedef CTColor<unsigned char> CColorb;
	typedef CTColor<float        > CColorf;
	typedef CTColor<double       > CColord;
	typedef CTColor<int          > CColori;

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//
	
}
