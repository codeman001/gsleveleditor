#ifndef _UILISTVIEW_H_
#define _UILISTVIEW_H_

#include "CUIWidget.h"

class CUIListviewItem;
class CUIListview;

// CUIListviewItem
// row item on list view
class CUIListviewItem: public CUIWidget
{
protected:
	CUIListview		*m_listview;
	bool			m_selected;
	bool			m_hover;

	void			*m_userdata;
public:
	CUIListviewItem( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIListviewItem();

	// update
	// update per frame
	virtual void update();

	// onETouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);	

	// setSelected
	// set selected
	void setSelected(bool b);

	// isSelected
	// if item is selected
	inline bool isSelected()
	{
		return m_selected;
	}

	// setUserData	
	inline void setUserData( void *data )
	{
		m_userdata = data;
	}

	// getUserData
	inline void* getUserData()
	{
		return m_userdata;
	}
};


// CUIListview
// the listview control
class CUIListview: public CUIWidget
{
protected:
	std::vector<CUIListviewItem*>	m_listItems;	
	int							m_incId;

	float						m_fY;

	int							m_yOffset;	
	int							m_lastfYOffset;
	float						m_fSpeed;

	int							m_allRowHeight;

	bool						m_isMultiSelect;

	int							m_beginTouchY;
	int							m_lastTouchY;
	int							m_currentTouchY;

	bool						m_touchDown;

	core::recti					m_defaultRect;

    int                        m_rowHeight;
protected:
	
	// updateItemMovement	
	void updateItemMovement();

	// updateItemPosition	
	void updateItemPosition();

public:
	CUIListview( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIListview();

	// update
	// update per frame
	virtual void update();

	// onETouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);

	// getItemCount
	// get item on list
	inline int getItemCount()
	{
		return (int)m_listItems.size();
	}

	// isMultiSelect
	// resume multiselect
	inline bool isMultiSelect()
	{
		return m_isMultiSelect;
	}

	// setMultiselect
	// multiselect
	inline void setMultiselect(bool b)
	{
		m_isMultiSelect = b;
	}

	// addItem
	// add a list item
	int addItem();

	// insertItem
	// insert a item
	int insertItem(int index);

	// getItem
	// get item
	CUIListviewItem* getItem(int id);

	// getItemWithUserData
	CUIListviewItem* getItemWithUserData( void *userdata );

	// removeItem
	// remove item
	void removeItem(int id);

	// removeAllItems
	// remove all item
	void removeAllItems();

	// onItemSelect
	// when the item is selected
	virtual void onItemSelect( CUIListviewItem *item, bool selected );
    
    inline void setRowHeight(int h)
    {
        m_rowHeight = h;
    }

	// setSelect
	// select a item
	void setSelect(int index);
};

#endif