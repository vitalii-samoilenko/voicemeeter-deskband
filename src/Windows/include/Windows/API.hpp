#ifndef WINDOWS_API_HPP
#define WINDOWS_API_HPP

#include "Windows/Error.hpp"

namespace Windows {
	inline ATOM RegisterClassW(
		_In_ CONST WNDCLASSW* lpWndClass
	) {
		ATOM atom{
			::RegisterClassW(
				lpWndClass)
		};
		if (atom == 0) {
			throw Error{ "Window class registration failed" };
		}
		return atom;
	};

	inline UINT RegisterWindowMessageW(
		_In_ LPCWSTR lpString
	) {
		UINT msgId{
			::RegisterWindowMessageW(
				lpString)
		};
		if (msgId == 0) {
			throw Error{ "Window message registration failed" };
		}
		return msgId;
	};

	inline HWND CreateWindowExW(
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
		HWND hWnd{
			::CreateWindowExW(
				dwExStyle,
				lpClassName,
				lpWindowName,
				dwStyle,
				X, Y,
				nWidth, nHeight,
				hWndParent,
				hMenu,
				hInstance,
				lpParam)
		};
		if (hWnd == NULL) {
			throw Error{ "Window creation failed" };
		}
		return hWnd;
	};

	inline HWND FindWindowExW(
		_In_opt_ HWND   hWndParent,
		_In_opt_ HWND   hWndChildAfter,
		_In_opt_ LPCWSTR lpClassName,
		_In_opt_ LPCWSTR lpWindowName
	) {
		HWND hWnd{
			::FindWindowExW(
				hWndParent,
				hWndChildAfter,
				lpClassName,
				lpWindowName)
		};
		if (hWnd == NULL) {
			throw Error{ "Could not find window" };
		}
		return hWnd;
	};

	inline DWORD GetTempPathW(
		_In_ DWORD nBufferLength,
		_Out_ LPWSTR lpBuffer
	) {
		DWORD length{
			::GetTempPathW(
				nBufferLength,
				lpBuffer)
		};
		if (length == 0) {
			throw Error{ "Could not get temp path" };
		}
		return length;
	};

	inline BOOL PostMessageW(
		_In_opt_ HWND hWnd,
		_In_ UINT Msg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	) {
		BOOL code{
			::PostMessageW(
				hWnd,
				Msg,
				wParam,
				lParam)
		};
		if (code == 0) {
			throw Error{ "Post message failed" };
		}
		return code;
	};

	inline HWND SetParent(
		_In_ HWND hWndChild,
		_In_opt_ HWND hWndNewParent
	) {
		HWND hWndParent{
			::SetParent(
				hWndChild,
				hWndNewParent)
		};
		if (hWndParent == NULL) {
			throw new Error{ "Set parent failed" };
		}
		return hWndParent;
	};

	inline BOOL GetClientRect(
		_In_ HWND hWnd,
		_Out_ LPRECT lpRect
	) {
		BOOL code{
			::GetClientRect(
				hWnd,
				lpRect)
		};
		if (code == 0) {
			throw Error{ "Unable to get client rectangle" };
		}
		return code;
	};

	inline HANDLE CreateEventW(
		_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
		_In_ BOOL bManualReset,
		_In_ BOOL bInitialState,
		_In_opt_ LPCWSTR lpName
	) {
		HANDLE hEvent{
			::CreateEventW(
				lpEventAttributes,
				bManualReset,
				bInitialState,
				lpName)
		};
		if (hEvent == NULL) {
			throw new Error{ "Event creation failed" };
		}
		return hEvent;
	};

	inline BOOL GetWindowRect(
		_In_ HWND hWnd,
		_Out_ LPRECT lpRect
	) {
		BOOL code{
			::GetWindowRect(
				hWnd,
				lpRect)
		};
		if (code == 0) {
			throw Error{ "Unable to get window rectangle" };
		}
		return code;
	};

	inline BOOL ScreenToClient(
		_In_ HWND hWnd,
		_Inout_ LPPOINT lpPoint
	) {
		BOOL code{
			::ScreenToClient(
				hWnd,
				lpPoint)
		};
		if (code == 0) {
			throw Error{ "Unable to convert coordinates" };
		}
		return code;
	};

	inline BOOL MoveWindow(
		_In_ HWND hWnd,
		_In_ int X,
		_In_ int Y,
		_In_ int nWidth,
		_In_ int nHeight,
		_In_ BOOL bRepaint
	) {
		BOOL code{
			::MoveWindow(
				hWnd,
				X, Y,
				nWidth, nHeight,
				bRepaint)
		};
		if (code == 0) {
			throw Error{ "Unable to move window" };
		}
		return code;
	};

	inline HDC BeginPaint(
		_In_ HWND hWnd,
		_Out_ LPPAINTSTRUCT lpPaint
	) {
		HDC hDc{ 
			::BeginPaint(
				hWnd,
				lpPaint)
		};
		if (hDc == NULL) {
			throw Error{ "Cannot begin painting" };
		}
		return hDc;
	};

	inline LONG_PTR SetWindowLongPtrW(
		_In_ HWND hWnd,
		_In_ int nIndex,
		_In_ LONG_PTR dwNewLong
	) {
		::SetLastError(0);
		LONG_PTR dwOldLong{
			::SetWindowLongPtrW(
				hWnd,
				nIndex,
				dwNewLong);
		};
		DWORD code{
			::GetLastError()
		};
		if (code != 0) {
			throw Error{ code, "Could not set window data" };
		}
		return dwOldLong;
	};

	inline LONG_PTR GetWindowLongPtrW(
		_In_ HWND hWnd,
		_In_ int nIndex
	) {
		::SetLastError(0);
		LONG_PTR dwLong{
			::GetWindowLongPtrW(
				hWnd,
				nIndex)
		};
		DWORD code{
			::GetLastError()
		};
		if (code != 0) {
			throw Error{ code, "Could not get window data" };
		}
		return dwLong;
	};

	inline BOOL UpdateWindow(
		_In_ HWND hWnd
	) {
		BOOL code{
			::UpdateWindow(
				hWnd)
		};
		if (code == 0) {
			throw Error{ "Window update failed" };
		}
		return code;
	};

	inline BOOL InvalidateRect(
		_In_opt_ HWND hWnd,
		_In_opt_ CONST RECT* lpRect,
		_In_ BOOL bErase
	) {
		BOOL code{
			::InvalidateRect(
				hWnd,
				lpRect,
				bErase)
		};
		if (code == 0) {
			throw Error{ "Rectangle invalidation failed" };
		}
		return code;
	};

	inline HCURSOR LoadCursorW(
		_In_opt_ HINSTANCE hInstance,
		_In_ LPCWSTR lpCursorName
	) {
		HCURSOR hCursor{
			::LoadCursorW(
				hInstance,
				lpCursorName)
		};
		if (hCursor == NULL) {
			throw Error{ "Failed to load cursor" };
		}
		return hCursor;
	};

	inline DWORD WaitForSingleObject(
		_In_ HANDLE hObject,
		_In_ DWORD dwMilliseconds
	) {
		DWORD code{
			::WaitForSingleObject(
				hObject,
				dwMilliseconds)
		};
		if (code != 0) {
			throw Error{ "Failed to wait for single object" };
		}
		return code;
	};

	inline BOOL CloseHandle(
		_In_ _Post_ptr_invalid_ HANDLE hObject
	) {
		BOOL code{
			::CloseHandle(
				hObject)
		};
		if (code == 0) {
			throw Error{ "Failed to close handle" };
		}
		return code;
	};

	inline BOOL SetProcessDpiAwarenessContext(
		_In_ DPI_AWARENESS_CONTEXT value
	) {
		BOOL code{
			::SetProcessDpiAwarenessContext(
				value)
		};
		if (code == FALSE) {
			throw Error{ "Failed to set process DPI awareness" };
		}
		return code;
	};

	inline BOOL AdjustWindowRectExForDpi(
		_Inout_ LPRECT lpRect,
		_In_ DWORD dwStyle,
		_In_ BOOL bMenu,
		_In_ DWORD dwExStyle,
		_In_ UINT dpi
	) {
		BOOL code{
			::AdjustWindowRectExForDpi(
				lpRect,
				dwStyle,
				bMenu,
				dwExStyle,
				dpi)
		};
		if (code == 0) {
			throw Error{ "Failed to adjust window rectangle for DPI" };
		}
		return code;
	};

	inline BOOL SetWindowPos(
		_In_ HWND hWnd,
		_In_opt_ HWND hWndInsertAfter,
		_In_ int X,
		_In_ int Y,
		_In_ int cx,
		_In_ int cy,
		_In_ UINT uFlags
	) {
		BOOL code{
			::SetWindowPos(
				hWnd,
				hWndInsertAfter,
				X, Y, cx, cy,
				uFlags)
		};
		if (code == 0) {
			throw Error{ "Failed to set window position" };
		}
		return code;
	};

	inline BOOL ReleaseCapture(
	) {
		BOOL code{
			::ReleaseCapture()
		};
		if (code == 0) {
			throw Error{ "Failed to release capture" };
		}
		return code;
	};

	inline HRSRC FindResourceW(
		_In_opt_ HMODULE hModule,
		_In_ LPCWSTR lpName,
		_In_ LPCWSTR lpType
	) {
		HRSRC hRsrc{
			::FindResourceW(
				hModule,
				lpName,
				lpType)
		};
		if (hRsrc == NULL) {
			throw Error{ "Failed to find resource" };
		}
		return hRsrc;
	};

	inline HGLOBAL LoadResource(
		_In_opt_ HMODULE hModule,
		_In_ HRSRC hResInfo
	) {
		HGLOBAL hGlobal{
			::LoadResource(
				hModule,
				hResInfo)
		};
		if (hGlobal == NULL) {
			throw Error{ "Failed to load resource" };
		}
		return hGlobal;
	};

	inline DWORD SizeofResource(
		_In_opt_ HMODULE hModule,
		_In_ HRSRC hResInfo
	) {
		DWORD size{
			::SizeofResource(
				hModule,
				hResInfo)
		};
		if (size == 0) {
			throw Error{ "Failed to fetch resource size" };
		}
		return size;
	};

	inline LPVOID LockResource(
		_In_ HGLOBAL hResData
	) {
		LPVOID pData{
			::LockResource(
				hResData)
		};
		if (pData == NULL) {
			throw Error{ "Failed to lock resource" };
		}
		return pData;
	};

	inline UINT_PTR SetTimer(
		_In_opt_ HWND hWnd,
		_In_ UINT_PTR nIDEvent,
		_In_ UINT uElapse,
		_In_opt_ TIMERPROC lpTimerFunc
	) {
		UINT_PTR id{
			::SetTimer(
				hWnd,
				nIDEvent,
				uElapse,
				lpTimerFunc)
		};
		if (id == 0) {
			throw Error{ "Failed to set timer" };
		}
		return id;
	};

	inline BOOL KillTimer(
		_In_opt_ HWND hWnd,
		_In_ UINT_PTR uIDEvent
	) {
		BOOL code{
			::KillTimer(
				hWnd,
				uIDEvent)
		};
		if (code == 0) {
			throw Error{ "Failed to kill timer" };
		}
		return code;
	};

	inline LSTATUS RegGetValue(
	  _In_ HKEY hkey,
	  _In_opt_ LPCWSTR  lpSubKey,
	  _In_opt_ LPCWSTR  lpValue,
	  _In_opt_ DWORD   dwFlags,
	  _Out_opt_ LPDWORD pdwType,
	  _Out_opt_ PVOID   pvData,
	  _In_out_opt_ LPDWORD pcbData
	) {
		LSTATUS code{};
		for (;;) {
			code = ::RegGetValueW(
				hKey,
				lpSubKey,
				lpValue,
				dwFlags,
				pdwType,
				pvData,
				pcbData);
			switch (code) {
			case ERROR_FILE_NOT_FOUND: {
				if (flag & RRF_SUBKEY_WOW6432KEY) {
					return code;
				}
				flag |= RRF_SUBKEY_WOW6432KEY;
			} break;
			default: {

			} return code;
			}
		}
	};
}

#endif
