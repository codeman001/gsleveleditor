package com.gseditor.gsgameplay;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class GameGLES1Render implements GLSurfaceView.Renderer
{
	public void onSurfaceCreated(GL10 gl, EGLConfig config) 
	{
		NativeInterface.getInstance().mainInitApp();
	}

	public void onSurfaceChanged(GL10 gl, int w, int h)
	{
		NativeInterface.getInstance().mainResizeWindow(w, h);
	}

	public void onDrawFrame(GL10 gl) 
	{
		NativeInterface.getInstance().mainLoop();
	}
}