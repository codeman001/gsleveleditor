#ifndef _IDOC_H_
#define _IDOC_H_

#include "stdafx.h"

class IDoc: public uiObject
{
public:
	static long s_objectID;
protected:
	
public:
	// newDocument
	// new 3d scene document
	virtual void newDocument()= 0;
	
	// saveDocument
	// save the document
	virtual bool saveDocument(wchar_t* lpPath) = 0;

	// openDocument
	// load document
	virtual bool openDocument(wchar_t* lpPath) = 0;

	// updateDocument
	// update all object in doc by frame...
	virtual void updateDocument() = 0;

	// drawDocument
	// draw documen by frame
	virtual void drawDocument() = 0;
};

#endif