namespace HTS_NAMESPACE {

	// Vector inlines //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <int C, typename T> inline CTVector<C,T>::CTVector() { for (int c=0; c<C; ++c) m_v[c] = T(0); }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const T* v ) { memcpy(m_v, v, sizeof(m_v)); }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const T v ) { for (int c=0; c<C; ++c) m_v[c] = v; }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const CTVector<C,T> & src) { for (int c=0; c<C; ++c) m_v[c] = src.m_v[c]; }
	template <int C, typename T> inline CTVector<C,T> & CTVector<C,T>::operator=(const CTVector & src) { if (this != &src) for (int c=0; c<C; ++c) m_v[c] = src.m_v[c]; return (*this); }

	template <int C, typename T> inline CTVector<C,T>::CTVector( const T a , const T b) { STATIC_ASSERT<(C == 2)>(); m_v[0]= a; m_v[1]= b; }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const T a , const T b, const T c) {  STATIC_ASSERT<(C == 3)>(); m_v[0]= a; m_v[1]= b; m_v[2]= c; }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const T a , const T b, const T c, const T d) { STATIC_ASSERT<(C == 4)>(); m_v[0]= a; m_v[1]= b; m_v[2]= c; m_v[3]= d; }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const T a , const T b, const T c, const T d, const T e) { STATIC_ASSERT<(C == 5)>(); m_v[0]= a; m_v[1]= b; m_v[2]= c; m_v[3]= d; m_v[4]= e; }
	template <int C, typename T> inline CTVector<C,T>::CTVector( const T a , const T b, const T c, const T d, const T e, const T f) { STATIC_ASSERT<(C == 6)>(); m_v[0]= a; m_v[1]= b; m_v[2]= c; m_v[3]= d; m_v[4]= e; m_v[5]= f; }

	template <int C, typename T> inline const T & CTVector<C,T>::operator[](int l) const { assert((l>=0) && (l < C)); return m_v[l]; }
	template <int C, typename T> inline T & CTVector<C,T>::operator[](int l) { assert((l>=0) && (l < C)); return m_v[l]; }

	template <int C, typename T> inline bool CTVector<C,T>::operator==(const CTVector<C,T> & b) const { for (int c=0; c<C; ++c) if (m_v[c] != b.m_v[c]) return false; return true; }
	template <int C, typename T> inline bool CTVector<C,T>::operator!=(const CTVector<C,T> & b) const { return !(operator==(b)); }

	template <int C, typename T> inline void CTVector<C,T>::operator +=(const CTVector<C,T> & d) { for (int c=0; c<C; ++c) m_v[c] += d.m_v[c]; }
	template <int C, typename T> inline void CTVector<C,T>::operator -=(const CTVector<C,T> & d) { for (int c=0; c<C; ++c) m_v[c] -= d.m_v[c]; }
	template <int C, typename T> inline void CTVector<C,T>::operator +=(const T d) { for (int c=0; c<C; ++c) m_v[c] += d; }
	template <int C, typename T> inline void CTVector<C,T>::operator -=(const T d) { for (int c=0; c<C; ++c) m_v[c] -= d; }
	template <int C, typename T> inline void CTVector<C,T>::operator /=(const T d) { for (int c=0; c<C; ++c) m_v[c] /= d; }
	template <int C, typename T> inline void CTVector<C,T>::operator *=(const T d) { for (int c=0; c<C; ++c) m_v[c] *= d; }

	template <int C, typename T> inline CTVector<C,T> CTVector<C,T>::normalize() const { return *this / length(); }
	template <int C, typename T> inline T CTVector<C,T>::norm () const { return length(); }
	template <int C, typename T> inline T CTVector<C,T>::length () const { return sqrt(length2()); }
	template <int C, typename T> inline T CTVector<C,T>::length2() const { T res(0); for (int c=0; c<C; ++c) res += m_v[c]*m_v[c]; return res; }

	// Matrix inlines //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( ) {  }
	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const T i) { for (int l=0;l<L;++l) m_v[l] = CTVector<C,T>(i); }
	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTMatrix<L,C,T> & src) { for (int l=0;l<L;++l) m_v[l] = src.m_v[l]; }

	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b) { STATIC_ASSERT< (L==2) >(); m_v[0] = a; m_v[1] = b; }
	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c) { STATIC_ASSERT< (L==3) >(); m_v[0] = a; m_v[1] = b; m_v[2] = c; }
	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c, const CTVector<C,T> & d) { STATIC_ASSERT< (L==4) >(); m_v[0] = a; m_v[1] = b; m_v[2] = c;; m_v[3] = d; }
	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c, const CTVector<C,T> & d, const CTVector<C,T> & e) { STATIC_ASSERT< (L==5) >(); m_v[0] = a; m_v[1] = b; m_v[2] = c; m_v[3] = d; m_v[4] = e; }
	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c, const CTVector<C,T> & d, const CTVector<C,T> & e, const CTVector<C,T> & f) { STATIC_ASSERT< (L==6) >(); m_v[0] = a; m_v[1] = b; m_v[2] = c; m_v[3] = d; m_v[4] = e; m_v[5] = f; }

	template <int L, int C, typename T> inline CTMatrix<L,C,T>::CTMatrix( const CTVector<C,T> & d) { STATIC_ASSERT< (C==L) >(); for (int i=0; i<L; ++i) for (int j=0; j<C; ++j) m_v[i][j] = (i == j) ? d[i] : 0; }

	template <int L, int C, typename T> inline const CTVector<C,T> & CTMatrix<L,C,T>::operator[](int l) const { assert((l>=0) && (l < L)); return m_v[l]; }
	template <int L, int C, typename T> inline CTVector<C,T> & CTMatrix<L,C,T>::operator[](int l) { assert((l>=0) && (l < L)); return m_v[l]; }
	template <int L, int C, typename T> inline void CTMatrix<L,C,T>::setCol( int iCol, const CTVector<L,T> & col) { ASSERT((iCol>=0) && (iCol<C)); for (int l=0; l<L; ++l) m_v[l][iCol] = col[l]; }

	template <int L, int C, typename T> inline void CTMatrix<L,C,T>::swapRows(int r1, int r2) { if (r1 != r2) { CTVector<C,T> tmp = m_v[r1]; m_v[r1] = m_v[r2]; m_v[r2] = tmp; }}


	// Vector operators //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <int C, typename T> inline CTVector<C,T> operator -(const CTVector<C,T> & a)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = -a[c]; return res; }

	template <int C, typename T> inline CTVector<C,T> operator -(const CTVector<C,T> & a, const T b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] - b ; return res; }
	template <int C, typename T> inline CTVector<C,T> operator -(const T a, const CTVector<C,T> & b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a - b[c] ; return res; }
	template <int C, typename T> inline CTVector<C,T> operator -(const CTVector<C,T> & a, const CTVector<C,T> & b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] - b[c] ; return res; }

	template <int C, typename T> inline CTVector<C,T> operator +(const CTVector<C,T> & a, const CTVector<C,T> & b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] + b[c] ; return res; }

	template <int C, typename T> inline CTVector<C,T> operator +(const T a, const CTVector<C,T> & b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a + b[c] ; return res; }

	template <int C, typename T> inline CTVector<C,T> operator +(const CTVector<C,T> & a, const T b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] + b ; return res; }

	template <int C, typename T> inline CTVector<C,T> operator *(const CTVector<C,T> & a, const T b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] * b; return res; }

	template <int C, typename T> inline CTVector<C,T> operator *(const T a, const CTVector<C,T> & b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a * b[c]; return res; }

	template <int C, typename T> inline CTVector<C,T> operator ^(const CTVector<C,T> & a, const T b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = (T) std::pow((double) a[c], (double) b); return res; }


	template <typename T> inline CTVector<2,T> prod(const CTVector<2,T> & a, const CTVector<2,T> & b)
	{ return CTVector<2,T>( a[0] * b[0],a[1] * b[1]); }
	template <typename T> inline CTVector<3,T> prod(const CTVector<3,T> & a, const CTVector<3,T> & b)
	{ return CTVector<3,T>( a[0] * b[0],a[1] * b[1],a[2] * b[2]); }
	template <typename T> inline CTVector<4,T> prod(const CTVector<4,T> & a, const CTVector<4,T> & b)
	{ return CTVector<4,T>( a[0] * b[0],a[1] * b[1],a[2] * b[2],a[3] * b[3]); }
	template <int C, typename T> inline CTVector<C,T> prod(const CTVector<C,T> & a, const CTVector<C,T> & b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] * b[c]; return res; }

	template <typename T> inline CTVector<3,T> crossProduct(const CTVector<3,T> & a, const CTVector<3,T> & b)
	{
		return CTVector<3,T> (
				a[1] * b[2] - a[2] * b[1],
				a[2] * b[0] - a[0] * b[2],
				a[0] * b[1] - a[1] * b[0]);
	}

	template <typename T> inline T tripleProduct(const CTVector<3,T> & a, const CTVector<3,T> & b, const CTVector<3,T> & c)
	{
		return a * crossProduct(b, c);
	}

	template <int C, typename T> inline T operator *(const CTVector<C,T> & a, const CTVector<C,T> & b)
	{ T res(0); for (int c=0; c<C; ++c) res += (a[c] * b[c]); return res; }

	template <int C, typename T> inline CTVector<C,T> operator /(const CTVector<C,T> & a, const T b)
	{ CTVector<C,T> res; for (int c=0; c<C; ++c) res[c] = a[c] / b; return res; }

	// Matrix operators //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<int M, int N, int P, typename T> // M,N x N,P
	CTMatrix<M,P,T> operator*( const CTMatrix<M,N,T>& a, const CTMatrix<N,P,T>& b)
	{
		CTMatrix<M,P,T> resM;
		for (int m=0; m<M; ++m)
			for (int p=0; p<P; ++p) {

				T res(0);
				for (int r=0; r<N; ++r)
					res += a[m][r] * b[r][p];

				resM[m][p]= res;
			}
		return resM;
	}

	template<int L, int C, typename T> // L,C x C,1
	inline CTVector<L,T> operator*( const CTMatrix<L,C,T>& a, const CTVector<C,T>& b)
	{
		CTVector<L,T> resV(0.);
		for (int l=0; l<L; ++l)
			for (int c=0; c<C; ++c)
				resV[l] += ( a[l][c] * b[c] );

		return resV;
	}

	template<int L, int C, typename T> // 1,L x L,C
	inline CTVector<C,T> operator*( const CTVector<L,T>& a, const CTMatrix<L,C,T>& b)
	{
		CTVector<C,T> resV(0.);
		for (int c=0; c<C; ++c)
			for (int l=0; l<L; ++l)
				resV[c] += ( a[l] * b[l][c] );

		return resV;
	}

	// MATRIX functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<int L, int C, typename T>
	CTMatrix<C,L,T> CTMatrix<L,C,T>::transpose() const
	{
		CTMatrix<C,L,T> res;
		for (int l=0; l<L; ++l)
			for (int c=0; c<C; ++c)
				res[c][l] = m_v[l][c];

		return res;
	}

	// MATRIX statics //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<int L, int C, typename T>
	CTMatrix<L,C,T> CTMatrix<L,C,T>::identity()
	{ STATIC_ASSERT< (L == C) >(); CTMatrix<L,C,T> res; for (int i=0; i<L; ++i) res.m_v[i][i] = T(1); return res; }

	template<>
	inline CTMatrix<3,3,double> CTMatrix<3,3,double>::translation( const CTVector<2,double> & v)
	{
		return CTMatrix<3,3,double>(
				CTVector<3,double>(1. ,0. ,v[0]),
				CTVector<3,double>(0. ,1. ,v[1]),
				CTVector<3,double>(0. ,0. , 1.)
			);
	}
	template<int L, int C, typename T>
	CTMatrix<L,C,T> CTMatrix<L,C,T>::translation( const CTVector< L-1, T> & v)
	{	STATIC_ASSERT< (L == C) >();
		CTMatrix<L,C,T> res( CTMatrix<L,C,T>::identity() );
		for (int l=0; l<L-1; ++l)
			res.m_v[l][C-1] = v[l];

		return res;
	}
	template<>
	inline CTMatrix<3,3,double> CTMatrix<3,3,double>::scaling( const CTVector<2,double> & k)
	{
		return CTMatrix<3,3,double>(
				CTVector<3,double>( k[0],   0., 0. ),
				CTVector<3,double>( 0.  , k[1], 0. ),
				CTVector<3,double>( 0.  , 0.  , 1. )
			);
	}
	template<int L, int C, typename T>
	CTMatrix<L,C,T> CTMatrix<L,C,T>::scaling( const CTVector< L-1, T> & v)
	{	STATIC_ASSERT< (L == C) >();
		CTMatrix<L,C,T> res( T(0) );
		for (int l=0; l<L-1; ++l)
			res.m_v[l][l] = v[l];

		res.m_v[L-1][C-1] = T(1);

		return res;
	}

	// MATRIX Inversion //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<int L, int C, typename T>
	inline bool inverse(CTMatrix<L,C,T> & m)
	{
		if (L != C) return false;
		CTMatrix<L,C,T> tmp;
		if (!m.inverse( tmp ))
			return false;

		m = tmp;
		return true;
	}

	template <int L, int C, typename T> inline bool CTMatrix<L,C,T>::isInversible() const
	{ if (L != C) return false; CTMatrix<L,C,T> tmp; return inverse(tmp); }

	template <int L, int C, typename T> inline CTMatrix<L,C,T> CTMatrix<L,C,T>::getInverse() const
	{
		STATIC_ASSERT<(L==C)>();
		CTMatrix<L,C,T> tmp;
		VERIFY(inverse(tmp));
		return tmp;
	}

	template <int L, int C, typename T> inline bool CTMatrix<L,C,T>::inverse(CTMatrix<L,C,T> & b) const
	{
		if (L != C) return false;

		CTMatrix<L,L,T> a(*this);	    // As a evolves from original mat into identity
		b= CTMatrix<L,L,T>::identity();   // b evolves from identity into inverse(a)
		int i, j, i1;

		// Loop over cols of a from left to right, eliminating above and below diag
		for (j=0; j<L; j++)
		{
			// Find largest pivot in column j among rows j..3
			i1 = j;		    // Row with largest pivot candidate
			for (i=j+1; i<L; i++)
				if (fabs(a.m_v[i][j]) > fabs(a.m_v[i1][j]))
					i1 = i;

			// Swap rows i1 and j in a and b to put pivot on diagonal
			if (i1 != j) {
				a.swapRows(i1, j);
				b.swapRows(i1, j);
			}

			// Scale row j to have a unit diagonal
			if (a.m_v[j][j] == T(0) )
				return false; // singular matrix; can't invert

			b.m_v[j] /= a.m_v[j][j];
			a.m_v[j] /= a.m_v[j][j];

			// Eliminate off-diagonal elems in col j of a, doing identical ops to b
			for (i=0; i<L; i++)
				if (i!=j)
				{
					b.m_v[i] -= (a.m_v[i][j] * b.m_v[j]);
					a.m_v[i] -= (a.m_v[i][j] * a.m_v[j]);
				}
		}

		return true;
	}

	template <int L, int C, typename T> inline bool CTMatrix<L,C,T>::inverse()
	{
		if (L!=C) return false;
		CTMatrix<L,C,T> tmp;
		if (!inverse(tmp))
			return false;

		operator=(tmp);
		return true;
	}





	template<typename T>
	inline T det(const CTMatrix<1,1,T> & m)
	{
		return m[0][0];
	}

	template<typename T>
	inline T det(const CTMatrix<2,2,T> & m)
	{
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}

	template<typename T>
	inline T det(const CTMatrix<3,3,T> & m)
	{
		return m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]
			   - m[2][0] * m[1][1] * m[0][2] - m[2][1] * m[1][2] * m[0][0] - m[2][2] * m[1][0] * m[0][1];
	}

	template<int L, int C, typename T>
	inline T det(const CTMatrix<L,C,T> & m)
	{
		STATIC_ASSERT<((L==C))>();

		T res = (T) 0.0;
		for (int i=0; i<L; ++i)
		{
			CTMatrix<L-1, L-1, T> subMatrix;

			for (int l=0; l<L; ++l)
			{
				for (int c=0; c<L-1; ++c)
				{
					if (l<i)
						subMatrix[l][c] = m[l][c+1];
					else if (l>i)
						subMatrix[l-1][c] = m[l][c+1];
				}
			}

			res += subMatrix.det() * ((i%2) ? -m[i][0] : m[i][0]);
		}
		return res;
	}

	template <int L, int C, typename T>
	inline T CTMatrix<L,C,T>::det() const
	{
		return HTS_NAMESPACE::det(*this);
	}


	// MATRIX Resolve //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<int L, int C, typename T>
	inline CTMatrix<L,C,T> setCol( const CTMatrix<L,C,T> & m, int iCol, const CTVector<L,T> & col)
	{
		CTMatrix<L,C,T> result(m);
		result.setCol( iCol, col );
		return result;
	}

	template<typename T>
	bool resolveCramer( const CTMatrix<2,2,T> & A, CTVector<2,T> & X, const CTVector<2,T> & B )
	{
		const T D = A.det();
		if (D == 0.)
			return false;

		X = CTVector<2,T>(
			setCol( A, 0, B).det() / D,
			setCol( A, 1, B).det() / D
		);

		return true;
	}

	template<typename T>
	bool resolveCramer( const CTMatrix<3,3,T> & A, CTVector<3,T> & X, const CTVector<3,T> & B )
	{
		const T D = A.det();
		if (D == 0.)
			return false;

		X = CTVector<3,T>(
			setCol( A, 0, B).det() / D,
			setCol( A, 1, B).det() / D,
			setCol( A, 2, B).det() / D
		);

		return true;
	}

	template<typename T>
	bool resolveCramer( const CTMatrix<4,4,T> & A, CTVector<4,T> & X, const CTVector<4,T> & B )
	{
		const T D = A.det();
		if (D == 0.)
			return false;

		X = CTVector<4,T>(
			setCol( A, 0, B).det() / D,
			setCol( A, 1, B).det() / D,
			setCol( A, 2, B).det() / D,
			setCol( A, 3, B).det() / D
		);

		return true;
	}

	template<int L, typename T>
	bool resolveCramer( const CTMatrix<L,L,T> & A, CTVector<L,T> & X, const CTVector<L,T> & B )
	{
		const T D = A.det();
		if (D == 0.)
			return false;

		const CTVector<L,T> determinants;
		for (int l=0; l<L; ++l)
			X[l] = setCol( A, l, B).det() / D;

		return true;
	}

}
