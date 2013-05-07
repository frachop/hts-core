namespace HTS_NAMESPACE {

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//
	template<typename T>
	class CTSize;
	
	template<typename T>
	class CTPoint {
	public:
		typedef T TYPE;

	public:
		TYPE x,y;
	
	public:
		CTPoint( ) {}
		CTPoint( TYPE _x, TYPE _y ) : x(_x), y(_y) {}
		CTPoint( const CTPoint & src ) :	x(src.x), y(src.y) {}
		CTPoint( const CTSize<T> & src );
		template<typename U> CTPoint( const CTPoint<U> & src ) :	x(static_cast<TYPE>(src.x)), y(static_cast<TYPE>(src.y)) {}
		
		CTPoint & operator=( const CTPoint & src ) {
			if (this != &src) { x = src.x; y = src.y; } return (*this); }

		template<typename U>
		CTPoint & operator=( const CTPoint<U> & src) const {
			if (this != &src) { x = static_cast<TYPE>(src.x); y = static_cast<TYPE>(src.y); } return (*this); }

		~CTPoint() {}

	public:

		bool operator==( const CTPoint & src) const { return (src.x == x) && (src.y == y); }
		bool operator!=( const CTPoint & src) const { return (src.x != x) || (src.y != y); }

	public:
		CTPoint<TYPE> & operator+=(const CTPoint<TYPE> & a) { x += a.x; y += a.y; return (*this); }
		CTPoint<TYPE> & operator-=(const CTPoint<TYPE> & a) { x -= a.x; y -= a.y; return (*this); }
		CTPoint<TYPE> & operator*=(const TYPE & a) { x *= a; y *= a; return (*this); }
		CTPoint<TYPE> & operator/=(const TYPE & a) { x /= a; y /= a; return (*this); }
	};

	// see ezrect.h for operators

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//

	typedef CTPoint<float > CPointf;
	typedef CTPoint<double> CPointd;
	typedef CTPoint<int   > CPointi;

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//

}
