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

    public boolean onTouchEvent(final MotionEvent event) {
        
        return true;
    }    
    
    GameGLES1Render mRenderer;
}