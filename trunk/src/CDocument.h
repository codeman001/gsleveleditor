#ifndef _CDOCUMENT_H_
#define _CDOCUMENT_H_

#include "IDoc.h"
#include "CGameObject.h"
#include "CZone.h"

class CDocument: public IDoc
{
protected:
	vector<CZone*>	m_zones;

public:
	CDocument();

	virtual ~CDocument();

	// newDocument
	// new 3d scene document
	virtual void newDocument();
	
	// saveDocument
	// save the document
	virtual bool saveDocument(wchar_t* lpPath);

	// openDocument
	// load document
	virtual bool openDocument(wchar_t* lpPath);

	// updateDocument
	// update all object in doc by frame...
	virtual void updateDocument();

	// drawDocument
	// draw documen by frame
	virtual void drawDocument();
};

#endif