//
//  CViewController.m
//  gameplay
//
//  Created by Pham Hong Duc on 9/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifdef IOS5

#import "CViewController.h"

// C++ link func
extern void appicationInitApp();
extern void appicationLoop();
extern void appicationExitApp();
extern void appicationExitApp();
extern void appicationPauseApp();
extern void appicationResumeApp();
extern void appicationResizeWindow(int w, int h);
extern void appicationTouchDown(int touchID, int x, int y);
extern void appicationTouchMove(int touchID, int x, int y);
extern void appicationTouchUp(int touchID, int x, int y);


@interface CViewController () {

}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation CViewController

@synthesize context = _context;
@synthesize effect = _effect;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    //self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
    
    [self setupGL];
    
    // init game
    appicationInitApp();
}

- (void)viewDidUnload
{   
    // quit game
    appicationExitApp();
    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
        
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
   
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    appicationLoop();
}

#pragma mark - GLKView and GLKViewController touches methods

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {
        CGPoint cursor = [touch locationInView:self.view];        
        appicationTouchDown( (int)touch, (int)cursor.x, (int)cursor.y);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {
        CGPoint cursor = [touch locationInView:self.view];
        appicationTouchMove((int)touch, (int)cursor.x, (int)cursor.y);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch* touch in touches)
    {	
        CGPoint cursor = [touch locationInView:self.view];
        appicationTouchUp( (int)touch, (int)cursor.x, (int)cursor.y);	
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}

@end

#endif