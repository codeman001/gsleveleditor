//
//  macosxapi.mm
//  gameplay
//
//  Created by Pham Hong Duc on 9/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <sys/utsname.h>

// getResourcePath
// get path of application
void getResourcePath(char* cBuffer, int iLength)
{	
	NSString* readPath = [[NSBundle mainBundle] resourcePath];
	[readPath getCString:cBuffer maxLength:iLength encoding:NSASCIIStringEncoding];
}

// getSavePath
// get directory to write file
void getSavePath(char* cBuffer, int iLength )
{
	//get the name of the app
	NSBundle*	bundle = [NSBundle mainBundle];
	NSString* 	bundleName = [[bundle infoDictionary] objectForKey:@"CFBundleName"];
	
	//get the path for library dir
	NSString* savePath = [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	
	//append a title in path	
	savePath = [savePath stringByAppendingFormat:@"/%@", bundleName];
	
	//create folder
	NSFileManager* fileManager = [NSFileManager defaultManager];	
	if([fileManager fileExistsAtPath:savePath] == NO)
	{
		NSError *pError;
		if([fileManager createDirectoryAtPath:savePath withIntermediateDirectories:YES attributes:nil error:&pError] == NO)
		{
			savePath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
			NSLog(@"Error %@", pError);
		}
	}
	
	// get to C++ buffer
	savePath = [savePath stringByAppendingFormat:@"/"];	
	[savePath getCString:cBuffer maxLength:iLength encoding:NSASCIIStringEncoding];
}
