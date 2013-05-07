namespace HTS_NAMESPACE {

	// /////////////////////////////////////////////////////////////////////////////////////////
	/// \defgroup maths Classes et fonctions mathématiques

	/// \defgroup CTVector_operators Opérateurs arithmétiques sur CTVector
	/// \ingroup maths
	/// \brief voir @link CTVector CTVector @endlink.

	/// \defgroup CTMatrix_operators Opérateurs arithmétiques sur CTMatrix
	/// \ingroup maths
	/// \brief voir @link CTMatrix CTMatrix @endlink.

	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
		\ingroup maths
		\class CTVector
		\brief Vecteur Templaté
	*/

	template <int C, typename T=double>
	class CTVector
	{
	public:
		CTVector();
		CTVector( const T* v );
		CTVector( const T v );
		CTVector(const CTVector & src);
		CTVector & operator=(const CTVector & src);

	public:
		CTVector( const T a , const T b);
		CTVector( const T a , const T b, const T c);
		CTVector( const T a , const T b, const T c, const T d);
		CTVector( const T a , const T b, const T c, const T d, const T e);
		CTVector( const T a , const T b, const T c, const T d, const T e, const T f);

	public: // Row access
		const T & operator[](int l) const;
		T & operator[](int l);

	public:
		bool operator==(const CTVector<C,T> & b) const;
		bool operator!=(const CTVector<C,T> & b) const;

	public:
		void operator -=(const CTVector<C,T> & d);
		void operator -=(const T d);
		void operator +=(const CTVector<C,T> & d);
		void operator +=(const T d);
		void operator /=(const T d);
		void operator *=(const T d);

	public:
		T length() const;  // squared length of a CTVector
		T length2() const; // length of a CTVector
		T norm() const;
		CTVector<C,T> normalize() const;

	protected:
		T m_v[C];
	};
	template <int C, typename T>
	CTVector<C,T> prod(const CTVector<C,T> & a, const CTVector<C,T> & b);
	template <typename T>
	CTVector<3,T> crossProduct(const CTVector<3,T> & a, const CTVector<3,T> & b);
	template <typename T>
	T tripleProduct(const CTVector<3,T> & a, const CTVector<3,T> & b, const CTVector<3,T> & c);


	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
		\ingroup maths
		\class CTMatrix
		\brief Matrice Templaté
	*/

	template <int L, int C, typename T= double>
	class CTMatrix
	{
	public:
		CTMatrix( );
		CTMatrix( const T i);
		CTMatrix( const CTMatrix<L,C,T> & src);
		CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b);
		CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c);
		CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c, const CTVector<C,T> & d);
		CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c, const CTVector<C,T> & d, const CTVector<C,T> & e);
		CTMatrix( const CTVector<C,T> & a, const CTVector<C,T> & b, const CTVector<C,T> & c, const CTVector<C,T> & d, const CTVector<C,T> & e, const CTVector<C,T> & f);
		CTMatrix( const CTVector<C,T> & d);

	public: // Row access
		const CTVector<C,T> & operator[](int l) const;
		CTVector<C,T> & operator[](int l);
		void setCol( int iCol, const CTVector<L,T> & col);

	public:
		void swapRows(int r1, int r2);
		CTMatrix<C,L,T> transpose() const;

	public:
		bool isInversible() const;
		CTMatrix<L,C,T> getInverse() const;
		bool inverse(CTMatrix<L,C,T> & b) const;
		bool inverse();
		T det() const;

	protected:
		CTVector<C,T> m_v[L];

	public: // statics
		static CTMatrix<L,C,T> identity( );
		static CTMatrix<L,C,T> translation( const CTVector< L-1, T> & v);
		static CTMatrix<L,C,T> scaling    ( const CTVector< L-1, T> & k);
	};

	template<int M, int N, int P, typename T> // M,N x N,P
	CTMatrix<M,P,T> operator*( const CTMatrix<M,N,T>& a, const CTMatrix<N,P,T>& b);
	template<int L, int C, typename T> // L,C x C,1
	CTVector<L,T> operator*( const CTMatrix<L,C,T>& a, const CTVector<C,T>& b);
	template<int L, int C, typename T> // 1,L x L,C
	CTVector<C,T> operator*( const CTVector<L,T>& a, const CTMatrix<L,C,T>& b);

	template<int L, int C, typename T> bool inverse(CTMatrix<L,C,T> & m);
	template<int L, int C, typename T> T det(CTMatrix<L,C,T> & m);

	template<int L, int C, typename T> CTMatrix<L,C,T> setCol( const CTMatrix<L,C,T> & m, int iCol, const CTVector<L,T> & col);
	template<int L, typename T> bool resolveCramer( const CTMatrix<L,L,T> & A, CTVector<L,T> & X, const CTVector<L,T> & B );

	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef CTVector<2,int> CVector2i;
	typedef CTVector<3,int> CVector3i;
	typedef CTVector<4,int> CVector4i;
	typedef CTVector<2,float> CVector2f;
	typedef CTVector<3,float> CVector3f;
	typedef CTVector<4,float> CVector4f;
	typedef CTVector<2,double> CVector2d;
	typedef CTVector<3,double> CVector3d;
	typedef CTVector<4,double> CVector4d;

	typedef CTMatrix<2,2,int> CMatrix2i;
	typedef CTMatrix<3,3,int> CMatrix3i;
	typedef CTMatrix<4,4,int> CMatrix4i;
	typedef CTMatrix<2,2,float> CMatrix2f;
	typedef CTMatrix<3,3,float> CMatrix3f;
	typedef CTMatrix<4,4,float> CMatrix4f;
	typedef CTMatrix<2,2,double> CMatrix2d;
	typedef CTMatrix<3,3,double> CMatrix3d;
	typedef CTMatrix<4,4,double> CMatrix4d;

	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#include "matrix.hpp"

