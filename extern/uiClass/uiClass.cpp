// uiClass.cpp 
//
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet:  15/12/2008
//
// Thong tin:
// - DLL bat dau cho thu vien uiClass
// - Phien ban truoc cua uiClass la openGUI duoc bat dau phat trien tu thang 3/2008
//   Do mot so su co tren openGUI nen thu vien da duoc thay doi ten 
//   thanh uiClass.
// - uiClass bat dau duoc chuyen source tu openGUI ngay 15/12/2008


#ifdef _MANAGED
#pragma managed(push, off)
#endif

#include "uiInclude.h"
#include "uiClass.h"
#include "resource.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{		
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif