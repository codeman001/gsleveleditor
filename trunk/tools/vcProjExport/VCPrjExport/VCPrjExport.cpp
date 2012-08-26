// VCPrjExport.cpp : Defines the entry point for the console application.
// Programer: Pham Hong Duc
// Email: hongduc.pr@gmail.com

#include "stdafx.h"
#include "tinyxml/tinyxml.h"
#include "UIString.h"

#include <string>
#include <list>
using namespace std;

list<string> g_incDir;
list<string> g_libDir;
list<string> g_def;
list<string> g_lib;
list<string> g_cSource;
list<string> g_cppSource;
list<string> g_objs;


void fixedFolderPath(char *lpFolder)
{
	for ( int i = 0, len = strlen(lpFolder); i < len; i++ )
	{
		if ( lpFolder[i] == '\\' )
			lpFolder[i] = '/';
	}
	
	// ./filename.ext -> filename.ext
	if ( lpFolder[0] == '.' && lpFolder[1] == '/' )
	{
		int i = 0, len = 0;
		for ( i = 0, len = strlen(lpFolder) - 2; i < len; i++ )		
			lpFolder[i] = lpFolder[i + 2];
		
		lpFolder[i] = NULL;
	}
}

void readListFolder(const char *lpString, list<string>* list)
{
	if ( lpString == NULL )
		return;
		
	char lpSplit[1024] = {0};
	char lpInc[1024] = {0};
	DWORD pos = 0;
	while ( uiString::split<char>(lpSplit, (char*)lpString, (char*)";", &pos) )
	{
		uiString::getBlock<char,char>(lpInc, lpSplit,"\"");

		if ( uiString::length<char>(lpInc) == 0 )
			uiString::copy<char,char>( lpInc, lpSplit );
		
		fixedFolderPath( lpInc );

		if ( strlen(lpInc) > 0 )
		{
			list->push_back( lpInc );
			printf("    %s\n", lpInc);
		}
	}
}

char *getFileExt(char *lpFileName)
{
	int i = strlen(lpFileName);
	while ( i >= 0 )
	{
		if ( lpFileName[i] == '.' )
			return &lpFileName[i];
		i--;
	}
	return lpFileName;
}

char *getFileName(char *lpPath)
{
	int i = strlen(lpPath);
	while ( i >= 0 )
	{
		if ( lpPath[i] == '/' )
			return &lpPath[i+1];
		i--;
	}
	return lpPath;
}

void readListString(const char *lpString, char *s, list<string>* list)
{
	if ( lpString == NULL )
		return;		

	char lpDef[512] = {0};	
	DWORD pos = 0;
	while ( uiString::split<char>(lpDef, (char*)lpString, s, &pos) )
	{		
		if ( strlen(lpDef) > 0 )
		{
			list->push_back( lpDef );
			printf("    %s\n", lpDef);
		}
		
	}
}

int readConfig(TiXmlNode *pRoot)
{
	TiXmlElement *pConfig = pRoot->FirstChildElement("Configurations");
	if ( pConfig == NULL )
	{
		printf("      Warning: Not found config \n");
		return 1;
	}

	// get first config
	pConfig = pConfig->FirstChildElement();
	printf( "      Read config name: %s\n", pConfig->Attribute("Name") );

	// get tool
	TiXmlElement *pTool = pConfig->FirstChildElement();
	while ( pTool )
	{
		if ( pTool->Attribute("Name") != NULL )
		{
			if ( uiString::comp<const char>( pTool->Attribute("Name"), "VCCLCompilerTool" ) == 0 )
			{				
				printf("\n* list include folder: \n");
				readListFolder( pTool->Attribute("AdditionalIncludeDirectories"), &g_incDir );
				printf("\n* definitions: \n");
				readListString( pTool->Attribute("PreprocessorDefinitions"), (char*)";",&g_def );
			}
			
			if ( uiString::comp<const char>( pTool->Attribute("Name"), "VCLinkerTool" ) == 0 )
			{
				printf("\n* list library folder: \n");
				readListFolder ( pTool->Attribute("AdditionalLibraryDirectories"), &g_libDir );				
				printf("\n* library: \n");
				readListString( pTool->Attribute("AdditionalDependencies"), (char*)" ",&g_lib );
			}
		}
		pTool = pTool->NextSiblingElement();
	}

	return 0;
}

void parseFolderSrc(TiXmlElement *pFilter)
{
	TiXmlElement *pChildFilter = pFilter->FirstChildElement("Filter");
	while ( pChildFilter )
	{
		parseFolderSrc( pChildFilter );
		pChildFilter = pChildFilter->NextSiblingElement("Filter");
	}
	
	char *lpFileName = new char[1024];
	TiXmlElement *pFile = pFilter->FirstChildElement("File");
	while ( pFile )
	{
		strcpy( lpFileName, pFile->Attribute("RelativePath") );
		
		fixedFolderPath(lpFileName);

		char *fileExt = getFileExt(lpFileName);
		
		if ( uiString::comp<char>(fileExt, ".c") == 0 )
		{
			printf("    C Source: %s\n", lpFileName);
			g_cSource.push_back( lpFileName );

			// change to .o
			fileExt[1] = 'o';
			fileExt[2] = 0;
			g_objs.push_back( getFileName(lpFileName) );
		}
		
		if ( uiString::comp<char>(fileExt, ".cpp") == 0 )
		{
			printf("    C++ Source: %s\n", lpFileName);
			g_cppSource.push_back( lpFileName );

			fileExt[1] = 'o';
			fileExt[2] = 0;
			g_objs.push_back( getFileName(lpFileName) );
		}

		pFile = pFile->NextSiblingElement("File");
	}
	delete lpFileName;
}


int readSourceFile(TiXmlNode *pRoot )
{
	TiXmlElement *pFiles = pRoot->FirstChildElement("Files");
	if ( pFiles == NULL )
	{		
		return 1;
	}
	
	parseFolderSrc( pFiles );
	return 0;
}

int parseVCDocument(TiXmlDocument *doc)
{
	TiXmlNode *pRoot = doc->FirstChildElement("VisualStudioProject");

	if ( pRoot == NULL )
	{
		printf("Project error\n");
		return 1;
	}

	readConfig( pRoot );
	
	printf("\n* list source code: \n");
	if ( readSourceFile( pRoot ) == 1 )
	{
		printf("      Error: Not found source files \n");
		return 1;
	}

	return 0;
}


void writeListToFile(FILE *pFile, list<string> *list, bool willContinue)
{
	std::list<std::string>::iterator i, end;

	i = list->begin();
	end = list->end();

	while ( i != end )
	{
		fprintf(pFile, "%s ", i->c_str());

		++i;

		if ( i == end && willContinue == false )
			fprintf(pFile, "\n\n");
		else
			fprintf(pFile, "\\\n");
	}
}

void writeListToFileWithParam(FILE *pFile, list<string> *list, char *param, bool willContinue)
{
	std::list<std::string>::iterator i, end;

	i = list->begin();
	end = list->end();

	while ( i != end )
	{
		fprintf(pFile, "%s%s ", param, i->c_str());

		++i;

		if ( i == end && willContinue == false )
			fprintf(pFile, "\n\n");
		else
			fprintf(pFile, "\\\n");
	}
}

#define WRITE_ANDROIDMAKE

void writePrjMakeFile(FILE *pFile)
{
	#ifdef WRITE_LINUXMAKE
	// write include folder
	if ( g_incDir.size() > 0 )
	{
		fprintf(pFile, "SRCINC_DIR = ");
		writeListToFileWithParam(pFile, &g_incDir,"-I", false);
	}

	if ( g_libDir.size() > 0 )
	{
		fprintf(pFile, "SRCLIB_DIR = ");
		writeListToFileWithParam(pFile, &g_libDir,"-L", false);
	}

	if ( g_lib.size() > 0 )
	{
		fprintf(pFile, "SRCLIB = ");
		writeListToFileWithParam(pFile, &g_lib,"-l", false);
	}

	if ( g_def.size() > 0 )
	{
		fprintf(pFile, "SRCDEF = ");
		writeListToFileWithParam(pFile, &g_def,"-D", false);
	}

	// write source C
	if ( g_cSource.size() > 0 )
	{
		fprintf(pFile, "SOURCE_C = ");
		writeListToFile(pFile, &g_cSource, false);
	}

	// write source CPP
	if ( g_cppSource.size() > 0 )
	{
		fprintf(pFile, "SOURCE_CPP = ");
		writeListToFile(pFile, &g_cppSource, false);
	}

	// write source OBJS
	if ( g_objs.size() > 0 )
	{
		fprintf(pFile, "OBJECTS = ");
		writeListToFile(pFile, &g_objs, false);
	}
	#endif

	#ifdef WRITE_ANDROIDMAKE

	fprintf(pFile, "LOCAL_C_INCLUDES := ");
	if ( g_incDir.size() > 0 )
		writeListToFile(pFile, &g_incDir, false);
	

	fprintf(pFile, "LOCAL_CFLAGS := ");
	if ( g_def.size() > 0 )	
		writeListToFileWithParam(pFile, &g_def,"-D", false);	


	fprintf(pFile, "LOCAL_SRC_FILES := "); 	
	// write source CPP
	if ( g_cppSource.size() > 0 )
		writeListToFile(pFile, &g_cppSource, true);
	
	if ( g_cSource.size() > 0 )	
		writeListToFile(pFile, &g_cSource, false);

	#endif
}


int main(int argc, char* argv[])
{
	if ( argc < 3 )
	{
		printf("Missing params \n");
		return 0;
	}

	char *lpFileName = argv[1];//"vcPrj.xml";
	char *lpOut = argv[2];//"prj.mak";

	TiXmlDocument vcPrjDocument(lpFileName);
	
	printf("Load document %s\n", lpFileName);
	if ( vcPrjDocument.LoadFile() == false )
	{
		printf("Load failed \n");
		return 1;
	}
	
	if ( parseVCDocument( &vcPrjDocument ) == 0 )
	{
		printf("Read document OK \n");

		FILE *pFile = fopen(lpOut,"wt");
		writePrjMakeFile(pFile);
		fclose(pFile);

		printf("WriteMake OK \n");
	}
	else
		printf("Read document error\n");

	return 0;
}

