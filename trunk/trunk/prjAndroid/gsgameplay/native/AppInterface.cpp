#include <android/log.h>

extern "C" {
	
	bool g_appInit = true;
	bool g_appResume = false;

	void appicationInitApp()
	{
		if ( g_appInit )
		{
			g_appInit = false;
		}

		if ( g_appResume )
		{			
			g_appResume = false;
		}
	}

	void appicationLoop()
	{		
	}
	
	void appicationExitApp()
	{		
		__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application has exited");
	}

	void appicationPauseApp()
	{		
	}

	void appicationResumeApp()
	{
		g_appResume = true;
	}

	void appicationResizeWindow(int w, int h)
	{		
	}

	void appicationTouchDown(int touchID, int x, int y)
	{	
	}

	void appicationTouchMove(int touchID, int x, int y)
	{		
	}

	void appicationTouchUp(int touchID, int x, int y)
	{		
	}

}
