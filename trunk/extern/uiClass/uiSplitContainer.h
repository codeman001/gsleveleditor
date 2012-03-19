// uiSplitContainer.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 20/12/2008
// Thong tin:
//

#ifndef _UISLITCONTAINER_
#define _UISLITCONTAINER_

#include "uiInclude.h"
#include "uiWindow.h"

using std::vector;

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{
	class uiSplitContainer;

	typedef vector<uiWindow*>		uiArrayWindows;
	typedef vector<uiArrayWindows>	uiMatrixWindows;	

	// uiSplitContainer
	// Doi tuong control Split
	class DLLEXPORT uiSplitContainer: public uiWindow
	{
	protected:
		// Danh sach cac cua so con (2 chieu)
		uiMatrixWindows				zm_listChild;		
		
		// Kich thuoc cua hang va cot
		vector<DWORD>		zm_rowSize;
		vector<DWORD>		zm_colSize;

		// So hang va so cot
		DWORD zm_row;
		DWORD zm_col;
		
		DWORD zm_nExpanderSize;
		
		// True neu chuot nhan xuong
		bool zm_isMouseDown;
		bool zm_isDrag;

		DWORD zm_nMinSize;
		
		DWORD zm_nCurrentRow;
		DWORD zm_nCurrentCol;

		// Toa do chuot
		int zm_oldX;
		int zm_oldY;
				
		bool zm_bErase;
		int zm_nHit;
		RECT zm_clientExpander;

		uiSplitContainer();
	public:
		// uiSplitContainer
		// Khoi tao uiSplit
		//		- Tham so: numRow (so hang)
		//		- Tham so: numCol (so cot)
		uiSplitContainer(LPWSTR lpTitle,int x, int y, int w, int h, uiWindow* pParent, DWORD numRow, DWORD numCol);
		virtual ~uiSplitContainer(void);
	protected:
	
		// getCellRect
		// Lay kich thuoc cua o
		bool getCellRect(int row, int col, int countR, int countC, RECT *r);

		// setCursorIcon
		// Thiet lap icon cho mouse
		void setCursorIcon( int r );

		// drawPatternInvert
		// Ve vien mo khi di chuyen chuot
		void drawPatternInvert(int x, int y, int w, int h);

		// getHitest
		// Kiem tra chuot dang o tren expander nao
		// Tham so:
		//		x,y: Toa do chuot
		//		r,c: Ket qua tra ve hang, cot
		// Return:
		//		-1: Loi
		//		0: expander ngang
		//		1: expander doc
		//		2: expander trung tam
		int	 getHittest(int x, int y, DWORD *r, DWORD *c);

		// eraseBg
		// Xoa dau vet da draw luc truoc
		void eraseBg(int hit);

		// getLimitRect
		// Lay vi tri point bi gioi han khi keo
		void getLimitRect( int *x, int *y );
	public:		

		// getObject
		// Tra ve doi tuong UIOBJECT_CTLSPLIT
		virtual DWORD getObject();

		// addWindow
		// Them 1 cua so vao splitContainer
		void setWindow( uiWindow* p, DWORD row, DWORD col );
		
		// getWindow
		// Lay cua so
		uiWindow* getWindow( DWORD row, DWORD col );

		// removeWindow
		// Loai bo cua so
		void removeWindow( DWORD row, DWORD col );

		// getRows
		// Tra ve so hang
		DWORD getRows();
		
		// Tra ve so cot
		DWORD getCols();

		// setColSize
		// Thiet lap size cho col
		void setColSize(DWORD col, DWORD size);
		
		// setRowSize
		// Thiet lap size cho hang
		void setRowSize(DWORD row, DWORD size);

		// getColSize
		// Lay kich thuoc cua cot
		DWORD getColSize(DWORD col);

		// getRowSize
		// Lay kich thuoc cua hang
		DWORD getRowSize(DWORD row);

		// setExpanderSize
		// Thiet lap kich thuoc cho Expander
		void setExpanderSize(DWORD size);

		// getExpanderSize
		// Lay kich thuoc Expander
		DWORD getExpanderSize();
		
		// setMinSize
		// Thiet lap kich thuoc nho nhat cua cua so
		void setMinSize(DWORD size);

		// getMinSize
		// Lay kich thuoc nho nhat cua cua so
		DWORD getMinSize();

		// updateSplit
		// Cap nhat kich thuoc 
		void updateSplit();

		virtual void _OnPaint( uiGraphics* pG );

		virtual void _OnSize( uiSizeEvent e, int w, int h );

		virtual void _OnMouseMove(uiMouseEvent e, int x, int y);

		virtual void _OnLButtonUp(uiMouseEvent e, int x, int y);

		virtual void _OnLButtonDown(uiMouseEvent e, int x, int y);
	};

}
#endif
