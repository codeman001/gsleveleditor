//
//  CEAGLView.mm
//  gameplay
//
//  Created by Pham Hong Duc on 9/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "CEAGLView.h"


// C++ link func
extern void appicationInitApp();
extern void appicationLoop();
extern void appicationExitApp();
extern void appicationPauseApp();
extern void appicationResumeApp();

extern void appicationResizeWindow(int w, int h);
extern void appicationTouchDown(int touchID, int x, int y);
extern void appicationTouchMove(int touchID, int x, int y);
extern void appicationTouchUp(int touchID, int x, int y);

extern void applicationChangeOrientation(int orientation);

@implementation CEAGLView

@synthesize RunGame;

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	
	if(self)
	{		
		[self setMultipleTouchEnabled:YES];
		[self setExclusiveTouch:YES];
        
        // get GL layer
		CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
		
        // init GL layer
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil];
	}	
	
    RunGame = false;
    
    
    // add orientation
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didRotate:)
                                                 name:@"UIDeviceOrientationDidChangeNotification" object:nil];
    
	return self;
}

- (void) dealloc
{
    [self destroyFramebuffer];
}


- (void) didRotate:(NSNotification *)notification
{	
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    
    if (orientation == UIDeviceOrientationLandscapeLeft)
    {
        NSLog(@"Landscape Left!");
        applicationChangeOrientation(2);
    }
    else if ( orientation == UIDeviceOrientationLandscapeRight )
    {
        NSLog(@"Landscape Right!");
        applicationChangeOrientation(1);
    }
    else 
    {
        NSLog(@"Portrait!");
        applicationChangeOrientation(0);
    }
}


- (void) layoutSubviews
{
    [mEglContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mColorRenderbuffer);    
        
    // get size render buffer
    int backingWidth = 0, backingHeight = 0;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	    
    // resize win
    appicationResizeWindow(backingWidth, backingHeight);                        
}

- (void) drawView:(id)sender
{
    //[EAGLContext setCurrentContext:mEglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, mViewFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);    
   
    // gameloop here
    appicationLoop();
    
    
    [mEglContext presentRenderbuffer:GL_RENDERBUFFER_OES];
}


- (bool) createFramebuffer
{
    // init opengl es 2.0
    mEglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!mEglContext || ![EAGLContext setCurrentContext:mEglContext]) 
	{
		return false;
	}
    
    [mEglContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    
    // create frame buffer
    glGenFramebuffersOES(1, &mViewFramebuffer);
    
    // create render buffer
	glGenRenderbuffersOES(1, &mColorRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mViewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mColorRenderbuffer);    
    
    // attach render buffer to framebuffer
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mColorRenderbuffer);
    
    
    [mEglContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mColorRenderbuffer);
    
    // get size render buffer
    int backingWidth = 0, backingHeight = 0;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
    // create depth buffer
    glGenRenderbuffersOES(1, &mDepthRenderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, mDepthRenderbuffer);
    
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES,mDepthRenderbuffer);
	
    // check error
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
	}
    
    // init game
    appicationInitApp();
    return true;
}

- (void) destroyFramebuffer
{
    if(mViewFramebuffer)
	{
		glDeleteFramebuffersOES(1, &mViewFramebuffer);
		mViewFramebuffer = 0;
	}
    
	if(mColorRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &mColorRenderbuffer);
		mColorRenderbuffer = 0;
	}
	
	if(mDepthRenderbuffer) 
	{
		glDeleteRenderbuffersOES(1, &mDepthRenderbuffer);
		mDepthRenderbuffer = 0;
	}   
    
    if ([EAGLContext currentContext] == mEglContext)
        [EAGLContext setCurrentContext:nil];
    
    mEglContext = nil;
    
    // quit app
    appicationExitApp();
}

- (void) swapFramebuffer
{
    [mEglContext presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void) startGameLoop
{
    if ( RunGame == false )
    {
        mDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
        float animationFrameInterval = 1/40.0f;
        [mDisplayLink setFrameInterval:animationFrameInterval];
        [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];  
    
        RunGame = true;
    }
}

- (void) stopGameLoop
{
    if ( RunGame == true )
    {
        [mDisplayLink invalidate];
        mDisplayLink = nil;
        
        RunGame = false;
    }
}



#pragma mark - GLKView and GLKViewController touches methods

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {
        CGPoint cursor = [touch locationInView:self];        
        appicationTouchDown( (int)touch, (int)cursor.x, (int)cursor.y);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {
        CGPoint cursor = [touch locationInView:self];
        appicationTouchMove((int)touch, (int)cursor.x, (int)cursor.y);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {	
        CGPoint cursor = [touch locationInView:self];
        appicationTouchUp( (int)touch, (int)cursor.x, (int)cursor.y);	
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}


@end