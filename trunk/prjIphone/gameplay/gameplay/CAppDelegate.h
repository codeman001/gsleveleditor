//
//  CAppDelegate.h
//  gameplay
//
//  Created by Pham Hong Duc on 9/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CEAGLView.h"

@interface CAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow  *window;
@property (strong, nonatomic) CEAGLView *glView;

@end
