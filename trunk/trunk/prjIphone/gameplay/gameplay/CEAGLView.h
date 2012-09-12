//
//  CEAGLView.h
//  gameplay
//
//  Created by Pham Hong Duc on 9/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface CEAGLView : UIView 
{    
@private
    GLuint mViewFramebuffer;
    GLuint mColorRenderbuffer;
    GLuint mDepthRenderbuffer;
    
    EAGLContext* mEglContext;
    
    id mDisplayLink;
}

@property (readwrite, nonatomic) bool RunGame;

- (id) initWithFrame:(CGRect)frame;


- (bool) createFramebuffer;
- (void) destroyFramebuffer;
- (void) swapFramebuffer;
- (void) startGameLoop;
- (void) stopGameLoop;

@end

