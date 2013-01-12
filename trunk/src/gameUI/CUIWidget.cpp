#include "stdafx.h"
#include "CUIWidget.h"
#include "IView.h"

CUIWidget::CUIWidget()
{
	m_parent = NULL;
	m_controlID = -1;
	m_visible = true;
    m_lockActionOnChild = false;
}

CUIWidget::CUIWidget(const char *name, CUIWidget* parent)
{
	m_parent = parent;	
	m_controlID = -1;
	m_visible = true;
    m_lockActionOnChild = false;
    
    m_widgetName = name;
    
    if ( parent )
        parent->addChild( this );
}

CUIWidget::CUIWidget(const char *name, CUIWidget* parent, CMenuFxObj flashObj)
{
	m_parent = parent;	
	m_controlID = -1;
	m_visible = true;
    m_lockActionOnChild = false;
    
    m_widgetName = name;
	m_flashObj = flashObj;

    if ( parent )
        parent->addChild( this );
}

CUIWidget::~CUIWidget()
{
	// destroy all child
	destroyChild();
}

// addChild
// add child widget
void CUIWidget::addChild( CUIWidget *child )
{
	child->remove();

	child->m_parent = this;
	m_childs.push_back( child );
}

// removeChild
// remove a child from childs list
void CUIWidget::removeChild( CUIWidget *child )
{
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		if ( (*i) == child )
		{
			m_childs.erase(i);
			return;
		}

		i++;
	}
}

// destroyChild
// destroy all child
void CUIWidget::destroyChild()
{
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		delete (*i);
		i++;
	}
	m_childs.clear();
}

// remove
// remove from parent
void CUIWidget::remove()
{
	if ( m_parent )
		m_parent->removeChild( this );
}

// getRootWidget
// get root
CUIWidget* CUIWidget::getRootWidget()
{
    CUIWidget* current = this;
    
    while ( current->m_parent != NULL)
        current = current->m_parent;
    
    return current;
}

// update
// update per frame
void CUIWidget::update()
{
	IVideoDriver* driver = getIView()->getDriver();
	
	// update bouding rect
	if ( m_parent == NULL )	
		m_rect = driver->getViewPort();
	else
    {
		getRectByFxObj( m_flashObj, m_rect );
        
        // hide touchzone obj
        m_flashObj.findObj("mcTouchZone").setVisible(false);
    }
    
	//update childs
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		(*i)->update();
		i++;
	}
}

// onTouchEvent
// update touch event
bool CUIWidget::onEvent(const SEvent& gameEvent)
{
    bool ret = false;
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		// delivery touch event to all child
        if ( (*i)->onEvent(gameEvent) == true )
            ret = true;
		i++;
	}
	return ret;
}

// getRectByFxObj (static function)
// get rect from flash obj
void CUIWidget::getRectByFxObj( CMenuFxObj fxObj, core::recti& rect )
{	
	int x,y,w,h;
    float fw, fh;
    
	// get size (pixel) on flash
    fxObj.getAbsoluteBound(&x, &y, &w, &h);

	// get ratio from flash to screen game
	if ( fxObj.getMenu() == NULL )
	{
		fw = 1.0f;
		fh = 1.0f;
	}
	else
		fxObj.getMenu()->getFxScaleRatio(&fw, &fh);
    
	// calc size
    float realX, realY, realW, realH;
    realX = (float)x * fw;
    realY = (float)y * fh;
    realW = (float)w * fw;
    realH = (float)h * fh;

	// result
	rect.UpperLeftCorner	= core::position2di((int)realX,(int)realY);
	rect.LowerRightCorner	= core::position2di((int)(realX+realW),(int)(realY+realH));
}

// setPosition
// set pos
void CUIWidget::setPosition(int x, int y)
{
	float fw, fh;
	if ( m_flashObj.getMenu() == NULL )
	{
		fw = 1.0f;
		fh = 1.0f;
	}
	else
		m_flashObj.getMenu()->getFxScaleRatio(&fw, &fh);

	m_flashObj.setPosition( (int)(x/fw), (int)(y/fh) );
}


// setText
// set string
void CUIWidget::setText( const char *fxName, const char *string )
{
	CMenuFxObj textEdit = m_flashObj.findObj(fxName);
    textEdit.setText( string );
    
    SButtonLabel    label;
    label.label = fxName;
    label.text = string;        
    registerLabel( label );
}
void CUIWidget::setText( const char *fxName, const wchar_t *string )
{
	CMenuFxObj textEdit = m_flashObj.findObj(fxName);
    textEdit.setText( string );
    
    SButtonLabel    label;
    label.label = fxName;
    label.textw = string;        
    registerLabel( label );
}

void CUIWidget::registerLabel(const SButtonLabel &label)
{
    for ( int i = 0; i < (int)m_labels.size(); i++ )
    {
        if ( m_labels[i].label == label.label )
        {
            m_labels[i] = label;
            return;
        }
    }
    m_labels.push_back( label );
}

void CUIWidget::replaceTextOnLabel()
{
    for ( int i = 0; i < (int)m_labels.size(); i++ )
    {
        CMenuFxObj textEdit = m_flashObj.findObj( m_labels[i].label.c_str() );
        
        if ( m_labels[i].text.size() > 0 )
            textEdit.setText( m_labels[i].text.c_str() );
        else
            textEdit.setText( m_labels[i].textw.c_str() );
    }
}