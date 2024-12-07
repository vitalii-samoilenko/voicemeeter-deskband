#pragma once

#include <windows.h>
#include <windowsx.h>
#include <uxtheme.h>

#include "Error.h"
#include "Messages.h"

namespace Windows {
	inline bool ThrowIfFailed(HRESULT code, const char* what) {
		if (FAILED(code)) {
			throw Error{ code, what };
		}
		return false;
	}

	inline void wRegisterClassW(
		_In_ CONST WNDCLASSW* lpWndClass
	) {
		if (RegisterClassW(
			lpWndClass
		) == 0) {
			throw Error{ "Window class registration failed" };
		}
	}

	inline UINT wRegisterWindowMessageW(
		_In_ LPCWSTR lpString
	) {
		UINT msgId{
			RegisterWindowMessageW(
				lpString
			)
		};
		if (!msgId) {
			throw Error{ "Window message registration failed" };
		}
		return msgId;
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
			throw Error{ "Window creation failed" };
		}
	}

	inline HWND wFindWindowExW(
		_In_opt_ HWND   hWndParent,
		_In_opt_ HWND   hWndChildAfter,
		_In_opt_ LPCWSTR lpClassName,
		_In_opt_ LPCWSTR lpWindowName
	) {
		HWND hWnd{
			FindWindowExW(
				hWndParent,
				hWndChildAfter,
				lpClassName,
				lpWindowName
			)
		};
		if (hWnd == NULL) {
			throw Error{ "Could not find window" };
		}
		return hWnd;
	}

	inline void wPostMessageW(
		_In_opt_ HWND hWnd,
		_In_ UINT Msg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	) {
		if (PostMessageW(
			hWnd,
			Msg,
			wParam,
			lParam
		) == 0) {
			throw Error{ "Post message failed" };
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
			throw Error{ "Unable to get client rectangle" };
		}
	}

	inline void wGetWindowRect(
		_In_ HWND hWnd,
		_Out_ LPRECT lpRect
	) {
		if (GetWindowRect(
			hWnd,
			lpRect
		) == FALSE) {
			throw Error{ "Unable to get window rectangle" };
		}
	}

	inline void wScreenToClient(
		_In_ HWND hWnd,
		_Inout_ LPPOINT lpPoint
	) {
		if (ScreenToClient(
			hWnd,
			lpPoint
		) == FALSE) {
			throw Error{ "Unable to convert coordinates" };
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
			throw Error{ "Unable to move window" };
		}
	}

	inline HDC wBeginPaint(
		_In_ HWND hWnd,
		_Out_ LPPAINTSTRUCT lpPaint
	) {
		HDC hDc{ BeginPaint(
			hWnd,
			lpPaint
		) };
		if (hDc == NULL) {
			throw Error{ "Cannot begin painting" };
		}
		return hDc;
	}

	inline void wSetWindowLongPtrW(
		_In_ HWND hWnd,
		_In_ int nIndex,
		_In_ LONG_PTR dwNewLong
	) {
		SetLastError(0);
		SetWindowLongPtrW(hWnd, nIndex, dwNewLong);
		if (GetLastError()) {
			throw Error{ "Could not set window data" };
		}
	}

	template<typename T>
	inline T* wGetWindowLongPtrW(
		_In_ HWND hWnd,
		_In_ int nIndex
	) {
		SetLastError(0);
		T* pT{ reinterpret_cast<T*>(GetWindowLongPtrW(hWnd, nIndex)) };
		if (GetLastError()) {
			throw Error{ "Could not get window data" };
		}
		return pT;
	}

	inline void wUpdateWindow(
		_In_ HWND hWnd
	) {
		if (UpdateWindow(
			hWnd
		) == FALSE) {
			throw Error{ "Window update failed" };
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
			throw Error{ "Rectangle invalidation failed" };
		}
	}

	inline void wDrawThemeParentBackground(
		_In_ HWND hWnd,
		_In_ HDC hDc,
		_In_opt_ const RECT* pRc
	) {
		HRESULT result{ DrawThemeParentBackground(
			hWnd,
			hDc,
			pRc
		) };
		if (result != S_OK) {
			throw Error{ static_cast<DWORD>(result), "Parent background drawing failed" };
		}
	}

	inline HCURSOR wLoadCursorW(
		_In_opt_ HINSTANCE hInstance,
		_In_ LPCWSTR lpCursorName
	) {
		HCURSOR hCursor{ LoadCursorW(hInstance, lpCursorName) };
		if (hCursor == NULL) {
			throw Error{ "Failed to load cursor" };
		}
		return hCursor;
	}

	inline HANDLE wCreateEventW(
		_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
		_In_ BOOL bManualReset,
		_In_ BOOL bInitialState,
		_In_opt_ LPCWSTR lpName
	) {
		HANDLE event{ CreateEventW(
			lpEventAttributes,
			bManualReset,
			bInitialState,
			lpName
		) };
		if (event == NULL) {
			throw Error{ "Event creation failed" };
		}
		return event;
	}

	inline void wWaitForSingleObject(
		_In_ HANDLE hHandle,
		_In_ DWORD dwMilliseconds
	) {
		if (WaitForSingleObject(
			hHandle,
			dwMilliseconds
		) != 0L) {
			throw Error{ "Failed to wait for single object" };
		}
	}

	inline void wCloseHandle(
		_In_ _Post_ptr_invalid_ HANDLE hObject
	) {
		if (CloseHandle(
			hObject
		) == FALSE) {
			throw Error{ "Failed to close handle" };
		}
	}

	inline void wSetProcessDpiAwarenessContext(
		_In_ DPI_AWARENESS_CONTEXT value
	) {
		if (SetProcessDpiAwarenessContext(
			value
		) == FALSE) {
			throw Error{ "Failed to set process DPI awareness" };
		}
	}

	inline void wAdjustWindowRectExForDpi(
		_Inout_ LPRECT lpRect,
		_In_ DWORD dwStyle,
		_In_ BOOL bMenu,
		_In_ DWORD dwExStyle,
		_In_ UINT dpi
	) {
		if (AdjustWindowRectExForDpi(
			lpRect,
			dwStyle,
			bMenu,
			dwExStyle,
			dpi
		) == FALSE) {
			throw Error{ "Failed to adjust window rectangle for DPI" };
		}
	}

	inline void wSetWindowPos(
		_In_ HWND hWnd,
		_In_opt_ HWND hWndInsertAfter,
		_In_ int X,
		_In_ int Y,
		_In_ int cx,
		_In_ int cy,
		_In_ UINT uFlags
	) {
		if (SetWindowPos(
			hWnd,
			hWndInsertAfter,
			X, Y, cx, cy,
			uFlags
		) == FALSE) {
			throw Error{ "Failed to set window position" };
		}
	}

	inline void wReleaseCapture(
	) {
		if (ReleaseCapture(
		) == FALSE) {
			throw Error{ "Failed to release capture" };
		}
	}

	inline HRSRC wFindResourceW(
		_In_opt_ HMODULE hModule,
		_In_ LPCWSTR lpName,
		_In_ LPCWSTR lpType
	) {
		HRSRC hRsrc{ FindResourceW(
			hModule,
			lpName,
			lpType
		) };
		if (hRsrc == NULL) {
			throw Error{ "Failed to find resource" };
		}
		return hRsrc;
	}

	inline HGLOBAL wLoadResource(
		_In_opt_ HMODULE hModule,
		_In_ HRSRC hResInfo
	) {
		HGLOBAL hGlobal{ LoadResource(
			hModule,
			hResInfo
		) };
		if (hGlobal == NULL) {
			throw Error{ "Failed to load resource" };
		}
		return hGlobal;
	}

	inline LPVOID wLockResource(
		_In_ HGLOBAL hResData
	) {
		LPVOID pData{ LockResource(
			hResData
		) };
		if (pData == NULL) {
			throw Error{ "Failed to lock resource" };
		}
		return pData;
	}

	inline UINT_PTR wSetTimer(
		_In_opt_ HWND hWnd,
		_In_ UINT_PTR nIDEvent,
		_In_ UINT uElapse,
		_In_opt_ TIMERPROC lpTimerFunc
	) {
		UINT_PTR id{ SetTimer(
			hWnd,
			nIDEvent,
			uElapse,
			lpTimerFunc
		) };
		if (id == 0) {
			throw Error{ "Failed to set timer" };
		}
		return id;
	}

	inline void wKillTimer(
		_In_opt_ HWND hWnd,
		_In_ UINT_PTR uIDEvent
	) {
		if (KillTimer(
			hWnd,
			uIDEvent
		) == 0) {
			throw Error{ "Failed to kill timer" };
		}
	}
}