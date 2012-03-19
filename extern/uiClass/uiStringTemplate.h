// uiStringTemplate.h
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 7/3/2009
//

#ifndef _UISTRINGTEMPLATE_
#define _UISTRINGTEMPLATE_

#include "uiInclude.h"
#include "uiString.h"

#define UISTRING_CAPACITY	20

namespace uiClass
{	

	template <class T>
	class DLLEXPORT uiStringTemplate: public uiObject
	{
	protected:
		T*		zm_char;
		DWORD	zm_count;
		DWORD	zm_capacity;		
	protected:
		// Thay doi kich thuoc mang
		void setCapacity(DWORD size, bool bCopyMemory = true)
		{			
			// Cap phat vung nho moi
			T* newArray = new T[ size ];
			
			// Copy noi dung cu sang
			if ( bCopyMemory )
				memcpy( newArray, zm_char, zm_capacity*sizeof(T) );

			delete zm_char;
			
			zm_char		= newArray;
			zm_capacity = size;
		}
		
		// calcSize
		// Tinh toan kich thuoc moi
		DWORD calcSize( DWORD size )
		{
			DWORD capacity = zm_capacity;
			
			// Tang gap doi
			while ( capacity <= (size+1) )
				capacity *= 2;

			return capacity;
		}

	public:		
		uiStringTemplate()
		{
			zm_capacity = UISTRING_CAPACITY;
			zm_count = 0;			
			zm_char = new T[ zm_capacity ];
		}

		uiStringTemplate( const uiStringTemplate& a )
		{
			zm_capacity = UISTRING_CAPACITY;
			zm_count = 0;			
			zm_char = new T[ zm_capacity ];

			*this = a;
		}

		uiStringTemplate(T* lpString)
		{
			zm_capacity = UISTRING_CAPACITY;
			zm_count = 0;			
			zm_char = new T[ zm_capacity ];

			if ( lpString == NULL )
			{			
				this->zm_char[0] = NULL;
				this->zm_count = 0;			
				return;
			}

			DWORD len = uiString::length<T>( lpString );
			
			// Cap phat kich thuoc
			this->setCapacity( this->calcSize(len) ,false);
			uiString::copy<T,T>( zm_char, lpString );

			zm_count = len;
		}

		uiStringTemplate(const T* lpString)
		{
			zm_capacity = UISTRING_CAPACITY;
			zm_count = 0;			
			zm_char = new T[ zm_capacity ];

			
			if ( lpString == NULL )
			{			
				this->zm_char[0] = NULL;
				this->zm_count = 0;			
				return;
			}


			DWORD len = uiString::length<T>( (T*) lpString );
			
			// Cap phat kich thuoc
			this->setCapacity( this->calcSize(len) ,false);
			uiString::copy<T,T>( zm_char, (T*) lpString );

			zm_count = len;
		}

		virtual ~uiStringTemplate()
		{
			delete zm_char;
		}
		
		// c
		// Tra ve mang cua chuoi
		T* c()
		{			
			return zm_char;
		}

		// getLength
		// Tra ve chuoi ky tu
		DWORD getLength()
		{
			return zm_count;
		}

		bool format(T* lpFormat, ... )
		{
			// Khoi tao danh sach tham so
			va_list listArgs;
			va_start(listArgs,lpFormat);			

			T lpString[800];
			WCHAR lpWFormat[800];
			WCHAR lpWResult[800];

			// Chuyen format sang Unicode
			uiString::copy<WCHAR,T>(lpWFormat, lpFormat);
			
			// Dinh dang sang Unicode
			#ifdef __GNUG__
				int i = vswprintf(lpWResult,lpWFormat,listArgs);
			#else
				int i = vswprintf(lpWResult,800,lpWFormat,listArgs);
			#endif

			if (i == -1)
				return false;

			// Dinh dang ket qua sang template <T>
			uiString::copy<T,WCHAR>( lpString, lpWResult);

			*this = lpString;

			return true;
		}


		// operator =
		// Phep gan giua 2 string
		uiStringTemplate<T> & operator=(const uiStringTemplate<T>& string)
		{						
			if ( string.zm_count == 0 )
			{			
				this->zm_char[0] = NULL;
				this->zm_count = 0;			
				return *this;
			}

			// Neu kich thuoc khong du -> tinh kich thuoc moi
			if ( string.zm_count >= this->zm_capacity )			
				// Thay doi kich thuoc
				this->setCapacity( this->calcSize(string.zm_count), false );			
			
			// Copy noi dung tu string qua
			uiString::copy<T>( this->zm_char, string.zm_char );
			this->zm_count = string.zm_count;			

			return *this;
		}

		// operator =
		// Phep gan giua 2 string
		uiStringTemplate<T>& operator=(T* string)
		{			
			if ( string == NULL )
			{
				this->zm_char[0] = NULL;
				this->zm_count = 0;
				return *this;
			}

			DWORD len = uiString::length<T>( string );

			// Neu kich thuoc khong du -> tinh kich thuoc moi
			if ( len >= this->zm_capacity )			
				// Thay doi kich thuoc
				this->setCapacity( this->calcSize(len), false );			
			
			// Copy noi dung tu string qua
			uiString::copy<T>( this->zm_char, string );
			this->zm_count = len;			

			return *this;
		}
		
		// operator =
		// Phep gan giua 2 string
		uiStringTemplate<T>& operator=(const T* string)
		{			
			if ( string == NULL )
			{
				this->zm_char[0] = NULL;
				this->zm_count = 0;
				return *this;
			}

			DWORD len = uiString::length<T>( (T*)string );

			// Neu kich thuoc khong du -> tinh kich thuoc moi
			if ( len >= this->zm_capacity )			
				// Thay doi kich thuoc
				this->setCapacity( this->calcSize(len), false );			
			
			// Copy noi dung tu string qua
			uiString::copy<T>( this->zm_char, (T*)string );
			this->zm_count = len;			

			return *this;
		}

		// Operator []
		T operator[]( WORD index )
		{
			return zm_char[index];
		}

		// setChar
		// Thiet lap ky tu cho string
		void setChar( WORD index, T c )
		{
			if ( index < zm_count )
				zm_char[index] = c;
		}

		// operator==
		bool operator==( uiStringTemplate<T>&a )
		{
			int k = uiString::comp<T>( zm_char, a.zm_char );
			if ( k == 0 )
				return true;
			return false;
		}

		// operator==
		bool operator==( T* s )
		{
			int k = uiString::comp<T>( zm_char, s );
			if ( k == 0 )
				return true;
			return false;
		}
		
		// operator>
		bool operator>( uiStringTemplate<T>&a )
		{
			int k = uiString::comp<T>( zm_char, a.zm_char );
			if ( k > 0 )
				return true;
			return false;
		}
		
		// operator>
		bool operator>( T* s )
		{
			int k = uiString::comp<T>( zm_char, s );
			if ( k > 0 )
				return true;
			return false;
		}

		// operator<
		bool operator<( uiStringTemplate<T>&a )
		{
			int k = uiString::comp<T>( zm_char, a.zm_char );
			if ( k < 0 )
				return true;
			return false;
		}

		// operator<
		bool operator<( T* s )
		{
			int k = uiString::comp<T>( zm_char, s );
			if ( k < 0 )
				return true;
			return false;
		}

		// operator+ function
		// Ghep 2 chuoi string
		friend uiStringTemplate<T> operator+( uiStringTemplate<T>&a, uiStringTemplate<T>&b)
		{
			a.refClear();
			b.refClear();

			DWORD newLen = a.zm_count + b.zm_count + 2;
			T* lpString = new T[newLen];

			uiString::copy<T,T>		( lpString, a.zm_char );
			uiString::copyAt<T,T>	( lpString, b.zm_char, a.zm_count);

			uiStringTemplate<T>* result = b.ref< uiStringTemplate<T> > ( new uiStringTemplate<T>(lpString) );			

			delete lpString;		
			return *result;
		}
		
		// operator+ function
		// Ghep 1 chuoi va 1 T*
		friend uiStringTemplate<T> operator+( uiStringTemplate<T>&a, T* p)
		{
			a.refClear();

			DWORD strlen = uiString::length<T>( p );	

			DWORD newLen = a.zm_count + strlen + 2;
			T* lpString = new T[newLen];

			uiString::copy<T,T>		( lpString, a.zm_char );
			uiString::copyAt<T,T>	( lpString, p, a.zm_count);

			uiStringTemplate<T>* result = a.ref< uiStringTemplate<T> > ( new uiStringTemplate<T>(lpString) );			

			delete lpString;		
			return *result;
		}

		// operator+ function
		// Ghep 1 chuoi va 1 T*
		friend uiStringTemplate<T> operator+(T* p, uiStringTemplate<T>&a)
		{
			a.refClear();

			DWORD strlen = uiString::length<T>( p );	

			DWORD newLen = a.zm_count + strlen + 2;
			T* lpString = new T[newLen];

			uiString::copy<T,T>		( lpString, p );
			uiString::copyAt<T,T>	( lpString, a.zm_char, strlen);

			uiStringTemplate<T>* result = a.ref< uiStringTemplate<T> > ( new uiStringTemplate<T>(lpString) );			

			delete lpString;		
			return *result;
		}

		// operator+=
		uiStringTemplate<T>& operator+=( uiStringTemplate<T>&a )
		{
			a.refClear();
			refClear();
						
			DWORD newLen = zm_count + a.zm_count;
			
			// Cap phat kich thuoc
			if ( newLen >= zm_capacity )
				this->setCapacity( this->calcSize(newLen) ,true);

			// Truong hop string2 += string2
			if ( zm_char == a.zm_char )
			{
				T* lpString = new T[ zm_count + 1 ];
				uiString::copy<T,T>( lpString, a.zm_char );
				uiString::copyAt<T,T>( zm_char, lpString, zm_count);
				delete lpString;
			}
			// Truong hop sting1 += string2
			else
				uiString::copyAt<T,T>( zm_char, a.zm_char, zm_count);
			
			zm_count = newLen;			

			return *this;
		}

		// operator+=
		uiStringTemplate<T>& operator+=( T* a )
		{			
			refClear();
			
			DWORD newLen = zm_count + uiString::length<T>(a);
			
			// Cap phat kich thuoc
			if ( newLen >= zm_capacity )
				this->setCapacity( this->calcSize(newLen) ,true);

			uiString::copyAt<T,T>( zm_char, a, zm_count);
			
			zm_count = newLen;
			return *this;
		}

		// operator+=
		uiStringTemplate<T>& operator+=( const T* a )
		{			
			refClear();
			
			DWORD newLen = zm_count + uiString::length<T>( (T*)a);
			
			// Cap phat kich thuoc
			if ( newLen >= zm_capacity )
				this->setCapacity( this->calcSize(newLen) ,true);

			uiString::copyAt<T,T>( zm_char, (T*)a, zm_count);
			
			zm_count = newLen;
			return *this;
		}

		// find
		// Vi tri cua chuoi dua vao
		int find( uiStringTemplate<T> &a, DWORD begin = 0 )
		{
			refClear();
			return uiString::find( zm_char, a.zm_char, (WORD)begin );
		}

		// find
		// Vi tri cua chuoi dua vao
		// Tra ve -1 neu khong tim thay
		int find( T *lpString, DWORD begin = 0 )
		{
			refClear();
			return uiString::find( zm_char, lpString, (WORD)begin );
		}
		
		// trimLeft
		// Cat bo cac khoang trong ben trai
		void trimLeft()
		{
			refClear();
			uiString::trimleft<T>( zm_char );
			zm_count = uiString::length<T>( zm_char );
		}

		// trimRight
		// Cat bo cac khoang trong ben phai
		void trimRight()
		{
			refClear();
			uiString::trimright<T>( zm_char );
			zm_count = uiString::length<T>( zm_char );
		}

		// trim
		// Cat bo cac khoang trong o 2 ben
		void trim()
		{
			refClear();
			uiString::trimleft<T>( zm_char );
			uiString::trimright<T>( zm_char );
			zm_count = uiString::length<T>( zm_char );			
		}

		// mid
		// Lay chuoi o giua
		uiStringTemplate<T> mid( DWORD begin, DWORD to )
		{
			refClear();

			T* lpString = new T[ zm_count + 1];
			// Lay ky tu o giua
			uiString::mid( lpString, zm_char, begin, to );
			// Khoi tao doi tuong
			uiStringTemplate<T>* pResult = ref< uiStringTemplate<T> > (new uiStringTemplate<T> ( lpString ));
			delete lpString;
			
			// Tra ve ket qua
			return *pResult;
		}

		// toLower
		// Chuyen thanh chu thuong
		void toLower()
		{
			refClear();
			uiString::toLower<T>( zm_char );
		}

		// toUpper
		// Chuyen thanh chu hoa
		void toUpper()
		{
			refClear();
			uiString::toUpper<T>(zm_char);
		}

		// toInt
		// Chuyen thanh int
		// pResult = 0 neu loi chuyen doi
		int toInt(int *pResult=NULL)
		{
			refClear();
			int result=0;
			if ( uiString::parseToInt<T>( zm_char, &result ) )
			{
				if ( pResult )
					*pResult = 1;
			}
			else
			{
				if ( pResult )
					*pResult = 0;
			}
			return result;
		}

		// toFloat
		// Chuyen thanh float
		float toFloat(int *pResult=NULL)
		{
			refClear();
			float result=0;
			if ( uiString::parseToFloat<T>( zm_char, &result ) )
			{
				if ( pResult )
					*pResult = 1;
			}
			else
			{
				if ( pResult )
					*pResult = 0;
			}
			return result;
		}

		// toDWORD
		// Chuyen thanh DWORD
		DWORD toDWORD(int *pResult=NULL)
		{
			refClear();
			DWORD result;
			if ( uiString::parseToDWORD<T>(zm_char, &result) )
			{
				if ( pResult )
					*pResult = 1;
			}
			else
			{
				if ( pResult )
					*pResult = 0;
			}
			return result;

		}

		// inorgeLoopChar
		// Loai bo cac ky tu bi lap (vi du: "this  is     my text  " -> "this is my text ")
		void inorgeLoopChar( T c )
		{
			refClear();
			uiString::inorgeLoopChar<T>( zm_char, c );
			zm_count = uiString::length<T>( zm_char );			
		}
	
		// Operator*
		// Tra ve chuoi ky tu
		T* operator*()
		{		
			
			return zm_char;
		}

		// split
		// Tach chuoi thanh nhieu chuoi con dua tren chuoi split
		//		pos:	Vi tri bat dau split
		//		nSplit:	So chuoi con se cat ( 0 se cat het )
		void split( uiStringTemplate<T>& splitString, std::vector< uiStringTemplate<T> >* pArray, DWORD *pos = 0, DWORD nSplit = 0 )
		{
			refClear();
			int iSplit = 0;

			T* lpResult = new T[zm_capacity];
			
			DWORD npos = 0;

			while ( uiString::split<T>( lpResult, zm_char, *splitString, &npos ) )
			{		
				pArray->push_back( lpResult );

				// Tang so lan dem split
				iSplit++;
				
				if ( pos )
					*pos = npos;

				// Neu co gioi han so luong split
				if ( nSplit )
				{
					if ( iSplit == nSplit )
						break;
				}

			}

			delete lpResult;
		}

		// split
		// Tach chuoi thanh nhieu chuoi con dua tren chuoi split
		//		pos:	Vi tri bat dau split
		//		nSplit:	So chuoi con se cat ( 0 se cat het )
		void split( T* lpSplitString, std::vector< uiStringTemplate<T> >* pArray, DWORD *pos = 0, DWORD nSplit = 0 )
		{
			refClear();
			int iSplit = 0;

			T* lpResult = new T[zm_capacity];
			
			DWORD npos = 0;

			while ( uiString::split<T>( lpResult, zm_char, lpSplitString, &npos ) )
			{		
				pArray->push_back( lpResult );

				// Tang so lan dem split
				iSplit++;
				
				if ( pos )
					*pos = npos;

				// Neu co gioi han so luong split
				if ( nSplit )
				{
					if ( iSplit == nSplit )
						break;
				}

			}			
						
			delete lpResult;
		}

	};


	typedef uiStringTemplate<char> uiStringA;
	typedef uiStringTemplate<WCHAR> uiStringW;

}

#endif
