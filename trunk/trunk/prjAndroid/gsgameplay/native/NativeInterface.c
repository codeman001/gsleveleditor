#include <string.h>
#include <jni.h>
#include <android/log.h>


void appicationInitApp();
void appicationLoop();
void appicationExitApp();
void appicationPauseApp();
void appicationResumeApp();
void appicationResizeWindow(int w, int h);
void appicationTouchDown(int touchID, int x, int y);
void appicationTouchMove(int touchID, int x, int y);
void appicationTouchUp(int touchID, int x, int y);

static JNIEnv* g_jniEnv = NULL;
static jclass mClassNativeInterface;

// class com.gseditor.NativeInterface
// native member function mainInitApp
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainInitApp( JNIEnv* env, jobject thiz )
{
    // todo nothing
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so" ,"Native Application Init");

	g_jniEnv = env;

	// init jni call from c to java
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so" ,"Init jni java implement");
	mClassNativeInterface = (*g_jniEnv)->FindClass(g_jniEnv,"com/gseditor/NativeInterface");
	if ( mClassNativeInterface != 0 )
	{
		// todo get java function
	}
	else
		__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so" ,"Can not find class NativeInterface");

	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so" ,"Init jni OK");
	appicationInitApp();
}

// class com.gseditor.NativeInterface
// native member function mainLoop
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainLoop( JNIEnv* env, jobject thiz )
{
	appicationLoop();
}

// class com.gseditor.NativeInterface
// native member function mainExitApp
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainExitApp( JNIEnv* env, jobject thiz )
{
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application Exit");
	appicationExitApp();
}

// class com.gseditor.NativeInterface
// native member function mainPauseApp
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainPauseApp( JNIEnv* env, jobject thiz )
{
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application Pause");
	appicationPauseApp();
}

// class com.gseditor.NativeInterface
// native member function mainResumeApp
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainResumeApp( JNIEnv* env, jobject thiz )
{
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application Resume");
	appicationResumeApp();
}

// class com.gseditor.NativeInterface
// native member function mainResizeWindow
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainResizeWindow( JNIEnv* env, jobject thiz, int w, int h )
{
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application resize: %d %d", w, h);
	appicationResizeWindow(w,h);
}

// class com.gseditor.NativeInterface
// native member function mainTouchDown
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainTouchDown( JNIEnv* env, jobject thiz, int touchID, int x, int y )
{
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application Touchdown: %d %d %d", touchID, x,y );
	appicationTouchDown(touchID, x,y);
}

// class com.gseditor.NativeInterface
// native member function mainTouchMove
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainTouchMove( JNIEnv* env, jobject thiz, int touchID, int x, int y )
{	
	appicationTouchMove(touchID, x,y);
}

// class com.gseditor.NativeInterface
// native member function mainTouchUp
JNIEXPORT void JNICALL Java_com_gseditor_NativeInterface_mainTouchUp( JNIEnv* env, jobject thiz, int touchID, int x, int y)
{
	__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application Touchup: %d %d %d", touchID, x,y );
	appicationTouchUp(touchID, x,y);
}