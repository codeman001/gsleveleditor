package com.gseditor.gsgameplay;

public class NativeInterface {
	
	public static NativeInterface	sInstance = null;
	
	static public NativeInterface getInstance()
	{
		if ( sInstance == null )
			sInstance = new NativeInterface();
		return sInstance;
	}
	
	public native void mainInitApp();
	
	public native void mainLoop();

	public native void mainExitApp();

	public native void mainPauseApp();

	public native void mainResumeApp();

	public native void mainResizeWindow(int w, int h);
	
	public native void mainTouchDown(int touchID, int x, int y);

	public native void mainTouchMove(int touchID, int x, int y);

	public native void mainTouchUp(int touchID, int x, int y);
	
	public native void updateCurrentLocation(double lat, double lng);
	
	static {
		System.loadLibrary("gsgameplay");
	}
	
}
