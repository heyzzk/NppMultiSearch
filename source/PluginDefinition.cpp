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


#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <stdio.h>
#include <Shlwapi.h>
#include <string>
#include "conversionPanel.h"
#include "resource.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;


Param param;
typedef std::basic_string<TCHAR> generic_string;
generic_string confPath;
ConversionPanel _conversionPanel;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	_conversionPanel.init((HINSTANCE)hModule, NULL);
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );

	setCommand(0, TEXT("MultiSearch"), conversionPanel, NULL, false);
	setCommand(1, TEXT("About"), about, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

HWND getCurrentScintillaHandle() {
    int currentEdit;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	return (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
}

const TCHAR *pluginConfName = TEXT("MultiSearch.ini");
const TCHAR *tags = TEXT("tags");

const TCHAR *p_tag11 = TEXT("tag11");
const TCHAR *p_tag12 = TEXT("tag12");
const TCHAR *p_tag13 = TEXT("tag13");
const TCHAR *p_tag14 = TEXT("tag14");
const TCHAR *p_tag15 = TEXT("tag15");

void getCmdsFromConf(const TCHAR *confPathVal, Param & paramVal)
{
	TCHAR cmdNames[MAX_PATH];
	::GetPrivateProfileSectionNames(cmdNames, MAX_PATH, confPathVal);
	TCHAR *pFn = cmdNames;

	if (*pFn && wcscmp(pFn, tags) == 0)
	{
		TCHAR szValue[MAX_PATH + 1] = _T("");
		GetPrivateProfileString(pFn, p_tag11, _T(""), szValue, MAX_PATH, confPathVal);
		paramVal._tag11 = szValue;
		
		GetPrivateProfileString(pFn, p_tag12, _T(""), szValue, MAX_PATH, confPathVal);
		paramVal._tag12 = szValue;
		
		GetPrivateProfileString(pFn, p_tag13, _T(""), szValue, MAX_PATH, confPathVal);
		paramVal._tag13 = szValue;
		
		GetPrivateProfileString(pFn, p_tag14, _T(""), szValue, MAX_PATH, confPathVal);
		paramVal._tag14 = szValue;
		
		GetPrivateProfileString(pFn, p_tag15, _T(""), szValue, MAX_PATH, confPathVal);
		paramVal._tag15 = szValue;
	}
}

void realSaveToConf(const TCHAR *confPathVal, int id, char* str)
{
	WCHAR wsz[128];
	TCHAR cmdNames[MAX_PATH];
	::GetPrivateProfileSectionNames(cmdNames, MAX_PATH, confPathVal);
	TCHAR *pFn = cmdNames;

	if (*pFn && wcscmp(pFn, tags) == 0)
	{
		
		swprintf(wsz, L"%S", str);
		LPCWSTR m_szFilename = wsz;
		
		switch(id)
		{
			case ID_TAG1_EDIT:
				::WritePrivateProfileString(pFn,p_tag11, m_szFilename,confPathVal);
				break;
			case ID_TAG2_EDIT:
				::WritePrivateProfileString(pFn,p_tag12, m_szFilename,confPathVal);
				break;
			case ID_TAG3_EDIT:
				::WritePrivateProfileString(pFn,p_tag13, m_szFilename,confPathVal);
				break;
			case ID_TAG4_EDIT:
				::WritePrivateProfileString(pFn,p_tag14, m_szFilename,confPathVal);
				break;
			case ID_TAG5_EDIT:
				::WritePrivateProfileString(pFn,p_tag15, m_szFilename,confPathVal);
				break;
		}
		
	}
}

// 
// if conf file does not exist, then create it and load it.
void loadConfFile()
{
	TCHAR confDir[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confDir);
	confPath = confDir;
	confPath += TEXT("\\");
	confPath += pluginConfName;
	
	const char confContent[] = "\
[tags]\n\
tag11=\n\
tag12=\n\
tag13=\n\
tag14=\n\
tag15=\n\
\n";

	if (!::PathFileExists(confPath.c_str()))
	{
		FILE *f = generic_fopen(confPath.c_str(), TEXT("w"));
		if (f)
		{
			fwrite(confContent, sizeof(confContent[0]), strlen(confContent), f);
			fflush(f);
			fclose(f);
		}
	}
	getCmdsFromConf(confPath.c_str(), param);
}

void getConfigAndSave(int id, char* str)
{
	TCHAR confDir[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confDir);
	confPath = confDir;
	confPath += TEXT("\\");
	confPath += pluginConfName;
	
	const char confContent[] = "\
[tags]\n\
tag11=\n\
tag12=\n\
tag13=\n\
tag14=\n\
tag15=\n\
\n";

	if (!::PathFileExists(confPath.c_str()))
	{
		FILE *f = generic_fopen(confPath.c_str(), TEXT("w"));
		if (f)
		{
			fwrite(confContent, sizeof(confContent[0]), strlen(confContent), f);
			fflush(f);
			fclose(f);
		}
	}
	getCmdsFromConf(confPath.c_str(), param);
	
	realSaveToConf(confPath.c_str(), id, str);
}

void filterRun1()
{

}
void filterRun2()
{
}
void filterRun3()
{
}

enum hexStat{st_init, st_c, st_cc};

void about()
{
	generic_string aboutMsg = TEXT("Version: ");
	aboutMsg += TEXT("1.0");
	aboutMsg += TEXT("\r\r");
	aboutMsg += TEXT("License: GPL\r\r");
	aboutMsg += TEXT("Author: Zekai Zheng <heyzzk@126.com>\r");
	::MessageBox(nppData._nppHandle, aboutMsg.c_str(), TEXT("MultiSearch Plugin"), MB_OK);
}

void editConf()
{
	if (!::PathFileExists(confPath.c_str()))
	{
		generic_string msg = confPath + TEXT(" is not present.\rPlease create this file manually.");
		::MessageBox(nppData._nppHandle, msg.c_str(), TEXT("Configuration file is absent"), MB_OK);
		return;
	}
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)confPath.c_str());
}

void conversionPanel()
{
	_conversionPanel.setParent(nppData._nppHandle);
	tTbData	data = {0};
	
	if (!_conversionPanel.isCreated())
	{
		_conversionPanel.create(&data);

		// define the default docking behaviour
		data.uMask = DWS_DF_FLOATING;

		data.pszModuleName = _conversionPanel.getPluginFileName();

		// the dlgDlg should be the index of funcItem where the current function pointer is
		// in this case is DOCKABLE_DEMO_INDEX
		data.dlgID = CONVERSIONPANEL_INDEX;
		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	
	_conversionPanel.display();
}