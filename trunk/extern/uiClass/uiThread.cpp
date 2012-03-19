#include "uiThread.h"

using namespace uiClass;

uiThread::uiThread ()
{
	zm_isDying = 0;
	zm_handle = 0;
}

// beginThread
// Bat dau chay thread
void uiThread::beginThread()
{
	// Huy thread hien tai va tao 1 thread moi
	if ( zm_handle )
		CloseHandle (zm_handle);

	// Tao 1 thread
	zm_handle = CreateThread (
            0, // Security attributes
            0, // Stack size
			uiThread::ThreadEntry,
            (void*)this,
            0,
			&zm_tId);

	
}

uiThread::~uiThread ()
{
	if ( zm_handle )
		CloseHandle (zm_handle);
}

// kill 
// Huy thread
void uiThread::kill ()
{
	zm_isDying = 1;
	this->waitForDeath();
}

// isDie
// Die thread
bool uiThread::isDie()
{
	return ( zm_isDying == 1 );
}

void uiThread::run ()
{
	// Nhay event
	eventThreadFunc(this);
}


// WaitForDeath 
// Cho 2 giay
void uiThread::waitForDeath ()
{
	WaitForSingleObject (zm_handle, 2000);
}

// getHandle
// Lay handle cua thread
HANDLE uiThread::getHandle()
{
	return zm_handle;
}




// static function run thread
DWORD WINAPI uiThread::ThreadEntry (void *pArg)
{
	uiThread* pThread = (uiThread*)pArg;
	pThread->run();

	return 1;
}


uiMutex::uiMutex () 
{ 
	InitializeCriticalSection (& zm_critSection); 
}

uiMutex::~uiMutex () 
{ 
	DeleteCriticalSection (& zm_critSection); 
}
	
// lock 
// Khoa thread lai
void uiMutex::lock ()
{
	EnterCriticalSection (& zm_critSection);
}
		
// release
// Mo khoa thread de tiep tuc tien trinh
void uiMutex::release ()
{
	LeaveCriticalSection (& zm_critSection);
}