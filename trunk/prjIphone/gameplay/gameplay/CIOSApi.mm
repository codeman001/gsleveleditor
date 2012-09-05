#import "CAppDelegate.h"
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

// get ios device
void deviceDetection()
{
    struct utsname u;
    uname(&u);
	
    printf("IOS device %s \n",u.machine);
	
    if( !strcmp(u.machine,"iPhone3,1") )
    {
		// todo iphone 4
	}
    else if( !strncmp(u.machine,"iPad3,2",5))
    {
		// todo ipad3        
    }
    else if( !strncmp(u.machine,"iPad2,2",5) )
    {
        // todo ipad 2
    }
    else if(  !strcmp(u.machine,"iPad1,1") )
    {
        // todo ipad 1
    }
    else
    {
        // default iphone 4
    }
}

// getDeviceFirmware
// check device firmware
void getDeviceFirmware()
{
    float sdk_version = [[[UIDevice currentDevice] systemVersion] floatValue];      
    
    if (sdk_version >= 3.0)
	{
		// 3.0
	}
	
    if (sdk_version >= 3.2)
	{
		// 3.2
	}
	
    if (sdk_version >= 4.0)
	{
		// > 4.0
	}
}

// getDeviceOrientation
// get orientation
int getDeviceOrientation()
{
	UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    
    switch (orientation)
    {
        case UIDeviceOrientationLandscapeLeft:
            // left
            break;
        case UIDeviceOrientationLandscapeRight:
            // right
            break;
        default:
			// portal
            break;
    };
	return 0;
}

// getDeviceWidthHeight
// return width, height of screen
void getDeviceWidthHeight( int &width, int &height )
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    width = (int)rect.size.width;
    height = (int)rect.size.height;
}