#include "stdafx.h"
#include "CUIListview.h"
#include "IView.h"

/////////////////////////////////////////////////////////
// CUIListviewItem implement
/////////////////////////////////////////////////////////

CUIListviewItem::CUIListviewItem( const char *name, CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(name, parent, flashObj)
{
	m_listview = (CUIListview*)parent;
	
	m_selected	= false;
	m_hover		= false;
}

CUIListviewItem::~CUIListviewItem()
{
}

// update
// update per frame
void CUIListviewItem::update()
{
	CUIWidget::update();
}

// onTouchEvent
// update touch event
bool CUIListviewItem::onEvent( const SEvent& gameEvent)
{
	// if childs handle this event
    if ( CUIWidget::onEvent(gameEvent) == true )
        return true;
	
	bool ret = false;
	CUIWidget *root = getRootWidget();
	if ( root->isLockActionOnChild() )
		return false;

	// process button action
	if ( gameEvent.EventType == EET_MOUSE_INPUT_EVENT )
	{
		// check mouse hit this button
		core::position2di mousePos(gameEvent.MouseInput.X,gameEvent.MouseInput.Y);

        // check mouse over on button
		bool mouseOver = m_rect.isPointInside(mousePos);
        
		// when mouse out the button
		if ( mouseOver == true )
		{
			if (gameEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN || gameEvent.MouseInput.Event == EMIE_MOUSE_MOVED)
			{
				if ( m_hover == false )
				{
                    m_flashObj.gotoFrame("focus", true);
					m_hover = true;
				}
			}
			else if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
			{
				if ( m_listview->isMultiSelect() == true )
					m_selected = !m_selected;
				else
                {
					m_selected = true;
                    m_flashObj.gotoFrame("selected", true);
                }
                
				m_listview->onItemSelect( this, m_selected );
				m_hover = false;
			}

			// handle this event
			ret = true;
		}
		else
		{
			if ( m_hover == true && m_selected == false )
			{
				m_flashObj.gotoFrame("normal", true);
				m_hover = false;
			}
		}
	}

	return ret;
}

// setSelected
// set selected
void CUIListviewItem::setSelected(bool b)
{
	m_selected = false;
	m_hover = false;
	m_flashObj.gotoFrame("normal", true);
}


/////////////////////////////////////////////////////////
// CUIListview implement
/////////////////////////////////////////////////////////

CUIListview::CUIListview( const char *name, CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(name, parent, flashObj)
{
	m_focus = -1;
	m_incId = 0;

	m_yOffset		= 0;
	m_lastfYOffset	= 0;

	m_fY		= 0.0f;
	m_fSpeed	= 0.0f;

	m_allRowHeight	= 0;
	m_isMultiSelect = false;

	m_beginTouchY	= -1;
	m_lastTouchY	= -1;
    m_rowHeight     = -1;
    
	m_touchDown = false;

	getRectByFxObj(flashObj, m_defaultRect);
    
    CMenuFxObj	mask = m_flashObj.findObj("listZone");
    mask.setVisible(false);
}

CUIListview::~CUIListview()
{
	removeAllItems();
}

// update
// update per frame
void CUIListview::update()
{
	CUIWidget::update();

	updateItemMovement();
	updateItemPosition();
}

// onTouchEvent
// update touch event
bool CUIListview::onEvent( const SEvent& gameEvent)
{
	float frameTime = getIView()->getTimeStep();

	bool ret = false;
	CUIWidget *root = getRootWidget();

	// mouse event
	if ( gameEvent.EventType == EET_MOUSE_INPUT_EVENT )
	{
		// check mouse hit this button
		core::position2di mousePos(gameEvent.MouseInput.X,gameEvent.MouseInput.Y);

        // check mouse over on button
		bool mouseOver = m_defaultRect.isPointInside(mousePos);
        		

		if (gameEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN && mouseOver == true )
		{
            CUIWidget::onEvent(gameEvent);
            
			root->lockActionOnChilld( true );

			m_beginTouchY	= mousePos.Y;
			m_currentTouchY	= mousePos.Y;
			m_lastTouchY	= mousePos.Y;

			m_lastfYOffset	= m_yOffset;

			m_touchDown = true;
			ret = true;
		}	
		else if ( gameEvent.MouseInput.Event == EMIE_MOUSE_MOVED && m_touchDown )
		{
			if ( mouseOver == false )
			{
				// drag out control
				m_touchDown = false;
				m_beginTouchY = -1;
				m_currentTouchY = -1;
				m_fY = (float)m_yOffset;

				root->lockActionOnChilld( false );				
			}
			else
			{
				m_currentTouchY = mousePos.Y;				
			}
			ret = true;
		}
		else if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP && m_touchDown )
		{
			root->lockActionOnChilld( false );
			
			m_touchDown = false;
			m_beginTouchY = -1;
			m_currentTouchY = -1;
			m_fY = (float)m_yOffset;

			ret = true;
            
            CUIWidget::onEvent(gameEvent);            
		}
	}

	return ret;
}

// addItem
// add a list item
int CUIListview::addItem()
{
	char name[128];
	sprintf(name,"%s_%d", m_widgetName.c_str(), ++m_incId);

	gameswf::as_value param(name);	
	m_flashObj.invokeASCallback("addListItem", &param, 1);

	CMenuFxObj	list = m_flashObj.findObj(name);	

	// add error
	if ( list.isNull() == true )
	{		
		char text[128];
		sprintf(text,"Can not insert item %s", name);
		os::Printer::log(text, ELL_ERROR);		
		return -1;
	}

	// set mask clip
	CMenuFxObj	mask = m_flashObj.findObj("listZone");
	list.setMaskClip( mask );

	// add item
	CUIListviewItem *item = new CUIListviewItem(name, this, list);
	m_listItems.push_back( item );

	return (int)(m_listItems.size() - 1);
}

// insertItem
// insert a item
int CUIListview::insertItem(int index)
{	
	if ( index < 0 || index >= getItemCount() )
		return addItem();

	return -1;
}

// getItem
// get item
CUIListviewItem* CUIListview::getItem(int id)
{
	if ( id < 0 || id >= getItemCount() )
		return NULL;

	return m_listItems[id];
}

// removeItem
// remove item
void CUIListview::removeItem(int id)
{
	if ( id < 0 || id >= getItemCount() )
		return;

	CUIListviewItem *item = m_listItems[id];

	// remove flash obj
	gameswf::as_value param( item->getName() );	
	m_flashObj.invokeASCallback("removeListItem", &param, 1);

	// remove item
	item->remove();
	delete item;

	// erase in array
	m_listItems.erase( m_listItems.begin() + id );
}

// removeAllItems
// remove all item
void CUIListview::removeAllItems()
{
	for ( int i = 0, n = getItemCount(); i < n; i++ )
	{
		CUIListviewItem *item = m_listItems[i];
		
		// remove flash obj
		gameswf::as_value param( item->getName() );	
		m_flashObj.invokeASCallback("removeListItem", &param, 1);

		// remove item
		item->remove();
		delete item;
	}

	m_listItems.clear();
	m_incId = 0;
}

// updateItemMovement	
void CUIListview::updateItemMovement()
{	
	const float FRICTION_COEF       = 0.007f;
    const float ELASTIC_COEF        = 0.0001f;
    const float RET_FR_COEF         = 0.014f;              

	float frameTime = getIView()->getTimeStep();
	float height	= (float)m_defaultRect.getHeight();
	float rowHeight = (float)m_allRowHeight;


	if ( m_touchDown == true )
	{
		m_fSpeed = (float)( m_currentTouchY - m_lastTouchY)*0.06f;
		m_lastTouchY = m_currentTouchY;

		// move offset of list items
		m_yOffset = m_lastfYOffset + (m_currentTouchY - m_beginTouchY);
	}    
	else
    {
		if (m_fY > 0.5f)
		{
			m_fSpeed -= frameTime * m_fY * ELASTIC_COEF;
            if (m_fSpeed < 0.0f)
				m_fSpeed -= core::min_<float>(RET_FR_COEF * frameTime, 1.0f) * m_fSpeed; // suplementary friction to avoid oscilation
		}
		else if (m_fY + core::max_<float>(height, rowHeight) - height < -0.5f)
        {
			m_fSpeed += frameTime * (height - (m_fY + core::max_<float>(height, rowHeight))) * ELASTIC_COEF;
            if (m_fSpeed > 0.0f)
				m_fSpeed -= core::min_<float>(RET_FR_COEF * frameTime, 1.0f) * m_fSpeed; // suplementary friction to avoid oscilation
		}
        else
        {
			m_fSpeed -= core::min_<float>(FRICTION_COEF * frameTime, 1.0f) * m_fSpeed; // friction

            // no slow moving
            if (fabsf(m_fSpeed) < 0.005f)
				m_fSpeed = 0.0f;
		}
        m_fY += m_fSpeed * frameTime;
        m_yOffset = (int)m_fY;
    }
}

// updateItemPosition
void CUIListview::updateItemPosition()
{
	int yItem = m_yOffset;
	m_allRowHeight = 0;

	for (int i = 0, n = m_listItems.size();  i < n; i++ )
	{
		CUIListviewItem *item = m_listItems[i];
		item->setPosition(0, yItem);
        
        if ( m_rowHeight == -1 )
        {
            yItem += item->getHeight();
            m_allRowHeight += item->getHeight();
        }
        else 
        {
            yItem += m_rowHeight;
            m_allRowHeight += m_rowHeight;
        }
	}
}

// onItemSelect
// when the item is selected
void CUIListview::onItemSelect( CUIListviewItem *item, bool selected )
{
	if ( m_isMultiSelect == false )
	{
		for (int i = 0, n = m_listItems.size();  i < n; i++ )
		{
			CUIListviewItem *it = m_listItems[i];
			
			if ( item != it )
				it->setSelected( false );
		}
	}
}