package com.gseditor.gsgameplay;

import android.os.Bundle;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.view.Menu;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);     
		
		// setup landscape
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		
		// setup fullscreen
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		
        if ( NativeInterface.IsUseOpenglES2 == true )
        {
        	mGameES2View = new OpenGLES2View(this);
        	setContentView(mGameES2View);
        }
        else
        {
        	mGameES1View = new OpenGLES1View(this);
        	setContentView(mGameES1View);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return true;
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        NativeInterface.getInstance().mainPauseApp();
    }

    @Override
    protected void onResume() {
        super.onResume();
        NativeInterface.getInstance().mainResumeApp();
    }

    @Override 
    protected void onDestroy(){
    	//NativeInterface.getInstance().mainExitApp();
    	super.onDestroy();    	
    }
    
    private OpenGLES1View mGameES1View = null;
    private OpenGLES2View mGameES2View = null;
}
