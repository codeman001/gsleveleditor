// luaCodeDefine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pugixml.hpp"

#include <string>
#include <vector>

struct SFunction
{
	std::string					name;
	std::string					returnValue;
	std::vector<std::string>	param;	
};


void sortFunctionByName( std::vector<SFunction>& listFunction )
{
	for ( int i = 0; i < listFunction.size() - 1; i++ )
	{
		for ( int j = i + 1; j < listFunction.size(); j++ )
		{
			SFunction& f1 = listFunction[i];
			SFunction& f2 = listFunction[j];
			
			if ( f1.name > f2.name )
			{
				SFunction f = f1;
				f1 = f2;
				f2 = f;
			}
		}
	}
}


int main(int argc, char* argv[])
{
	if ( argc != 3 )	
		return 1;
	
	const char *src = argv[1];
	const char *dst = argv[2];
	
	// open source file
	FILE *file = fopen( src, "rb" );
	fseek(file, 0, SEEK_END);
	unsigned long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// open data
	char *buffer = new char[size];
	fread( buffer, size, 1, file );
	buffer[size] = NULL;
	fclose( file );

	// parse xml
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_buffer(buffer, size);
	if (result.status != pugi::status_ok)
		return 1;

	pugi::xml_node notepad = doc.child("NotepadPlus");	
	pugi::xml_node autoComplete = notepad.child("AutoComplete");
	pugi::xml_node keyWord = autoComplete.child("KeyWord");

	std::vector<SFunction>	listFunction;

	while ( keyWord )
	{
		SFunction func;
		func.name = keyWord.attribute("name").value();

		pugi::xml_node overload = keyWord.child("Overload");
		func.returnValue = overload.attribute("retVal").value();

		pugi::xml_node param = overload.child("Param");
		while ( param )
		{
			func.param.push_back( std::string(param.attribute("name").value()) );
			param = param.next_sibling();
		}

		listFunction.push_back( func );
		keyWord = keyWord.next_sibling();
	}

	sortFunctionByName( listFunction );

	// write xml
	notepad.remove_child( autoComplete );
	autoComplete = notepad.append_child("AutoComplete");

	for ( int i = 0; i < listFunction.size(); i++ )
	{
		SFunction& f = listFunction[i];

		pugi::xml_node keyWord = autoComplete.append_child("KeyWord");
		keyWord.append_attribute("name").set_value( f.name.c_str() );
		keyWord.append_attribute("func").set_value( "yes" );

		pugi::xml_node overload = keyWord.append_child("Overload");
		overload.append_attribute("retVal").set_value( f.returnValue.c_str() );

		for ( int j = 0; j < f.param.size(); j++ )
		{
			pugi::xml_node param = overload.append_child("Param");
			param.append_attribute("name").set_value( f.param[j].c_str() );
		}

	}
	
	doc.save_file( dst );
	return 0;
}

