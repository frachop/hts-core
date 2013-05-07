namespace HTS_NAMESPACE {

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//
	
	template<typename T>
	class CTRect {
	public:
		typedef T TYPE;

	public:
		TYPE x,y,w,h;
	
	public:
		CTRect( ) {}
		CTRect( TYPE _x, TYPE _y, TYPE _w, TYPE _h ) : x(_x), y(_y), w(_w), h(_h) {}
		CTRect( const CTRect & src ) : x(src.x), y(src.y),w(src.w), h(src.h) {}
		CTRect( const CTPoint<TYPE> & p0, const CTPoint<TYPE> & p1 ) : x(p0.x), y(p0.y), w(p1.x-p0.x), h(p1.y-p0.y) {}
		CTRect( const CTPoint<TYPE> & p, const CTSize<TYPE> & sz ) : x(p.x), y(p.y), w(sz.cx), h(sz.cy) {}
		CTRect & operator=( const CTRect & src ) { if (this != &src) { x = src.x; y = src.y; w = src.w; h = src.h; } return (*this); }
		virtual ~CTRect() {}

		CTRect operator&( const CTRect & src ) const {
			if (this != &src) {
				CTRect rect_intersect;
				rect_intersect.x = std::max(src.x,x);
				rect_intersect.y = std::max(src.y,y);
				rect_intersect.w = std::min(src.x+src.w, x+w) - rect_intersect.x;
				rect_intersect.h = std::min(src.y+src.h, y+h) - rect_intersect.y;
				return rect_intersect;
			}
			return src;
		}

		CTRect operator|( const CTRect & src ) const {
			if (this != &src) {
				CTRect rect_union;
				rect_union.x = std::min(src.x,x);
				rect_union.y = std::min(src.y,y);
				rect_union.w = std::max(src.x+src.w, x+w) - rect_union.x;
				rect_union.h = std::max(src.y+src.h, y+h) - rect_union.y;
				return rect_union;
			}
			return src;

		}
		
		CTRect & operator&=( const CTRect & src ) { return ( (*this) = (*this) & src ); }
		CTRect & operator|=( const CTRect & src ) { return ( (*this) = (*this) | src ); }

	public:
		bool operator==( const CTRect & src ) const { return (src.x == x) && (src.y == y) && (src.w == w) && (src.h == h);}
		bool operator!=( const CTRect & src ) const { return (src.x != x) || (src.y != y) || (src.w != w) || (src.h != h); }

	public:
		bool ptInRect( const CTPoint<TYPE> & pt ) const { return (pt.x >= x) && (pt.x < x+w) && (pt.y >= y) && (pt.y < y+h); }
	
	public:
		TYPE width () const { return w; }
		TYPE height() const { return h; }
		TYPE left  () const { return x; }
		TYPE right () const { return x + w; }
		TYPE top   () const { return y; }
		TYPE bottom() const { return y + h; }
		CTPoint<TYPE> topLeft() const { return CTPoint<TYPE>(left(),top());  }
		CTPoint<TYPE> bottomRight() const { return CTPoint<TYPE>(right(),bottom());  }
		CTPoint<TYPE> leftTop() const { return topLeft();  }
		CTPoint<TYPE> rightBottom() const { return bottomRight();  }
	
	public:
		CTPoint<TYPE> pos() const { return CTPoint<TYPE>(x,y); }
		CTSize<TYPE> size() const { return CTSize <TYPE>(w,h); }
		bool isEmpty() const { return (w <= (TYPE)0) || (h <= (TYPE)0); }
	
	public:
		void setWidth (const TYPE & _w) { w = _w; }
		void setHeight(const TYPE & _h) { h = _h; }
		void setSize(const CTSize<TYPE> & sz) { w = sz.cx; h = sz.cy; }

	public:
		CTRect & translate (const TYPE & dx, const TYPE & dy) { x += dx; y += dy; return (*this); }
		CTRect & translate (const CTPoint<TYPE> & pt) { return translate(pt.x, pt.y); }
		CTRect & offset    (const TYPE & dx, const TYPE & dy) { return translate(dx, dy); }
		CTRect & offset    (const CTPoint<TYPE> & pt) { return translate(pt); }
		CTRect & operator+=(const CTPoint<TYPE> & pt) { return translate( pt); }
		CTRect & operator-=(const CTPoint<TYPE> & pt) { return translate(-pt); }

		CTRect & grow      (const TYPE & cx, const TYPE & cy) { x += cx; y += cy; return (*this); }
		CTRect & grow      (const CTSize <TYPE> & sz ) { return grow(sz.cx, sz.cy); }
		CTRect & operator+=(const CTSize <TYPE> & sz) { w += sz.cx; h += sz.cy; return (*this); }
		CTRect & operator-=(const CTSize <TYPE> & sz) { w += sz.cx; h += sz.cy; return (*this); }

	public:
		CTPoint<TYPE> centerPoint() const { return CTPoint<TYPE>( x + (w / ((TYPE)2)), y + (h / ((TYPE)2)) ); }
		
		CTRect<TYPE> & inflate(TYPE left, TYPE top, TYPE right, TYPE bottom) { x -= left; y -= top; w += (left+right); h += (top+bottom); return (*this); }
		CTRect<TYPE> & inflate(TYPE _x, TYPE _y) { return inflate( _x, _y, _x, _y); }
		CTRect<TYPE> & inflate(TYPE _i) { return inflate( _i, _i, _i, _i); }
		CTRect<TYPE> & inflate(const CTSize<TYPE> &sz ) { return inflate(sz.cx, sz.cy); }

		CTRect<TYPE> & deflate(TYPE left, TYPE top, TYPE right, TYPE bottom) { x += left; y += top; w -= (left+right); h -= (top+bottom); return (*this); }
		CTRect<TYPE> & deflate(TYPE _x, TYPE _y) { return deflate( _x, _y, _x, _y); }
		CTRect<TYPE> & deflate(TYPE _i) { return deflate( _i, _i, _i, _i); }
		CTRect<TYPE> & deflate(const CTSize<TYPE> &sz ) { return deflate(sz.cx, sz.cy); }
	};

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//

	typedef CTRect<float > CRectf;
	typedef CTRect<double> CRectd;
	typedef CTRect<int   > CRecti;

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	template <typename T> inline CTSize<T>::CTSize(const CTPoint<T> & src) : cx(src.x), cy(src.y) {}
	template <typename T> inline CTPoint<T>::CTPoint(const CTSize<T> & src) : x(src.cx), y(src.cy) {}

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	template <typename T, typename U> inline CTSize<T> operator/(const CTSize<T> & a, const U & d) { return CTSize<T>( a.cx / d, a.cy / d); }
	template <typename T, typename U> inline CTSize<T> operator*(const CTSize<T> & a, const U & d) { return CTSize<T>( a.cx * d, a.cy * d); }
	template <typename T, typename U> inline CTSize<T> operator*(const U & d, const CTSize<T> & a) { return CTSize<T>( a.cx * d, a.cy * d); }

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	template<typename T> inline CTSize<T> operator+(const CTPoint<T> & a, const CTSize<T> & b) { return CTSize<T>(a.x + b.cx, a.y + b.cy); }
	template<typename T> inline CTSize<T> operator+(const CTPoint<T> & a, const CTPoint<T> & b) { return CTSize<T>(a.x + b.x, a.y + b.y); }
	template<typename T> inline CTSize<T> operator-(const CTPoint<T> & a, const CTPoint<T> & b) { return CTSize<T>(a.x - b.x, a.y - b.y); }
	template<typename T> inline CTSize<T> operator-(const CTPoint<T> & a, const CTSize<T> & b) { return CTSize<T>(a.x - b.cx, a.y - b.cy); }

	template<typename T> inline CTPoint<T> operator*(const CTPoint<T> & a, const T & b) { return CTPoint<T>(a.x * b, a.y * b); }
	template<typename T> inline CTPoint<T> operator*(const T & a, const CTPoint<T> & b) { return CTPoint<T>(a * b.x, a * b.y); }
	template<typename T> inline CTPoint<T> operator/(const CTPoint<T> & a, const T & b) { return CTPoint<T>(a.x / b, a.y / b); }
	template<typename T> inline CTPoint<T> operator-(const CTPoint<T> & a) { return CTPoint<T>(-a.x, -a.y); }

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//

	template<typename T> inline CTSize<T> operator+(const CTSize<T> & a, const CTSize<T> & b) { return CTSize<T>(a.cx + b.cx, a.cy + b.cy); }
	template<typename T> inline CTSize<T> operator-(const CTSize<T> & a, const CTSize<T> & b) { return CTSize<T>(a.cx - b.cx, a.cy - b.cy); }
	template<typename T> inline CTSize<T> operator*(const CTSize<T> & a, const T & b) { return CTSize<T>(a.cx * b, a.cy * b); }
	template<typename T> inline CTSize<T> operator*(const T & a, const CTSize<T> & b) { return CTSize<T>(a * b.cx, a * b.cy); }
	template<typename T> inline CTSize<T> operator/(const CTSize<T> & a, const T & b) { return CTSize<T>(a.cx / b, a.cy / b); }
	template<typename T> inline CTSize<T> operator-(const CTSize<T> & a) { return CTSize<T>(-a.cx, -a.cy); }

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	//
	
	template<typename T> inline CTRect<T> operator+(const CTRect<T> & r, const CTPoint<T> & pt) { CTRect<T> res(r); res += pt; return res; }
	template<typename T> inline CTRect<T> operator-(const CTRect<T> & r, const CTPoint<T> & pt) { CTRect<T> res(r); res -= pt; return res; }
	template<typename T> inline CTRect<T> operator+(const CTRect<T> & r, const CTSize <T> & sz) { CTRect<T> res(r); res += sz; return res; }
	template<typename T> inline CTRect<T> operator-(const CTRect<T> & r, const CTSize <T> & sz) { CTRect<T> res(r); res += sz; return res; }

}
