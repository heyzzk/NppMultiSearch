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


#include "conversionPanel.h"
#include "PluginDefinition.h"
#include <stdio.h>
#include "menuCmdID.h"

extern NppData nppData;


#ifdef UNICODE
#define generic_strtoul wcstoul
#define generic_sprintf swprintf
#else
#define generic_strtoul strtoul
#define generic_sprintf sprintf
#endif

#define BCKGRD_COLOR (RGB(255,102,102))
#define TXT_COLOR    (RGB(255,255,255))
#define CF_NPPTEXTLEN	TEXT("Notepad++ Binary Text Length")

UINT ScintillaGetText(HWND hWnd, char* text, INT start, INT end)
{
	Sci_TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = text;
	return (UINT)::SendMessage(hWnd, SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
}

UINT ScintillaMsg(HWND hWnd, UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
UINT ScintillaMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return (UINT)::SendMessage(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK ConversionPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_INITDIALOG :
		{
            generic_string confPath;
			Param mparam;
			
            const TCHAR *pluginConfName = TEXT("MultiSearch.ini");
            TCHAR confDir[MAX_PATH];
            ::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confDir);
            confPath = confDir;
            confPath += TEXT("\\");
            confPath += pluginConfName;
            
            getCmdsFromConf(confPath.c_str(), mparam);
			
			::SendDlgItemMessage(_hSelf, ID_TAG1_EDIT, WM_SETTEXT, 0, (LPARAM)mparam._tag11.c_str());
            ::SendDlgItemMessage(_hSelf, ID_TAG2_EDIT, WM_SETTEXT, 0, (LPARAM)mparam._tag12.c_str());
            ::SendDlgItemMessage(_hSelf, ID_TAG3_EDIT, WM_SETTEXT, 0, (LPARAM)mparam._tag13.c_str());
            ::SendDlgItemMessage(_hSelf, ID_TAG4_EDIT, WM_SETTEXT, 0, (LPARAM)mparam._tag14.c_str());
            ::SendDlgItemMessage(_hSelf, ID_TAG5_EDIT, WM_SETTEXT, 0, (LPARAM)mparam._tag15.c_str());

			return TRUE;
		}
		break;

		case WM_COMMAND : 
		{
			
			switch (wParam)
			{
				case ID_SAVE_BUTTON :	//SAVE
				{
					
					return TRUE;
				}
				case ID_APPLY_BUTTON :  //APPLY
				{
					saveConfig(ID_TAG1_EDIT);
					saveConfig(ID_TAG2_EDIT);
					saveConfig(ID_TAG3_EDIT);
					saveConfig(ID_TAG4_EDIT);
					saveConfig(ID_TAG5_EDIT);
					
					HWND hSciTgt = getCurrentScintillaHandle();
					INT		size = ScintillaMsg(hSciTgt, SCI_GETLENGTH);

					LPSTR	buffer = (LPSTR)new CHAR[size + 1];

					ScintillaGetText(hSciTgt, buffer, 0, size);
					buffer[size] = 0;

					char errMsg[256];
					sprintf(errMsg, "size=%d", size);
					//::MessageBoxA(NULL, errMsg, "DEBUG", MB_OK);

					LPSTR	buf_found = (LPSTR)new CHAR[size + 1];
					memset(buf_found, 0, size + 1);

					char target1[128];
					char target2[128];
					char target3[128];
					char target4[128];
					char target5[128];
					readTextFromUi(ID_TAG1_EDIT, target1);
					readTextFromUi(ID_TAG2_EDIT, target2);
					readTextFromUi(ID_TAG3_EDIT, target3);
					readTextFromUi(ID_TAG4_EDIT, target4);
					readTextFromUi(ID_TAG5_EDIT, target5);

					int len_t1 = (int)strlen(target1);
					int len_t2 = (int)strlen(target2);
					int len_t3 = (int)strlen(target3);
					int len_t4 = (int)strlen(target4);
					int len_t5 = (int)strlen(target5);

					//char tmpbuf[2048];

					int start = 0;
					int cur = 0;
					int matched = 0;
					int len_saved = 0;

					while (cur < size)
					{
						int pos_t1 = 0;
						int pos_t2 = 0;
						int pos_t3 = 0;
						int pos_t4 = 0;
						int pos_t5 = 0;
						int found = 0;
						start = cur;
						while (buffer[cur] != '\n' && buffer[cur] != EOF && cur<size)	//process by line
						{
							if (found == 1) //has found, skip the comparation
							{
								cur++;
								continue;
							}

							buffer[cur] == target1[pos_t1] ? (pos_t1++) : (pos_t1 = 0);
							if (len_t1 > 0 && pos_t1 == len_t1)	//Don't break when found, it needs to move to the \n.
								found = 1;

							buffer[cur] == target2[pos_t2] ? (pos_t2++) : (pos_t2 = 0);
							if (len_t2 > 0 && pos_t2 == len_t2)	//Don't break when found, it needs to move to the \n.
								found = 1;

							buffer[cur] == target3[pos_t3] ? (pos_t3++) : (pos_t3 = 0);
							if (len_t3 > 0 && pos_t3 == len_t3)	//Don't break when found, it needs to move to the \n.
								found = 1;

							buffer[cur] == target4[pos_t4] ? (pos_t4++) : (pos_t4 = 0);
							if (len_t4 > 0 && pos_t4 == len_t4)	//Don't break when found, it needs to move to the \n.
								found = 1;

							buffer[cur] == target5[pos_t5] ? (pos_t5++) : (pos_t5 = 0);
							if (len_t5 > 0 && pos_t5 == len_t5)	//Don't break when found, it needs to move to the \n.
								found = 1;

							cur++;
						}

						if (found == 1)
						{
							memcpy(buf_found + len_saved, buffer + start, cur - start + 1);//add 1 at last which is /n
							len_saved += cur - start + 1;
							matched++;
						}

						cur++;
					}
					//memset(errMsg,0,255);sprintf(errMsg, "which=%d", which);::MessageBoxA(NULL, errMsg, "DEBUG", MB_OK);

					if (matched > 0)//creat a new window to show the result
					{
						::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
						int which = -1;
						::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
						
						if (which != -1)
						{
							HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
							::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)buf_found);
						}
					}else{
						::MessageBoxA(NULL, "Not Found", "Message", MB_OK);
					}

					free(buffer);
					free(buf_found);
					return TRUE;
				}
			}

			return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void ConversionPanel::readTextFromUi(int id, char *res)
{
	const int inStrSize = 256;
	::SendDlgItemMessageA(_hSelf, id, WM_GETTEXT, inStrSize, (LPARAM)res);
}

void ConversionPanel::saveConfig(int id)
{
	const int intStrMaxSize = 256;
	char intStr[intStrMaxSize];
	size_t intStrLen = 0;

	::SendDlgItemMessageA(_hSelf, id, WM_GETTEXT, intStrMaxSize, (LPARAM)intStr);//zzk 这里获取到id对应的字符，保存到intStr
	intStrLen = strlen(intStr);
	
	getConfigAndSave(id,intStr);
}