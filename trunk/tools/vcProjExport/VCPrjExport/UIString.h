// uiString.h
// Lap trinh vien: Pham Hong Duc
// Email: hongduc.pr@gmail.com
// Ngay viet: 14/12/2008
//
// uiString ho tro cho viec decode, parse text string
// Day la 1 file header trong thanh phan cua thu vien uiClass

#ifndef _UISTRING_
#define _UISTRING_

#define DLLEXPORT

#include <string.h>
#include <windows.h>

//namespace uiClass
//{
	// uiString
	// - static class ho tro xu ly string
	class DLLEXPORT uiString
	{
	public:
		// length
		// Tra ve do dai cua 1 string
		template <class T>
		static DWORD length(T* pString)
		{
			DWORD result = 0;
			while ( pString[result] != 0 )
				result++;

			return result;
		}
		
		// find
		// Tim kiem 1 ky tu trong 1 chuoi
		template <class T>
		static int find(T* pString, T pCharSearch, int begin = 0)
		{
			int index = begin;

			while ( pString[index] != 0 )
			{
				// Tim thay chuoi can tim
				if ( pString[index] == pCharSearch )
					return index;

				index++;
			}

			return -1;
		}

		// find
		// Tim kiem 1 chuoi trong 1 chuoi
		template <class T>
		static int find(T* pString, T* pKeySearch, int begin = 0)
		{
			DWORD size = uiString::length<T>(pKeySearch)*sizeof(T);
			int index = begin;

			while ( pString[index] != 0 )
			{
				// Tim thay chuoi can tim
				if ( memcmp( &pString[index], pKeySearch, size ) == 0 )
					return index;

				index++;
			}

			return -1;
		}
		
		// trimleft
		// Loai bo cac khoang trong ben trai
		template <class T>
		static void trimleft(T* pString)
		{
			int len = uiString::length<T>(pString);
			if (len == 0)
				return;

			while (  pString[0] != NULL && (pString[0] == ' ' || pString[0] == '\r' || pString[0] == '\n' || pString[0] == '\t' )  )
			{
				len--;
				if (len == 0)
					return;

				memmove((void*) &pString[0], (void*) &pString[1], len*sizeof(T));
				pString[len] = 0;
			}
		}
		
		// trimright
		// Loai bo cac khoang trong ben phai
		template <class T>
		static void trimright(T* pString)
		{
			int len = uiString::length<T>(pString);
			len--;

			for (; len >= 0; len--)
			{
				if ( pString[len] == ' ' || pString[len] == '\r' || pString[len] == '\n' || pString[len] == '\t' )
					pString[len] = NULL;
				else
					return;
			}
		}
		

		// trimleft
		// Loai bo cac khoang trong ben trai
		template <class T>
		static void trimleft(T* pString, T delChar)
		{
			int len = uiString::length<T>(pString);
			if (len == 0)
				return;

			while ( pString[0] != NULL && pString[0] == delChar )
			{
				len--;
				if (len == 0)
					return;

				memmove((void*) &pString[0], (void*) &pString[1], len*sizeof(T));
				pString[len] = 0;
			}
		}
		
		// trimright
		// Loai bo cac khoang trong ben phai
		template <class T>
		static void trimright(T* pString, T delChar)
		{
			int len = uiString::length<T>(pString);
			len--;

			for (; len >= 0; len--)
			{
				if ( pString[len] == delChar )
					pString[len] = NULL;
				else
					return;
			}
		}


		// trim
		// Loai bo cac khoang trong o hai ben
		template <class T>
		static void trim(T* pString)
		{
			uiString::trimleft<T>(pString);
			uiString::trimright<T>(pString);
		}
		
		// trim
		// Loai bo cac khoang trong o hai ben
		template <class T>
		static void trim(T* pString, T delChar)
		{
			uiString::trimleft<T>(pString, delChar);
			uiString::trimright<T>(pString, delChar);
		}
		
		// countEntry
		// Dem so luong ky tu entry trong chuoi
		template <class T>
		static int countEntry(T* pString,T entry, int begin=0)
		{
			int index = begin;
			int result = 0;

			while ( pString[index] != 0 )
			{			
				if ( pString[index] == entry )
					result++;

				index++;
			}

			return result;
		}

		// copy
		// copy tu string src sang string dst
		template <class T1, class T2>
		static int copy(T1* pStringDst, T2* pStringSrc)
		{
			int index = 0;		

			while ( pStringSrc[index] != 0 )
			{						
				pStringDst[index] = (T1) (pStringSrc[index]);
				index++;
			}
			pStringDst[index] = 0;
			return index;
		}

		// copyAt
		// copy string src sang string dst tai 1 vi tri bat ky
		template <class T1, class T2>
		static int copyAt(T1* pStringDst, T2* pStringSrc, DWORD at)
		{
			int index = at;
			int i=0;

			while ( pStringSrc[i] != 0 )
			{						
				pStringDst[index] = (T1) (pStringSrc[i]);
				index++;i++;
			}
			pStringDst[index] = 0;
			return index;
		}

		// cat
		// Noi string src dang sau string dst
		template <class T1, class T2>
		static int cat(T1* pStringDst, T2* pStringSrc)
		{
			int index = uiString::length<T1>(pStringDst);		
			int i = 0;

			while ( pStringSrc[i] != 0 )
			{						
				pStringDst[index] = (T1) (pStringSrc[i]);
				index++;i++;
			}
			pStringDst[index] = 0;
			return i;
		}
		
		// comp
		// So sanh 2 chuoi (khong xac dinh size)
		template <class T>
		static int comp(T* pString1, T* pString2)
		{
			DWORD len1 = uiString::length<T>(pString1);
			DWORD len2 = uiString::length<T>(pString2);
			DWORD len = len1<len2?len1:len2;
			
			int r = memcmp(pString1, pString2, len);
			
			// 2 chuoi bang nhau
			if (r == 0)
			{
				if (len1 == len2)
					return 0;
				if (len1 > len2)
					return 1;
				if (len1 < len2)
					return -1;
			}

			return r;
		}

		// comp
		// So sanh 2 chuoi (xac dinh size)
		template <class T>
		static int comp(T* pString1, T* pString2, DWORD len)
		{
			return memcmp(pString1, pString2, len);		
		}

		// mid
		// Lay chuoi nam giua string src
		template <class T1, class T2>
		static DWORD mid(T1* pStringDst,T2* pStringSrc, DWORD begin, DWORD end)
		{
			DWORD index,i = 0;
			for (index = begin; index < end; index++)
			{
				pStringDst[i] = (T1) pStringSrc[index];
				i++;
			}

			pStringDst[i] = NULL;

			return i;
		}
		
		// getBlock
		// Tim chuoi nam giua 2 ky tu pBlock vi du "@ ...... @"
		template <class T1, class T2>
		static int getBlock(T1* pStringDst,T2* pStringSrc, T2* pBlock, DWORD begin = 0)
		{
			int dwBegin = uiString::find<T2>(pStringSrc,*pBlock,begin);
			if (dwBegin == -1)
				return -1;

			dwBegin++;
			int dwEnd = uiString::find<T2>(pStringSrc,*pBlock,dwBegin);
			if (dwEnd == -1)
				return -1;

			uiString::mid<T1,T2>(pStringDst,pStringSrc, dwBegin,dwEnd);
			return dwEnd+1;
		}

		// findBlock
		// Tim chuoi nam giua cac block "{}" hay "[]" "<>" ...
		template <class T1, class T2>
		static int findBlock(T1* pStringDst,T2* pStringSrc, T2* pBlock, DWORD begin = 0)
		{
			DWORD i = begin;
			int block = 0;
			int midBegin = -1;

			while (pStringSrc[i] != NULL)
			{
				if (pStringSrc[i] == pBlock[0])		// Gap dau '{' hay '[' hay '('
				{
					block++;
					// Root Block 
					if (block == 1)
						midBegin = i+1;
					
					// Bo qua kiem tra "}'
					i++;
					continue;
				}
				if (pStringSrc[i] == pBlock[1])		// Gap dau '}' hay ']' hay ')'
				{
					block--;
					// Root block end can tim
					if (block == 0 && midBegin != -1)
					{
						uiString::mid<T1,T2>(pStringDst,pStringSrc, midBegin,i);
						return i+1;
					}
					
					// Block khong hop le (khong co '{' ma '}')
					if (block <= 0)
						return -1;
				}
				i++;
			}		
			return -1;
		}

		// findBlockString
		// Tim chuoi nam giua cac block "<begin>....<end>" hay "<aTest>...</aTest>"
		template <class T1, class T2>
		static int findBlockString(T1* pStringDst,T2* pStringSrc, T2* pBlock1, T2* pBlock2, DWORD begin = 0)
		{
			DWORD blockLen1 = uiString::length<T2>(pBlock1);
			DWORD blockLen2 = uiString::length<T2>(pBlock2);		
			DWORD len = blockLen1>blockLen2?blockLen1:blockLen2;

			DWORD i = begin;
			int block = 0;
			DWORD midBegin = -1;

			while (pStringSrc[i+len-1] != NULL)
			{
				// Gap dau '<begin>
				if ( uiString::comp<T2>(&pStringSrc[i],pBlock1, blockLen1) == 0)
				{
					block++;
					if (block == 1)
						midBegin = i + uiString::length<T2>(pBlock1);

					i++;
					continue;
				}	
				// Gap dau '</begin>'
				if ( uiString::comp<T2>(&pStringSrc[i],pBlock2, blockLen2) == 0)
				{
					block--;
					// Root block can tim
					if (block == 0 && midBegin != -1)
					{
						uiString::mid<T1,T2>(pStringDst,pStringSrc, midBegin,i);
						return i+1;
					}

					// Block khong hop le...
					if (block <= 0)
						return -1;
				}			
				i++;
			}	
					
			return -1;
		}

		// inorgeLoopChar
		// Bo qua cac char bi lap (vi du: "this  is     my text  " -> "this is my text ")
		template<class T>
		static void inorgeLoopChar(T* pString, T charLoop)
		{
			DWORD i = 0;
			while (pString[i] != NULL)
			{			
				if ( pString[i] == charLoop )
				{
					// Xoa cac ky tu giong no
					while ( pString[i+1] == charLoop && pString[i+1] != NULL )
					{
						DWORD len = uiString::length<T>( &pString[i] );
						memmove(&pString[i], &pString[i+1], len);
					}
				}

				i++;
			}
		}

		// split
		// Tach chuoi lpStringSearch thanh nhieu chuoi lpStrResult dua tren cac ky tu lpStrSplit
		template<class T>
		static bool split(T* lpStrResult,T* lpStringSearch,T* lpStrSplit, DWORD *pos)
		{
			DWORD nLenSearch = uiString::length<T>(lpStringSearch);
			if (*pos >= nLenSearch)
				return false;

			// Tim chieu dai cua chuoi lpStrSplit "|,."
			DWORD nLen = uiString::length<T>(lpStrSplit);
			if ( nLen == 0 )
				return false;

			DWORD i;
			DWORD minPos = -1;

			// Lan dau tien....
			for (i = 0; i < nLen; i++)
			{		
				DWORD tempPos = *pos;
				
				tempPos = uiString::find<T>(lpStringSearch,lpStrSplit[i],tempPos);
				if (tempPos == -1)
					continue;			

				minPos = tempPos;
				break;
			}
			
			// Khong tim thay cac ky tu trong chuoi lpStrSplit trong pString
			// Ket qua chinh la chuoi tim kiem
			if (minPos == -1)
			{
				uiString::mid<T>(lpStrResult, lpStringSearch, *pos, nLenSearch);

				*pos = nLenSearch;
				return true;
			}


			// Truong hop co the split
			for (; i < nLen; i++)
			{
				DWORD tempPos = uiString::find<T>(lpStringSearch,lpStrSplit[i],*pos);			
				// Neu khong tim thay
				if (tempPos == -1)
					continue;
				
				if (minPos > tempPos)
					minPos = tempPos;
			}

			// Thiet lap chuoi moi
			uiString::mid<T>(lpStrResult, lpStringSearch, *pos, minPos);
			*pos = minPos+1;

			return true;
		}
		
		// format
		// Dinh dang string
		template<class T>
		static bool format(T* lpString, T* lpFormat, ... )
		{
			// Khoi tao danh sach tham so
			va_list listArgs;
			va_start(listArgs,lpFormat);

			WCHAR lpWFormat[300];
			WCHAR lpWResult[400];

			// Chuyen format sang Unicode
			uiString::copy<WCHAR,T>(lpWFormat, lpFormat);
			
			// Dinh dang sang Unicode
			#ifdef __GNUG__
				int i = vswprintf(lpWResult, (const T*)lpWFormat,listArgs);
			#else
				int i = vswprintf(lpWResult,400,lpWFormat,listArgs);
			#endif

			if (i == -1)
				return false;

			// Dinh dang ket qua sang template <T>
			uiString::copy<T,WCHAR>(lpString, lpWResult);
			return true;
		}	
		
		// parseToInt
		// Chuyen string thanh int
		template<class T>
		static bool parseToInt(T* lpString, int *result)
		{
			WCHAR lpAString[400];
			uiString::copy<WCHAR,T>(lpAString, lpString);
			
			if (swscanf(lpAString,L"%d", result) == -1)
				return false;

			return true;
		}

		// parseToInt
		// Chuyen string thanh float
		template<class T>
		static bool parseToFloat(T* lpString, float *result)
		{
			WCHAR lpAString[400];
			uiString::copy<WCHAR,T>(lpAString, lpString);
			
			if (swscanf(lpAString,L"%f", result) == -1)
				return false;

			return true;
		}

		// parseToDWORD
		// Chuyen string thanh float
		template<class T>
		static bool parseToDWORD(T* lpString, DWORD *result)
		{
			WCHAR lpAString[400];
			uiString::copy<WCHAR,T>(lpAString, lpString);
			
			if (swscanf(lpAString,L"%u", result) == -1)
				return false;

			return true;
		}

		// parseFromHex
		// Chuyen hex thanh int
		template<class T>
		static bool parseFromHex(T* lpString, int *result)
		{
			WCHAR lpAString[400];
			uiString::copy<WCHAR,T>(lpAString, lpString);
			
			if (swscanf(lpAString,L"%X", result) == -1)
				return false;				

			return true;
		}

		// toLower
		// Chuyen ve chu thuong
		template<class T>
		static void toLower(T *lpString)
		{
			DWORD i = 0;
			while (lpString[i] != NULL)
			{	
				lpString[i] = (int)towlower( lpString[i] );
				i++;
			}
		}

		// toLower
		// Chuyen ve chu thuong
		template<class T>
		static void toUpper(T *lpString)
		{
			DWORD i = 0;
			while (lpString[i] != NULL)
			{	
				lpString[i] = (int)towupper( lpString[i] );
				i++;
			}
		}
	};
//}

#endif
