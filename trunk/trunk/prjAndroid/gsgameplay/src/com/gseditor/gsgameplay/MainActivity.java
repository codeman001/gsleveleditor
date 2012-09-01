package com.gseditor.gsgameplay;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);        
        mGameES1View = new OpenGLES1View(this);
        setContentView(mGameES1View);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return true;
    }
    
    @Override
    protected void onPause() {
        super.onPause();        
    }

    @Override
    protected void onResume() {
        super.onResume();        
    }

    private OpenGLES1View mGameES1View;

    static {
        System.loadLibrary("gsgameplay");
    }
}
