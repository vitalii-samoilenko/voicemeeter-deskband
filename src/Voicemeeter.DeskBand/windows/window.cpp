#include "window.h"

#include "wrappers.h"
#include "../errormessagebox.h"

using namespace Voicemeeter::DeskBand::Windows;

static const LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter.DeskBand" };
static const LRESULT OK{ 0 };

Window::Window(HINSTANCE hInstance, Presentation::Scene& scene)
	: m_hWnd{ NULL }
	, m_scene { scene }
	, m_pCompTarget{ nullptr } {
	WNDCLASSW wndClass{
		0U,								//.style
		WindowProcW,					//.lpfnWndProc
		0, 0,							//.cbClsExtra, .cbWndExtra
		hInstance,						//.hInstance
		NULL,							//.hIcon
		wLoadCursorW(NULL, IDC_ARROW),	//.hCursor
		NULL,							//.hbrBackground
		NULL,							//.lpszMenuName
		LPSZ_CLASS_NAME					//.lpszClassName
	};
	wRegisterClassW(&wndClass);
	wCreateWindowExW(
		WS_EX_NOREDIRECTIONBITMAP,
		LPSZ_CLASS_NAME,
		NULL,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		this
	);

	ComPtr<IDCompositionDevice> pCompDevice;
	ThrowIfFailed(DCompositionCreateDevice3(
		NULL,
		IID_PPV_ARGS(&pCompDevice)
	), "Composition device creation failed");
	ThrowIfFailed(pCompDevice->CreateTargetForHwnd(
		m_hWnd,
		TRUE,
		&m_pCompTarget
	), "Composition target creation failed");
	ComPtr<IDCompositionVisual> pCompVisual;
	ThrowIfFailed(pCompDevice->CreateVisual(
		&pCompVisual
	), "Composition visual creation failed");

	m_scene.Initialize(m_hWnd, pCompVisual.Get());

	ThrowIfFailed(m_pCompTarget->SetRoot(
		pCompVisual.Get()
	), "Failed to set composition target root");
	ThrowIfFailed(pCompDevice->Commit(
	), "Failed to commit composition device");
}

void Window::Show(int nCmdShow) const {
	ShowWindow(
		m_hWnd,
		nCmdShow
	);
}

LRESULT CALLBACK Window::WindowProcW(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	auto shutdown = [](DWORD errCode)->LRESULT {
		ErrorMessageBox(errCode);
		PostQuitMessage(0);

		return OK;
	};
	try{
		Window* pWnd{ wGetWindowLongPtrW<Window>(hWnd, GWLP_USERDATA) };

		switch (uMsg) {
		case WM_NCCREATE:
			pWnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_hWnd = hWnd;

			if (wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd))) {
				return FALSE;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);

			return OK;
		case WM_SIZE: {
			pWnd->m_scene.Resize(LOWORD(lParam), HIWORD(lParam));

			return OK;
		}
		}
	}
	catch (const windows_error& e) {
		return shutdown(e.code());
	}
	catch (const com_error& e) {
		return shutdown(e.code());
	}
	catch (...) {
		return shutdown(MSG_ERR_GENERAL);
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}