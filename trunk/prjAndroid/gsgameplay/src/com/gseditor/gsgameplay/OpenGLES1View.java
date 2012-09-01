package com.gseditor.gsgameplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class OpenGLES1View extends GLSurfaceView 
{
	public OpenGLES1View(Context context) {
        super(context);
        mRenderer = new GameGLES1Render();
        setRenderer(mRenderer);
    }
      
    
    public boolean onTouchEvent(final MotionEvent event) 
    {
    	 int action = event.getAction();
    	 int actionCode = action & MotionEvent.ACTION_MASK;
    	   
        if (actionCode == MotionEvent.ACTION_DOWN || actionCode == MotionEvent.ACTION_POINTER_DOWN) 
        {
        	for (int i = 0; i < event.getPointerCount(); i++) 
        	{
        		NativeInterface.getInstance().mainTouchDown(event.getPointerId(i), (int) event.getX(i), (int) event.getY(i));
        	}
        }
        else if ( actionCode == MotionEvent.ACTION_POINTER_UP)
        {
        	int idTouchUp = action >> MotionEvent.ACTION_POINTER_ID_SHIFT; 
        	NativeInterface.getInstance().mainTouchUp(idTouchUp, (int) event.getX(idTouchUp), (int) event.getY(idTouchUp));        	
        }
        else if ( actionCode == MotionEvent.ACTION_UP )
        {
        	for (int i = 0; i < event.getPointerCount(); i++) 
        	{
        		NativeInterface.getInstance().mainTouchUp(event.getPointerId(i), (int) event.getX(i), (int) event.getY(i));
        	}        	
        }
        else if ( actionCode == MotionEvent.ACTION_MOVE)
        {
        	for (int i = 0; i < event.getPointerCount(); i++) 
        	{
        		NativeInterface.getInstance().mainTouchMove(event.getPointerId(i), (int) event.getX(i), (int) event.getY(i));
        	}	
        }
        return true;
    }
    
    GameGLES1Render mRenderer;
}