#pragma once

#include "error.h"
#include "../messages.h"

using namespace Voicemeeter::DeskBand::Windows;

inline bool ThrowIfFailed(HRESULT code, const char* what) {
	if (FAILED(code)) {
		throw com_error{ code, what };
	}
	return false;
}

inline void wRegisterClassW(
    _In_ CONST WNDCLASSW* lpWndClass
) {
	if (RegisterClassW(
		lpWndClass
	) == 0) {
		throw windows_error{ "Window class registration failed" };
	}
}
inline void wCreateWindowExW(
    _In_ DWORD dwExStyle,
    _In_opt_ LPCWSTR lpClassName,
    _In_opt_ LPCWSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam
) {
	if (CreateWindowExW(
		dwExStyle,
		lpClassName,
		lpWindowName,
		dwStyle,
		X, Y, nWidth, nHeight,
		hWndParent,
		hMenu,
		hInstance,
		lpParam
	) == NULL) {
		throw windows_error{ "Window creation failed" };
	}
}

inline void wGetClientRect(
	_In_ HWND hWnd,
	_Out_ LPRECT lpRect
) {
	if (GetClientRect(
		hWnd,
		lpRect
	) == FALSE) {
		throw windows_error{ "Unable to get client rectangle" };
	}
}

inline void wMoveWindow(
	_In_ HWND hWnd,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_ BOOL bRepaint
) {
	if (MoveWindow(
		hWnd,
		X, Y, nWidth, nHeight,
		bRepaint
	) == FALSE) {
		throw windows_error{ "Unable to move window" };
	}
}

inline void wBeginPaint(
	_In_ HWND hWnd,
	_Out_ LPPAINTSTRUCT lpPaint
) {
	if (BeginPaint(
		hWnd,
		lpPaint
	) == NULL) {
		throw windows_error{ MSG_ERR_GENERAL, "Cannot begin painting" };
	}
}

inline DWORD wSetWindowLongPtrW(
	_In_ HWND hWnd,
	_In_ int nIndex,
	_In_ LONG_PTR dwNewLong
) {
	SetLastError(0);
	SetWindowLongPtrW(hWnd, nIndex, dwNewLong);
	return GetLastError();
}

template<typename T>
inline T* wGetWindowLongPtrW(
	_In_ HWND hWnd,
	_In_ int nIndex
) {
	SetLastError(0);
	T* pT{ reinterpret_cast<T*>(GetWindowLongPtrW(hWnd, nIndex)) };
	if (GetLastError()) {
		throw windows_error{ "Could not get window data" };
	}
	return pT;
}

inline void wUpdateWindow(
	_In_ HWND hWnd
) {
	if (UpdateWindow(
		hWnd
	) == FALSE) {
		throw windows_error{ "Window update failed" };
	}
}

inline void wInvalidateRect(
	_In_opt_ HWND hWnd,
	_In_opt_ CONST RECT* lpRect,
	_In_ BOOL bErase
) {
	if (InvalidateRect(
		hWnd,
		lpRect,
		bErase
	) == FALSE) {
		throw windows_error{ "Rectangle invalidation failed" };
	}
}

inline HCURSOR wLoadCursorW(
	_In_opt_ HINSTANCE hInstance,
	_In_ LPCWSTR lpCursorName
) {
	HCURSOR hCursor{ LoadCursorW(hInstance, lpCursorName) };
	if (hCursor == NULL) {
		throw windows_error{ "Failed to load cursor" };
	}
	return hCursor;
}