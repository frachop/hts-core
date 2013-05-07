namespace HTS_NAMESPACE {

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//
	template<typename T>
	class CTPoint;
	
	template<typename T>
	class CTSize {
	public:
		typedef T TYPE;

	public:
		TYPE cx,cy;
	
	public:
		CTSize( ) {}
		CTSize( TYPE _cx, TYPE _cy ) : cx(_cx), cy(_cy) {}
		CTSize( const CTSize<TYPE> & src ) : cx(src.cx), cy(src.cy) {}
		CTSize( const CTPoint<T> & src );
		template<typename U> CTSize( const CTSize<U> & src ) : cx(static_cast<TYPE>(src.cx)), cy(static_cast<TYPE>(src.cy)) {}
		
		CTSize & operator=( const CTSize & src ) {
			if (this != &src) { cx = src.cx; cy = src.cy; } return (*this); }

		template<typename U>
		CTSize & operator=( const CTSize<U> & src) const {
			if (this != &src) { cx = static_cast<TYPE>(src.cx); cy = static_cast<TYPE>(src.cy); } return (*this); }

		~CTSize() {}

	public:

		bool operator==( const CTSize & src) const { return (src.cx == cx) && (src.cy == cy); }
		bool operator!=( const CTSize & src) const { return (src.cx != cx) || (src.cy != cy); }

	public:
		CTSize<TYPE> & operator+=(const CTSize<TYPE> & a) { cx += a.cx; cy += a.cy; return (*this); }
		CTSize<TYPE> & operator-=(const CTSize<TYPE> & a) { cx -= a.cx; cy -= a.cy; return (*this); }
		template<typename U> CTSize<TYPE> & operator*=(const U & a) { cx *= a; cy *= a; return (*this); }
		template<typename U> CTSize<TYPE> & operator/=(const U & a) { cx /= a; cy /= a; return (*this); }
	};

	// see ezrect.h for operators


	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//

	typedef CTSize<float > CSizef;
	typedef CTSize<double> CSized;
	typedef CTSize<int   > CSizei;

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//

}
