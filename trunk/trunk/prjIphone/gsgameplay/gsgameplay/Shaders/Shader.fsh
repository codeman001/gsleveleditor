//
//  Shader.fsh
//  gsgameplay
//
//  Created by Pham Hong Duc on 9/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
