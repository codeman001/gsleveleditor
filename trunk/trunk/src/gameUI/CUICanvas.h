#ifndef _CUICANVAS_H_
#define _CUICANVAS_H_

#include "CUIWidget.h"

class CUICanvas: public CUIWidget
{
protected:

public:
	CUICanvas(const char *name, CUIWidget *parent);
	virtual ~CUICanvas();
};

#endif