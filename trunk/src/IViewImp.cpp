#include "stdafx.h"
#include "IView.h"

IView::IView()
{
	m_repaintWindow = true;

	m_device = NULL;
	m_driver = NULL;
	m_smgr = NULL;
}

IView::~IView()
{
}