// uiWindow.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//

#ifndef _UIWINDOW_
#define _UIWINDOW_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiObject.h"
#include "uiMenu.h"
#include "uiKeyAccel.h"

using std::vector;

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{
	// Class prototype ///////////////////////////////
	class uiWindow;

	// Graphics Class
	class uiColor;
	class uiGraphicsObject;
	class uiPen;
	class uiBrush;
	class uiGraphics;
	class uiBitmap;
	//////////////////////////////////////
	class uiForm;
	class uiDialog;
	class uiDock;
	class uiLockLayout;
	class uiControl;
	class uiTabController;
	// ///////////////////////////////////	
	class uiIcon;
	class uiCursor;
	// ///////////////////////////////////

#pragma region "uiGraphics Class"
	// uiGraphics
	// Doi tuong Graphics ho tro draw
	class DLLEXPORT uiGraphics:public uiObject
	{
		protected:
			HBITMAP zm_hBitmap;
			HDC		zm_hDC;
			HWND	zm_hWnd;
			bool	zm_bIsBackBuffer;
			bool	zm_releaseHDC;
		public:
			uiGraphics();

			// uiGraphics
			// Khoi tao doi tuong graphics
			// Tham so:		- Cua so can ve
			//				- pWin = NULL ve len man hinh
			uiGraphics(uiWindow* pWin);

			uiGraphics(HDC hDC);
			virtual ~uiGraphics(void);	
			
			// getObject
			// Tra ve ID doi tuong
			DWORD getObject();

			// getHandle
			// Lay gia tri HDC
			HDC getHandle();
			
			// getHwnd
			// Lay handle HWND
			HWND getHwnd();

			// getBitmap
			// Lay handle Bitmap
			HBITMAP getBitmap();

			// selectObject
			// Thiet lap cac doi tuong graphics
			HGDIOBJ selectObject(uiGraphicsObject *pObj);
			
			// selectObject
			// Thiet lap doi tuong graphics
			HGDIOBJ selectObject(HGDIOBJ obj);

			// createBackBuffer
			// Khoi tao vung dem
			uiGraphics* createBackBuffer(int w, int h);
			
			// createBackBuffer
			// Khoi tao vung dem theo 1 bitmap
			uiGraphics* createBackBuffer(uiBitmap *pBitmap);


			// swapBuffer
			// Hoan doi vung dem
			void swapBuffer(int xDest, int yDest, int wDest, int hDest, 
				uiGraphics *pBuffer, int xSrc, int ySrc, int wSrc, int hSrc, 
				DWORD dwOption);
			
			// swapImage
			// Ve Images len cua so
			void swapImage(int xDest, int yDest, int wDest, int hDest,
				uiBitmap *pImg, int xSrc,int ySrc, int wSrc, int hSrc, bool bRepeatX, bool bRepeatY, DWORD option = SRCCOPY);

			// releaseGraphics
			// Huy bo vung dem
			void releaseGraphics();
			
			// drawPixel
			// Ve 1 pixel mau tai diem x,y
			void drawPixel(int x, int y, uiColor color);

			// drawLine
			// Ve 1 line
			void drawLine(int x1, int y1, int x2, int y2);
			
			// moveTo
			// Ve point dau tien cua line
			void moveTo(int x, int y);

			// lineTo
			// Ve point tiep theo cua line
			void lineTo(int x, int y);
			
			// drawPolyline
			// Ve nhieu duong thang
			void drawPolyline(uiPoint *a, int nPoint);

			// drawPolylineLoop
			// Ve polygon line (khong fill)
			void drawPolylineLoop(uiPoint *a, int nPoint);
			
			// drawBezier
			// Ve duong con Bezier
			void drawBezier(uiPoint *a, int nPoint);			
			
			// drawArc
			// Ve duong cung
			void drawArc(int nLeft, int nTop, int nRight, int nBottom, 
				int nXStart, int nYStart, int nXEnd, int nYEnd );			

			// drawArcAngle
			// Ve 1 duong cung theo goc
			void drawArcAngle(int x, int y, DWORD dwRadius,float fStartAngle, float fSweepAngle);
			
			// drawPolygon
			// Ve 1 polygon fill
			void drawPolygon( uiPoint *a, int nPoint);
			
			// drawEllipse
			// Ve 1 hinh ellipse
			void drawEllipse(int x1, int y1, int x2, int y2);			

			// drawChord
			// Ve 1 cung ellip
			void drawChord(int x1, int y1, int x2, int y2, int xR1, int yR1, int xR2, int yR2 );

			// drawChord
			// Ve 1 cung pie
			void drawPie(int x1, int y1, int x2, int y2, int xR1, int yR1, int xR2, int yR2 );
			
			// drawPatternInvert
			// Ve 1 rectangle co mau sac invert voi mau hien tai
			void drawPatternInvert(int x, int y, int w, int h);
			
			// drawRectangle
			// Ve 1 hinh chu nhat (ko fill)
			void drawRectangle(int x1, int y1, int x2, int y2);		

			// drawFillRectangle
			// Ve 1 hinh chu nhat
			void drawFillRectangle(int x1, int y1, int x2, int y2, uiBrush* pBrush);
			
			// drawRoundRectangle
			// Ve 1 hinh chu nhat (bo goc)
			void drawRoundRectangle(int x1, int y1, int x2, int y2, int R1, int R2);
			
			// drawText
			// Ve 1 doan text ra man hinh
			void drawText(int x, int y, LPWSTR lpText);
			
			// drawTextInRect
			// Ve 1 doan text trong 1 khung hinh chu nhat
			// Tham so:		- nFormat (const DT_  cua GDI)
			void drawTextInRect(int x, int y, int w, int h, LPWSTR lpText, UINT nFormat=0);
			
			// setTextColor
			// Thiet la mau sac cho text
			void setTextColor(uiColor c);
			
			// setTextBkColor
			// Thiet lap mau nen
			void setTextBkColor(uiColor c);

			// setTextBkTransparent
			// Thiet lap mau nen trong suot
			void setTextBkTransparent(bool bSet = true);

			// setTextAlign 
			// Canh text (su dung cho drawTextInRect) nTAlign la const TA_ cua GDI
			void setTextAlign( UINT nTAlign );			
			
			// getCharWidth 
			// Lay chieu rong cua 1 ky tu
			DWORD getCharWidth(WCHAR wChar);

			// getStringWidth
			// Lay chieu rong cua 1 chuoi ky tu
			DWORD getStringWidth(WCHAR *lpString);

			// getTextColor
			// Lay mau sac cua text
			uiColor getTextColor();

			// getTextBkColor
			// Lay mau nen cua text
			uiColor getTextBkColor();

			// releaseHDCWhenDestructor
			// Cho phep ko huy HDC
			void releaseHDCWhenDestructor( bool b );
	};
#pragma endregion

#pragma region "uiGraphicsObject Class"
	// uiGraphicsObject
	// Doi tuong truu truong trong graphics
	class DLLEXPORT uiGraphicsObject: public uiObject
	{
		protected:		
			HGDIOBJ zm_hObj;
		public: 
			uiGraphicsObject();
			
			// getSafeObject
			// Lay handle HGDIOBJ
			HGDIOBJ getSafeObject();

			// deleteObject
			// Xoa bo object
			void deleteObject();	
	};
#pragma endregion

#pragma region "uiPen Class"
	// uiPen
	// Doi tuong ho tro thiet lap kieu ve
	class DLLEXPORT uiPen: public uiGraphicsObject
	{		
		protected:
			LOGPEN zm_lPen;
		public:		
			uiPen();			
			~uiPen();
			
			// uiPen
			// Constructor khoi tao but ve
			// Tham so:		- Chieu rong
			//				- Kieu ve (const GDI PS_)
			//				- Mau sac
			uiPen(int nWidth, int nStyle, uiColor color);
			
			// getObject
			// Tra ve ID doi tuong (const UIOBJECT_ )
			virtual DWORD getObject();

			// create
			// Khoi tao but ve
			// Tham so:		- Chieu rong
			//				- Kieu ve (const GDI PS_)
			//				- Mau sac
			void create(int nWidth, int nStyle, uiColor color);
			
			// create
			// Khoi tao but ve tu brush
			// Tham so:		- Chieu rong
			//				- Kieu ve (const GDI PS_)
			//				- Mau sac
			void createBrushPen(int nWidth, int nStyle, uiBrush* pBrush);
			
			// createUserPen
			// Khoi tao but ve tu do
			// Tham so:		- Chieu rong
			//				- Brush the hien mau sac
			//				- So phan tu cua mang
			//				- Kieu mang
			// Thong tin: xem them ham ExtCreatePen cua WINAPI
			void createUserPen(int nWidth, uiBrush* pBrush,int dwStyleCount, DWORD *aStyle);
	};
#pragma endregion

#pragma region "uiBrush Class"
	// uiBrush
	// Doi tuong ho tro mau to
	class DLLEXPORT uiBrush: public uiGraphicsObject{
		protected:
			LOGBRUSH	zm_lBrush;
		public:
			uiBrush ();
			uiBrush	(uiColor color );
			~uiBrush();
			
			// getObject
			// Tra ve ID doi tuong (const UIOBJECT_ )
			virtual DWORD getObject();
			
			// getLogBrush
			// Tra ve handle Brush
			LOGBRUSH getLogBrush();
			
			// createPatternBrush
			// Khoi tao brush dua tren 1 bitmap
			// void createPatternBrush( ZBitmap *pBitmap );

			// createSolidBrush
			// Khoi tao brush theo mau sac
			void createSolidBrush( uiColor color );

			// createHatchBrush
			// Khoi tao mau to theo mau nHatch la const (HS_ cua GDI)
			void createHatchBrush( uiColor color, int nHatch );			

			// createNullBrush
			// Khoi tao 1 brush khong mau
			void createNullBrush();

			// createUserMonoPatternBrush 
			// Khoi tao brush theo 1 bitmap trang den
			void createUserMonoPatternBrush ( int w, int h, BYTE* pData );

			// createPatternBrush
			// Khoi tao brush theo 1 hinh BITMAP
			void createPatternBrush( uiBitmap *pBitmap );
	};
#pragma endregion

#pragma region "uiColor Class"
	// uiColor
	// Doi tuong mau sac
	class DLLEXPORT uiColor:public uiObject{
		protected:
			DWORD zm_dwColor;
		public:
			uiColor();
			uiColor(DWORD color);
			uiColor(UINT R, UINT G, UINT B, UINT A=0);
			
			// getObject
			// Tra ve ID doi tuong (const UIOBJECT_ )
			virtual DWORD getObject();

			// getColor
			// Lay mau sac hien tai
			DWORD getColor();

			// setRGBA
			// Thiet lap mau sac
			void  setRGBA(BYTE nR, BYTE nG, BYTE nB, BYTE nA = 0);
			
			// fromRGBA
			// Thiet lap mau sac RGBA
			static uiColor fromRGBA		(UINT nR, UINT nG, UINT nB, UINT nA = 0);

			// fromSystem
			// Thiet lap mau sac he thong (index la const cua WINAPI COLOR_ )
			static uiColor fromSystem	(UINT nIndex);

			// fromDWORD
			// Thiet lap mau sac tu 1 gia tri dword
			static uiColor fromDWORD	(DWORD color );
			
			// getR
			// Lay sac do
			BYTE getR();

			// getG
			// Lay sac xanh la
			BYTE getG();
			
			// getB
			// Lay sac xanh
			BYTE getB();

			// getA
			// Lay do trong suot
			BYTE getA();
			
			// Lay mau sac hien tai
			void getRGBA(BYTE *R, BYTE* G, BYTE* B, BYTE* A);

			uiColor &operator= (const uiColor& c);
	};
#pragma endregion

#pragma region "uiFont Class"
	// uiFont
	// Doi tuong Font
	class DLLEXPORT uiFont:public uiGraphicsObject{
			LOGFONTW zm_lf;			
		public:			
			uiFont( int nHeight, LPWSTR lpFontName );

			virtual ~uiFont();
			
			// createFont
			// Khoi tao font
			// Tham so:	- size
			//			- ten font
			void createFont(int nHeight, LPWSTR lpFontName );
			
			// getObject
			// Tra ve ID doi tuong (const UIOBJECT_ )
			virtual DWORD getObject();
			
			// setFontNormal
			// Thiet lap font binh thuong
			void setFontNormal();

			// setFontBold
			// Thiet lap font dam
			void setFontBold();

			// setFontStrong
			// Thiet lap font dam
			void setFontStrong();

			// setFontUnderline
			// Thiet lap font gach duoi
			void setFontUnderline(bool bSet=true);
			
			// setFontIlatic
			// Thiet lap font nghieng
			void setFontIlatic(bool bSet=true);

			// setRotation
			// Xoay text
			void setRotation( int nDeg );
			
			// setQualityAntialiased
			// Khu rang cua text khi draw
			void setQualityAntialiased();
			
			// setQualityNormal
			// Draw o che do binh thuong
			void setQualityNormal();
	
	};	
#pragma endregion

#pragma region "uiBitmap class"
	class DLLEXPORT uiBitmap: public uiGraphicsObject
	{
	protected:
		
	public:
		uiBitmap(HBITMAP hBitmap);

		uiBitmap(uiGraphics *pG);

		uiBitmap(LPWSTR fromResouce, bool bResourse);
		
		uiBitmap(DWORD w, DWORD h, BYTE nPlanes, BYTE nBitCount, void* lpBits);

		virtual ~uiBitmap();

		// loadFromFile
		// Load bitmap tu file .bmp
		bool loadFromFile( LPWSTR lpFile );

		// loadFromResource
		// Load bitmap from ID resource
		bool loadFromResource( LPWSTR res );

		// getObject
		// Tra ve doi tuong UIOBJECT_BITMAP
		virtual DWORD getObject();

		// getWidth
		// Lay chieu rong cua Bitmap
		int getWidth();
		
		// getHeiht
		// Lay chieu cao cua Bitmap
		int getHeight();

		// getBitPerPixel
		// Lay so bit/Pixel
		BYTE getBitPerPixel();

		// getRowBit
		// Lay du lieu mau sac o line thu n
		void getRowBit(BYTE* pData, UINT n);

	};
#pragma endregion

#pragma region "uiWindow Class"
	// uiWindow
	// Doi tuong co so cho cac cua so cua uiClass
	class DLLEXPORT uiWindow: public uiObject
	{
		protected:
			// Handle cua cua so
			HWND		zm_hWnd;
			HINSTANCE	zm_hInstance;
			
			// Cua so cha (create)
			uiWindow *zm_pParent;

			// Cua so cha (include - di chuyen ben trong cua so nay)
			uiWindow *zm_pParentInclude;
			
			// Trang thai dock cua cua so
			BYTE		zm_byteDock;
			uiDock			*zm_pDockCtl;
			uiLockLayout	*zm_pLockLayoutCtl;

			// Danh sach cua so con
			vector<uiWindow*>	zm_aChild;
			
			// Style cua cua so
			DWORD		zm_nStyle;
			
			// Margin: top - bottom - left - right
			uiMargin	zm_margin;
			uiMargin	zm_lockLayout;
			DWORD		zm_dwLockLayout;

			// Tabindex
			int			zm_nTabIndex;
			uiTabController *zm_tabController;			

			bool		zm_bInit;
			bool		zm_bReDraw;
			uiColor		zm_drawColor;
		// Cac function khoi tao
		protected:
			// registerWindow
			// Dang ky cua so
			virtual bool registerWindow(LPWSTR lpName, HINSTANCE hInst);	
			
			// windowProc
			// Xu ly cac su kien cho chuong trinh
			static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			
			// messageMap
			// Phan loai message
			virtual LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
			
			uiWindow();

			// constructor
			// Mot so khoi tao mac dinh ban dau cho uiWindow
			void constructor();					

		public:
			uiWindow( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
			virtual ~uiWindow(void);			

			// Create
			// Khoi tao cua so
			virtual bool create( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );			

			// getObject
			// Tra ve const UIOBJECT_WINDOW
			virtual DWORD getObject();
							
#pragma region "uiWindow event"
		public:
			static list<uiWindow*>*	sm_globalEventWindow;
		public:
			static void initAppListen();
			static void releaseAppListen();
			static void listenThisAppEvent( uiWindow *p );
			static void unListenThisAppEvent( uiWindow *p );

			// messageMap
			// Phan loai message
			// return:
			//		- false:	disable msg to another window and use default msg
			//		- true:		normal
			virtual bool globalMessageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				return true;
			}
#pragma endregion

#pragma region "uiWindow event"
		// Su kien cho cua so
		protected:
			virtual void _OnMouseMove		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnMouseLeave		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnMouseHover		(	uiMouseEvent mouseEvent,int x, int y	);
			
			virtual void _OnLButtonDown		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnLButtonUp		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnLButtonDbClick	(	uiMouseEvent mouseEvent,int x, int y	);

			virtual void _OnMButtonDown		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnMButtonUp		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnMButtonDbClick	(	uiMouseEvent mouseEvent,int x, int y	);

			virtual void _OnRButtonDown		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnRButtonUp		(	uiMouseEvent mouseEvent,int x, int y	);
			virtual void _OnRButtonDbClick	(	uiMouseEvent mouseEvent,int x, int y	);
			
			virtual void _OnMouseWheel		(	uiMouseEvent mouseEvent,int x, int y	);
			

			virtual bool _OnKeyDown			(	uiKeyEvent	keyEvent	);
			virtual bool _OnKeyUp			(	uiKeyEvent	keyEvent	);
			virtual bool _OnChar			(	uiCharEvent charEvent	);

						
			virtual void _OnCreate();
			virtual void _OnInit();
			
			virtual void _OnPaint		(	uiGraphics * pG );

			virtual void _OnSize		(	uiSizeEvent sizeEvent, int nWidth, int nHeight);
			virtual void _OnMove		(	int x, int y );
			virtual bool _OnClose		();
			virtual void _OnDestroy		();			
			virtual void _OnSetFocus	();
			virtual void _OnLostFocus	();
		
			virtual void _OnCommand		( DWORD dwCommand, DWORD lParam );
			virtual void _OnNotify		( DWORD dwNotify, DWORD lParam );

			virtual void _OnDropFile	( uiFileDrop* fileDrop );
			
			virtual void _OnHScroll		( uiScrollBarEvent scrollEvent );
			virtual void _OnVScroll		( uiScrollBarEvent scrollEvent );

			virtual void _onMeasureItem	( MEASUREITEMSTRUCT* lpItemStruct );
			virtual void _onDrawItem	( DRAWITEMSTRUCT* lpDrawItemStruct );
#pragma endregion

		public:

#pragma region "uiWindow Function"
			// gethHandle
			// Lay gia tri HWND cua cua so
			HWND		getHandle();

			// gethInstance
			// Lay gia tri HINSTANCE
			HINSTANCE	getInstance();

			// gethParent
			// Lay doi tuong cua so cha
			uiWindow*	getParent();				

			// changeStyle
			// Thay doi kieu dang cua cua so
			// Tham so:	- Style them vao <const cua WINAPI (WS_ )>
			//			- Style loai bo <const cua WINAPI (WS_ )>
			void changeStyle	(LONG lStyleAdd,LONG lStyleRemove);

			// changeStyleEx
			// Thay doi kieu dang mot so ex mo rong cua cua so
			// Tham so:	- Style them vao <const cua WINAPI (WS_EX )>
			//			- Style loai bo <const cua WINAPI (WS_EX )>
			void changeStyleEx	(LONG lStyleAdd,LONG lStyleRemove);					
			
			// setDock
			// Dock vao cua so cha
			// Tham so:	- Dia chi cua so cha
			//			- Const Dock (UIDOCK_ )
			void setDock(uiWindow* pParentWin, BYTE nDock);
			
			// getDock
			// Lay const UIDOCK
			BYTE getDock();
			
			// unDock
			// Thoat khoi dock cua so cha
			void unDock();

			// setParent
			// Thay doi cua so cha
			void setParent(uiWindow *pWin);		
			
			// addChild
			// Them mot cua so con
			void addChild(uiWindow *pChild);

			// removeChild
			// Loai bo cua so con
			void removeChild(uiWindow* pChild);

			// invalidateRect
			// Cap nhat cua so 			
			void invalidateRect	(RECT *r, bool bRePaint);
			
			// validateRect
			// Cap nhat cua so 
			void validateRect	(RECT *r);		
			
			// setForegroundWindow
			// Active cua so
			void setForegroundWindow();

			// enableWindow
			// Bat chuc nang cua so
			void enableWindow(bool b);

			// isEnable
			// Kiem tra cua so co enable hay khong
			bool isEnable();

			// isShow
			// Kiem tra cua so Hiden hay Show
			bool isShow();

			// showWindow
			// Hien/An cua so
			void showWindow(bool b);			
			
			// updateWindow
			// Cap nhat cua so
			void updateWindow();		
			
			// setWindowState
			// Thay doi trang thai cua so theo hang so SW_ (WINAPI)
			void setWindowState(UINT nState);
			
			// moveWindow
			// Di chuyen cua so
			void moveWindow(int x, int y, int w, int h);

			// setLeftTop
			// Thiet lap vi tri cua so (so voi cua so cha)
			void setLeftTop(int x, int y);

			// getLeftTop
			// Lay vi tri cua cua so (so voi cua so cha)
			void getLeftTop(int* x,int* y);
			
			// getTop
			// Lay toa do Y cua cua so (so voi cua so cha)
			int getTop();

			// getLeft
			// Lay toa do X cua cua so (so voi cua so cha)
			int getLeft();

			// setWidthHeight
			// Thiet lap chieu rong va chieu cua cua so
			void setWidthHeight(int nWidth, int nHeight);

			// getWidthHeight
			// Lay chieu rong va cao cua cua so
			void getWidthHeight(int* nWidth, int* nHeight);

			// getWidth
			// Lay chieu rong
			int getWidth();

			// getHeight
			// Lay chieu cao
			int getHeight();
			
			// getPointToScreen
			// Chuyen doi 1 point tu toa do cua so -> toa do man hinh
			void getPointToScreen(int *x, int *y);

			// getPointToScreen
			// Chuyen doi 1 point tu toa do man hinh -> toa do cua so
			void getPointFromScreen(int *x, int *y);

			// setCapture
			// Hook chuot khi no di chuyen ra ngoai cua so
			void setCapture();

			// releaseCapture
			// unHook do ham setCapture()
			void releaseCapture();			
			
			// getClientWH
			// Lay chieu rong + cao cua vung client
			void getClientWH(int* nWidth, int* nHeight);

			// setClientWH
			// Thiet lap chieu rong + cao
			void setClientWH(int w, int h);

			// getClientWidth
			// Lay chieu rong cua client
			int  getClientWidth();

			// getClientHeight
			// Lay chieu cao cua client
			int  getClientHeight();
			
			// setCaption
			// Thiet lap tieu de
			void setCaption(LPWSTR lpCaption);

			// getCaption
			// Lay tieu de cua cua so
			void getCaption(LPWSTR lpCaption);					
			
			// setMargin
			// Thiet lap le cho cua so
			void setMargin(int l, int r, int t, int b);

			// getMargin
			// Lay gia tri le da thiet lap
			void getMargin(int *l, int *r, int *t, int *b);
			
			// showScrollBarVer
			// Hien thi scrollBar ngang
			void showScrollBarVer(bool b);

			// showScrollBarHorz
			// Hien thi scrollBar doc
			void showScrollBarHorz(bool b);

			// setScrollRange
			// Thiet lap mien gia tri cho scroll bar
			// scroll = SB_HORZ
			// scroll = SB_VERT
			void setScrollRange( int scroll, int min, int max );

			// getScrollRange
			// Lay gia tri scroll
			// scroll = SB_HORZ
			// scroll = SB_VERT
			void getScrollRange( int scroll, int *min, int *max );

			// setScrollPos
			// Thiet lap vi tri pos
			void setScrollPos( int scroll, int pos );

			// getScrollPos
			// Lay gia tri pos
			void getScrollPos( int scroll, int *pos );

			// enableScrollBar
			// Bat tat su dung scroll
			// scroll = SB_BOTH
			// scroll = SB_HORZ
			// scroll = SB_VERT
			void enableScrollBar( int scroll, bool enable );

			// showScrollBar
			// Hien thi scrollBar
			void showScrollBar(bool b);

			// showMaximizeButton
			// Hien thi nut Maximum
			void showMaximizeButton(bool b);
			
			// showMinimumButton
			// Hien thi nut Minimum
			void showMinimumButton(bool b);
			
			// showCaption
			// Hien thi thanh caption
			void showCaption(bool b);
			
			// showBorder
			// Hien thi border
			void showBorder(bool b);
			
			// setOpacity
			// Thiet lap do trong suot
			void setOpacity(BYTE nOpacity);

			// setFocus
			// Thiet lap active cua so nay
			void setFocus();
			
			// changeWindowStyle
			// Thay doi kieu cua cua so (nStyle la hang so UISTYLE_)
			void changeWindowStyle(int nStyle);
			
			// closeWindow
			// Dong cua so
			void closeWindow();
			
			// rePaint
			// Ve lai cua so
			void rePaint();

			// doModal
			// Hien thi 1 Dialog
			void doModal(uiDialog *pDialog);

			// bringTop
			// Dem windows ra dang truoc tat cac cac cua so
			void bringTop();

			// setMenu
			// Thiet lap menu cho cua so
			void setMenu( uiMenu *pMenu );

			// setTabIndex
			// Thiet lap vi tri tab 
			//		- i < 0: Control khong forus khi nhan Tab
			void setTabIndex( int i );
			
			// getTabIndex
			// Lay gia tri TabIndex
			int getTabIndex();
			
			// setTabController
			// Thiet lap doi tuong tabController
			void setTabController( uiTabController* pTabController );

			// getTabController
			// Lay doi tuong TabController
			uiTabController* getTabController();

			// setIcon
			// Thiet lap icon cho cua so
			void setIcon(uiIcon *pIcon, bool bLargeIcon);

			// setCursor
			// Thiet lap cursor cho cua so
			void setCursor( uiCursor *pCursor);
	
			// setFont
			// Thiet lap font chu cho cua so
			void setFont( uiFont *font );

			// alert
			// Hien thi 1 thong bao msgBox
			void alert( LPWSTR strMessage, LPWSTR caption, DWORD dwFlags = 0 );

			// question
			// Hien thi 1 thong bao yes, no
			bool question( LPWSTR lpString, LPWSTR caption, DWORD dwFlags = 0 );

			// updateDock
			// Cap nhat lai kich thuoc cac cua so da Dock
			void updateDock();
			
			// setLockLayout
			// Thiet lap lockLayout
			void setLockLayout( uiWindow* pWin, DWORD dwLock, uiMargin m );
		
			void setLockLayout( uiWindow* pWin, DWORD dwLock, int l, int t, int r, int b );

			// unLockLayout
			// Huy bo layout da lock
			void unLockLayout();

			// getLockLayout
			// Lay trang thai lockLayout
			DWORD getLockLayout( uiMargin *m );

			// updateLockLayout
			// Cap nhat LockLayout
			void updateLockLayout();

			// enableReDraw
			// Cho phep ve lai hay ko 
			void enableReDraw( bool b );

			// setReDrawColor
			// Thiet lap mau sau cho nen windows
			void setReDrawColor( uiColor b );

#pragma endregion

	};


#pragma endregion

#pragma region "uiForm Class"
	class DLLEXPORT uiForm : public uiWindow
	{
		protected:
			bool zm_bAllowIdle;	
			uiKeyAccel *zm_pKeyAccel;
		protected:
			// registerWindow
			// Dang ky cua so form
			virtual bool registerWindow(LPWSTR lpNameApp, HINSTANCE hInst);
			
			// messageMap
			// Phan tich su kien cho uiForm
			virtual LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);			

			uiForm();			
		public:
			
			// getObject
			// Tra ve const UIOBJECT_FORM
			virtual DWORD getObject();

			// uiForm
			// Constructor khoi tao form
			uiForm(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, LPWSTR lpName);
			virtual ~uiForm(void);
			
			// create
			// Khoi tao cua so
			virtual int create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent, LPWSTR lpName);			
			
			// Idle 
			// Cho phep chay su kien idle hay ko
			void enableOnIdle(bool b);

			// isEnableOnIdle	
			bool isEnableOnIdle();

			// setPositionCenterOfScreen
			// Cho phep hien thi ngay chinh giua man hinh
			void setPositionCenterOfScreen();
		public:
			// _OnIdle
			// Su kien idle luon luon chay khi enableOnIdle.
			virtual void _OnIdle();

			// getKeyAccelTable
			// Lay bang hotKey
			uiKeyAccel* getKeyAccelTable();

			// createAccelTable
			// Tao bang Accel
			void createAccelTable( uiKeyAccelEntry *p );
	};
#pragma endregion

#pragma region "uiDock Class"
	// uiDock
	// Doi tuong ho tro dock cua so
	class DLLEXPORT uiDock:public uiObject
	{
		protected:
			uiWindow			*zm_pMyWin;
			vector<uiWindow*>	zm_aDocked;
			vector<uiWindow*>	zm_aDock;
		protected:
			// getClientRect
			// Lay vung cua so trong con lai de dock
			void getClientRect(int *x, int *y, int *r, int *b);			
		public:
			// getObject
			DWORD getObject();

			uiDock(uiWindow* pWin);
			virtual ~uiDock();
			
			// dock
			// dock cua so pWin vao parent
			void dock(uiWindow* pWin);		
			
			// unDock
			// unDock cua so pWin
			void unDock(uiWindow* pWin);

			// update
			// cap nhat vi tri cac cua so
			void update();			
	};
#pragma endregion

	// uiLockLayout
	// Xu ly lock cua so
	class DLLEXPORT uiLockLayout: public uiObject
	{
	protected:
		uiWindow				*zm_pMyWin;
		vector< uiWindow* >		zm_aLockLayout;
	public:
		DWORD getObject();

		uiLockLayout(uiWindow* pWin);
		virtual ~uiLockLayout();

		// lockLayout
		// Them Window vao lockLayout
		void lockLayout( uiWindow *pWin );

		// unlockLayout
		// Loai bo Window khoi lockLayout
		void unlockLayout( uiWindow *pWin );

		// update
		// Cap nhat vi tri cac cua so
		void update();

	};


#pragma region "uiDialog class"
	class DLLEXPORT uiDialog: public uiForm
	{
	protected:
		uiDialog();
		
		// constructor
		// Mot so khoi tao mac dinh ban dau
		void constructor(uiWindow* pParent);		
		
		// messageMap
		// Phan tich su kien
		virtual LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
		
	public:				
		// getObject
		// Tra ve const UIOBJECT_DIALOG
		virtual DWORD getObject();

		uiDialog(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent);
		virtual ~uiDialog(void);
		
		// create
		// Khoi tao dialog
		virtual bool create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent);
	};
#pragma endregion
	
#pragma region "uiControl class"
	class DLLEXPORT uiControl: public uiWindow
	{
	protected:
		// Su kien mac dinh cua control
		LONG zm_lOldWinProc;
		UINT zm_nID;
	protected:
		uiEventCallback	eventOnFocus;
		uiEventCallback	eventOnLostFocus;
	public:
		uiControl();
		virtual ~uiControl();
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// subClass
		// Thiet lap subclass cho control de bat MSG
		void subClass();

		// windowProc
		// Quan ly su kien va nam bat control se thuc thi su kien
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// messageMap
		// Phan loai su kien
		virtual LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);	

		// messageControlMap
		// Cac message danh cho control
		virtual void messageControlMap(UINT uMsg);
		
		// notifyControl
		// Xu ly notify
		virtual void notifyControl(LPARAM lParam);

		// eventOnFocus
		// Su kien onFocus
		template<class T,void (T::*pTMethod)(uiObject*)> 
		void setEventOnFocus(T* pObject)		{ eventOnFocus.setEvent<T,pTMethod>( pObject ); }
		
		// eventOnLostFocus
		// Su kien onLostFocus
		template<class T,void (T::*pTMethod)(uiObject*)> 
		void setEventOnLostFocus(T* pObject)	{ eventOnLostFocus.setEvent<T,pTMethod>( pObject );}

		// setID
		// Thiet lap ID 
		void setID( UINT nId );

		// getID
		// Lay gia tri ID
		UINT getID();

	};
#pragma endregion

#pragma region "uiIcon class"
	class DLLEXPORT uiIcon: public uiObject
	{
	protected:	
		HICON zm_hIcon;

		uiIcon();
	public:
		// getHandle
		// Tra ve handle HICON
		HICON getHandle();
		
		// loadIcon
		// Load icon tu 1 file
		bool loadIcon(LPWSTR lpFilename, int sx, int sy);
		
		// LoadIcon
		// Load icon tu resource
		bool loadIcon(LPWSTR nResource);


		// getObject
		// Tra ve doi tuong UIOBJECT
		DWORD getObject();

		// draw
		// Ve icon
		void draw( uiGraphics* pG, int x, int y);
		void draw( uiGraphics* pG, int x, int y, int w, int h, bool bDisable = false);
	
	public:		
		virtual ~uiIcon(void);

		uiIcon(LPWSTR lpFilename, int sx, int sy);
		uiIcon(LPWSTR lpResource);		
	};
#pragma endregion
	
#pragma region "uiCursor class"
	class DLLEXPORT uiCursor:public uiObject
	{
	protected:
		HCURSOR zm_hCursor;
	public:
		// getHandle
		// Tra ve handle HCURSOR
		HCURSOR getHandle();

		// getObject
		// Tra ve UIOBJECT
		DWORD getObject();
		
		// loadFromFile
		// Load icon .cur
		bool loadFromFile( LPWSTR lpFile );

		// loadFromResource
		// Load icon tu resource
		bool loadFromResource( LPWSTR lpRes );

		uiCursor(LPWSTR from, bool bResource);
		virtual ~uiCursor();				
	};
#pragma endregion


#pragma region "uiTabController class"
	// uiTabController
	// Doi tuong ho tro tabIndex
	class DLLEXPORT uiTabController: public uiObject
	{
	protected:
		list<uiWindow*>				zm_listWindow;
		list<uiWindow*>::iterator	zm_current;
	public:
		
		// CompareWindow
		// So sanh 2 win theo thu tu tabIndex
		static bool CompareWindow(	uiWindow*& win1, uiWindow*& win2 );

		// getObject
		// Tra ve const UIOBJECT
		DWORD getObject();

		// pushWindow
		// Them mot cua so vao list
		void pushWindow( uiWindow* pWin );

		// removeWindow
		// Loai bo cua so ra khoi list
		void removeWindow( uiWindow* pWin );

		// setfocus
		// Thiet lap cua so dang active
		void setWinFocus( uiWindow* pWin );

		// updateIndex
		// Sap xep lai danh sach theo thu tu index
		void updateIndex();

		// next
		// Chuyen toi cua so phia truoc
		void next();
	};
#pragma endregion

}

#endif
