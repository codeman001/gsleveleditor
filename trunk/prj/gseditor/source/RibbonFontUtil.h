#ifndef _RIBBON_UTIL_H_
#define _RIBBON_UTIL_H_

#include <windows.h>
#include <UIRibbon.h>
#include <Richedit.h>

#define TWIPS_PER_POINT 20 // For setting font size in CHARFORMAT2.

void GetCharFormat2FromIPropertyStore(IPropertyStore* pPropStore, CHARFORMAT2 *pCharFormat);
void GetIPropStoreFromCharFormat2(const CHARFORMAT2* pCharFormat, IPropertyStore *pPropStore);

#endif