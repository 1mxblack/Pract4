#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<windows.h>
#include<tchar.h>
#include <sys/types.h>
#include <string.h>
#include <wchar.h>
#include <strsafe.h>
#define ALL_WAY _T("C:\\c\\*")
#define ALL_TRUE_WAY _T("C:\\c\\")
void ErrorExit(LPCTSTR lpszFunction) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));//ВЫДЕЛЯЕТ ИЗ кучи памяти кусок локал памяти
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);//принимает форматирующею строку и возращает отфармотированную строку
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); // создаёт с мемедж окно с текстом
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
void ren_rev(TCHAR* way, TCHAR* true_way) {
	int res = 1;
	WIN32_FIND_DATA file_now;
	HANDLE dir_now = FindFirstFile(way, &file_now);
	if (dir_now != INVALID_HANDLE_VALUE) {

		while (dir_now && res) {
			TCHAR* p_char = file_now.cFileName;
			if (*p_char != '.') {
				if (file_now.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					TCHAR new_way[256], new_true_way[256];
					_tcscpy(new_way, true_way);
					_tcscat(new_way, file_now.cFileName);
					_tcscat(new_way, _T("\\"));
					_tcscpy(new_true_way, new_way);
					_tcscat(new_way, _T("*"));
					ren_rev(new_way, new_true_way);
				}
				else {
					int i = 0, j = 0;
					for (; *p_char != '.' && *p_char; p_char++, i++) {
						res++;
					};
					if (*p_char == '.') {
						TCHAR* p_char_3 = p_char;
						TCHAR str_rev[256];
						TCHAR* p_char_2 = str_rev;
						size_t res_size = 1;
						for (p_char--; i > 0; i--, p_char_2++, p_char--) {
							*p_char_2 = *p_char;
						}
						for (; *p_char_3; p_char_3++, p_char_2++) {
							*p_char_2 = *p_char_3;
						}
						*p_char_2 = 0;
						TCHAR old_name[256];
						_tcscpy(old_name, true_way);
						_tcscat(old_name, file_now.cFileName);
						TCHAR new_name[256];
						_tcscpy(new_name, true_way);
						_tcscat(new_name, str_rev);
						res = MoveFile(old_name, new_name);
						if (!res) {
							ErrorExit(_T("error:> Error of rename files"));
						}
					}
				}
			}
			res = FindNextFile(dir_now, &file_now);
		}
		DWORD d_res = GetLastError();
		if (d_res != ERROR_NO_MORE_FILES) {
			ErrorExit(_T("error:> Error of found more files"));
		}
		res = FindClose(dir_now);
		if (!res) {
			ErrorExit(_T("error:> Error of close files"));
		}
		else printf("work done\n");

	}
	else {
		ErrorExit(_T("error:> Error of found dir"));
	}
}
int main() {
	TCHAR way[256] = ALL_WAY;
	TCHAR true_way[256] = ALL_TRUE_WAY;
	ren_rev(way, true_way);
	return 0;
}
