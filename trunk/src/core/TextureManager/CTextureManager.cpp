//
//  CTextureManager.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 1/6/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "IView.h"
#include "CTextureManager.h"

CTextureManager::CTextureManager()
{
    
}

CTextureManager::~CTextureManager()
{
    
}

// registerTexture
// add texture to a list
void CTextureManager::registerTexture(ITexture* tex)
{
    if (tex == NULL)
        return;
        
    std::vector<ITexture*>::iterator i = m_textureList.begin(), end = m_textureList.end();
    while (i != end)
    {
        if ( (*i) == tex )
            return;
        
        i++;
    }    
    m_textureList.push_back(tex);
}

// removeAllTexture
// free memory texture
void CTextureManager::removeAllTexture()
{
    IVideoDriver *driver = getIView()->getDriver();
    
    std::vector<ITexture*>::iterator i = m_textureList.begin(), end = m_textureList.end();
    while (i != end )
    {
        printf("Remove Texture: %s\n", (*i)->getName().getPath().c_str() );
        driver->removeTexture( (*i) );
        i++;
    }
    m_textureList.clear();
}