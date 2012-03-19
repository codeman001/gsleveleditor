// uiFont.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiString.h"
#include "uiInclude.h"

using namespace uiClass;

uiFont::uiFont( int nHeight, LPWSTR lpFontName )
{
	this->createFont( nHeight, lpFontName );
}

uiFont::~uiFont()
{
	this->deleteObject();
}

// createFont
// Khoi tao font
// Tham so:	- size
//			- ten font
void uiFont::createFont(int nHeight, LPWSTR lpFontName )
{
	zm_lf.lfHeight         = nHeight;
    zm_lf.lfWidth          = 0;
    zm_lf.lfEscapement     = 0;
    zm_lf.lfOrientation    = 0;
    zm_lf.lfWeight         = FW_NORMAL;
    zm_lf.lfItalic         = FALSE;
    zm_lf.lfUnderline      = FALSE;
    zm_lf.lfStrikeOut      = FALSE;
    zm_lf.lfCharSet        = ANSI_CHARSET;
    zm_lf.lfOutPrecision   = OUT_TT_PRECIS;
    zm_lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    zm_lf.lfQuality        = DEFAULT_QUALITY;
    zm_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	
	uiString::copy<WCHAR,WCHAR>( zm_lf.lfFaceName, lpFontName );	
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);
}
			
// getObjectType
// Tra ve doi tuong UIOBJECT
DWORD uiFont::getObject()
{
	return UIOBJECT_FONT;
}
			
// setFontNormal
// Thiet lap font binh thuong
void uiFont::setFontNormal()
{
	this->deleteObject();
	  
	zm_lf.lfWeight         = FW_NORMAL;
    zm_lf.lfItalic         = FALSE;
    zm_lf.lfUnderline      = FALSE;
    zm_lf.lfStrikeOut      = FALSE;

	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);	
}

// setFontBold
// Thiet lap font dam
void uiFont::setFontBold()
{
	this->deleteObject();

	zm_lf.lfWeight = FW_BOLD;	
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);	
}

// setFontStrong
// Thiet lap font dam
void uiFont::setFontStrong()
{
	this->deleteObject();

	zm_lf.lfWeight = FW_HEAVY;
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);
}

// setFontUnderline
// Thiet lap font gach duoi
void uiFont::setFontUnderline(bool bSet)
{
	this->deleteObject();

	zm_lf.lfUnderline = bSet;
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);	
}
			
// setFontIlatic
// Thiet lap font nghieng
void uiFont::setFontIlatic(bool bSet)
{
	this->deleteObject();

	zm_lf.lfItalic = bSet;
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);	
}

// setRotation
// Xoay text
void uiFont::setRotation( int nDeg )
{
	this->deleteObject();
	
	zm_lf.lfEscapement  = nDeg;
	zm_lf.lfOrientation = nDeg;	
	
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);
}
			
// setQualityAntialiased
// Khu rang cua text khi draw
void uiFont::setQualityAntialiased()
{
	this->deleteObject();
	
	zm_lf.lfQuality = ANTIALIASED_QUALITY;	
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);
}
			
// setQualityNormal
// Draw o che do binh thuong
void uiFont::setQualityNormal()
{
	this->deleteObject();

	zm_lf.lfQuality = DEFAULT_QUALITY;
	zm_hObj = (HFONT) CreateFontIndirectW(&zm_lf);
}