#pragma once

LONG WINAPI unhandled_handler( struct _EXCEPTION_POINTERS* apExceptionInfo );

int MessageBoxTimeoutA( HWND hWnd, LPCSTR lpText,
						LPCSTR lpCaption, UINT uType, WORD wLanguageId,
						DWORD dwMilliseconds );
