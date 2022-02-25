// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

//
// All difinitions of plugin interface
//
#include "PluginInterface.h"
#include <string>

typedef std::basic_string<TCHAR> generic_string;
//-------------------------------------//
//-- STEP 1. DEFINE YOUR PLUGIN NAME --//
//-------------------------------------//
// Here define your plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT("MultiSearch");

//-----------------------------------------------//
//-- STEP 2. DEFINE YOUR PLUGIN COMMAND NUMBER --//
//-----------------------------------------------//
//
// Here define the number of your plugin commands
//
const int nbFunc = 2;
#define CONVERSIONPANEL_INDEX 3

//
// Initialization of your plugin data
// It will be called while plugin loading
//
void pluginInit(HANDLE hModule);

//
// Cleaning of your plugin
// It will be called while plugin unloading
//
void pluginCleanUp();

//
//Initialization of your plugin commands
//
void commandMenuInit();

//
//Clean up your plugin commands allocation (if any)
//
void commandMenuCleanUp();

//
// Function which sets your command 
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);


//
// Your plugin command functions
//

#ifdef UNICODE
	#define NppMainEntry wWinMain
	#define generic_strtol wcstol
	#define generic_strncpy wcsncpy
	#define generic_stricmp wcsicmp
	#define generic_strncmp wcsncmp
	#define generic_strnicmp wcsnicmp
	#define generic_strncat wcsncat
	#define generic_strchr wcschr
	#define generic_atoi _wtoi
	#define generic_itoa _itow
	#define generic_atof _wtof
	#define generic_strtok wcstok
	#define generic_strftime wcsftime
	#define generic_fprintf fwprintf
	#define generic_sscanf swscanf
	#define generic_fopen _wfopen
	#define generic_fgets fgetws
	#define generic_stat _wstat
	#define generic_sprintf swprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESW
#else
	#define NppMainEntry WinMain
	#define generic_strtol strtol
	#define generic_strncpy strncpy
	#define generic_stricmp stricmp
	#define generic_strncmp strncmp
	#define generic_strnicmp strnicmp
	#define generic_strncat strncat
	#define generic_strchr strchr
	#define generic_atoi atoi
	#define generic_itoa itoa
	#define generic_atof atof
	#define generic_strtok strtok
	#define generic_strftime strftime
	#define generic_fprintf fprintf
	#define generic_sscanf sscanf
	#define generic_fopen fopen
	#define generic_fgets fgets
	#define generic_stat _stat
	#define generic_sprintf sprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESA
#endif

struct Param  {
	generic_string _tag11;
	generic_string _tag12;
	generic_string _tag13;
	generic_string _tag14;
	generic_string _tag15;
	//Param():_tag11(''),_tag12(''),_tag13(''),_tag14(''),_tag15('') {};
};

void getCmdsFromConf(const TCHAR *confPath, Param & param);
void loadConfFile();
void conversionPanel();
void editConf();
void about();

void filterRun1();
void filterRun2();
void filterRun3();

void getConfigAndSave(int id, char* str);

void realSaveToConf(const TCHAR *confPathVal, int id, generic_string str);

#endif //PLUGINDEFINITION_H
