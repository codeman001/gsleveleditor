//
//  CTextureManager.h
//  gameplay
//
//  Created by Pham Hong Duc on 1/6/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef _CTEXTURE_MANAGER_H_
#define _CTEXTURE_MANAGER_H_

#include "stdafx.h"
#include "uiClass/uiSingleton.h"

class CTextureManager: public uiSingleton<CTextureManager>
{
protected:
    std::vector<ITexture*>  m_textureList;
public:
    CTextureManager();
    ~CTextureManager();
    
    // registerTexture
    // add texture to a list
    void registerTexture(ITexture* tex);
    
    // removeAllTexture
    // free memory texture
    void removeAllTexture();
};


#endif
