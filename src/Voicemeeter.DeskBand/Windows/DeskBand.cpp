#include <cmath>
#include <fstream>
#include <string>

#include "Windows/ErrorMessageBox.h"
#include "Windows/Wrappers.h"

#include "DeskBand.h"

#define RECTWIDTH(x)   ((x).right - (x).left)
#define RECTHEIGHT(x)  ((x).bottom - (x).top)

using namespace ::Voicemeeter::Windows;

extern long         g_cDllRef;
extern HINSTANCE    g_hInst;

extern CLSID CLSID_DeskBand;

static bool g_shutdown{ false };

static constexpr LPCWSTR LPSZ_CLASS_NAME{ L"Voicemeeter.DeskBand" };
static constexpr DWORD STYLE{ WS_OVERLAPPEDWINDOW };
static constexpr UINT WM_DIRTY{ WM_USER + 0U };

static constexpr LRESULT OK{ 0 };

DeskBand::DeskBand(
) : m_cRef{ 1 }
  , m_pSite{ NULL }
  , m_pInputObjectSite{ NULL }
  , m_fHasFocus{ FALSE }
  , m_fIsDirty{ FALSE }
  , m_fCompositionEnabled{ FALSE }
  , m_dwBandID{ 0 }
  ,	m_hWnd{ NULL }
  , m_hWndParent{ NULL }
  , m_pCompositionTimer{ nullptr }
  , m_pDirtyTimer{ nullptr }
  , m_pRemoteTimer{ nullptr }
  , m_lpTimer{}
  , m_pMixer{ nullptr }
  , m_pRemote{ nullptr }
  , m_pScene{ nullptr } {
	InterlockedIncrement(&g_cDllRef);
}

DeskBand::~DeskBand() {
	if (m_pSite) {
		m_pSite->Release();
	}
	if (m_pInputObjectSite) {
		m_pInputObjectSite->Release();
	}
	InterlockedDecrement(&g_cDllRef);
}

void DeskBand::SetDirty() {
	::Windows::wPostMessageW(m_hWnd, WM_DIRTY, 0, 0);
}
void DeskBand::EnableInputTrack() {
	SetCapture(m_hWnd);
}
void DeskBand::DisableInputTrack() {
	::Windows::wReleaseCapture();
}

//
// IUnknown
//
STDMETHODIMP DeskBand::QueryInterface(REFIID riid, void** ppv) {
	HRESULT hr = S_OK;

	if (IsEqualIID(IID_IUnknown, riid)
		|| IsEqualIID(IID_IOleWindow, riid)
		|| IsEqualIID(IID_IDockingWindow, riid)
		|| IsEqualIID(IID_IDeskBand, riid)
		|| IsEqualIID(IID_IDeskBand2, riid)) {
		*ppv = static_cast<IOleWindow*>(this);
	} else if (IsEqualIID(IID_IPersist, riid)
		|| IsEqualIID(IID_IPersistStream, riid)) {
		*ppv = static_cast<IPersist*>(this);
	} else if (IsEqualIID(IID_IObjectWithSite, riid)) {
		*ppv = static_cast<IObjectWithSite*>(this);
	} else if (IsEqualIID(IID_IInputObject, riid)) {
		*ppv = static_cast<IInputObject*>(this);
	} else {
		hr = E_NOINTERFACE;
		*ppv = NULL;
	}

	if (*ppv) {
		AddRef();
	}

	return hr;
}

STDMETHODIMP_(ULONG) DeskBand::AddRef() {
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) DeskBand::Release() {
	ULONG cRef = InterlockedDecrement(&m_cRef);
	if (0 == cRef) {
		delete this;
	}

	return cRef;
}

//
// IOleWindow
//
STDMETHODIMP DeskBand::GetWindow(HWND* phwnd) {
	*phwnd = m_hWnd;

	return S_OK;
}

STDMETHODIMP DeskBand::ContextSensitiveHelp(BOOL) {
	return E_NOTIMPL;
}

//
// IDockingWindow
//
STDMETHODIMP DeskBand::ShowDW(BOOL fShow) {
	if (m_hWnd) {
		ShowWindow(m_hWnd, fShow ? SW_SHOW : SW_HIDE);
	}

	return S_OK;
}

STDMETHODIMP DeskBand::CloseDW(DWORD) {
	if (m_hWnd) {
		ShowWindow(m_hWnd, SW_HIDE);
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	return S_OK;
}

STDMETHODIMP DeskBand::ResizeBorderDW(const RECT*, IUnknown*, BOOL) {
	return E_NOTIMPL;
}

//
// IDeskBand
//
STDMETHODIMP DeskBand::GetBandInfo(DWORD dwBandID, DWORD, DESKBANDINFO* pdbi) {
	if (!pdbi) {
		return E_INVALIDARG;
	}

	m_dwBandID = dwBandID;

	if (pdbi->dwMask & DBIM_MINSIZE) {
		switch (m_pRemote->get_Type()) {
		case ::Voicemeeter::Clients::Remote::Type::Voicemeeter: {
			pdbi->ptMinSize.x = 189;
			pdbi->ptMinSize.y = 30;
		} break;
		case ::Voicemeeter::Clients::Remote::Type::Banana: {
			pdbi->ptMinSize.x = 370;
			pdbi->ptMinSize.y = 30;
		} break;
		case ::Voicemeeter::Clients::Remote::Type::Potato: {
			pdbi->ptMinSize.x = 370;
			pdbi->ptMinSize.y = 30;
		} break;
		}
	}

	if (pdbi->dwMask & DBIM_MAXSIZE) {
		pdbi->ptMaxSize.y = -1;
	}

	if (pdbi->dwMask & DBIM_INTEGRAL) {
		pdbi->ptIntegral.y = 1;
	}

	if (pdbi->dwMask & DBIM_ACTUAL) {
		const ::std::valarray<double>& vertex{ m_pScene->get_Size() };

		pdbi->ptActual.x = static_cast<LONG>(::std::ceil(vertex[0]));
		pdbi->ptActual.y = static_cast<LONG>(::std::ceil(vertex[1]));
	}

	if (pdbi->dwMask & DBIM_TITLE) {
		// Don't show title by removing this flag.
		pdbi->dwMask &= ~DBIM_TITLE;
	}

	if (pdbi->dwMask & DBIM_MODEFLAGS) {
		pdbi->dwModeFlags = DBIMF_NORMAL | DBIMF_VARIABLEHEIGHT;
	}

	if (pdbi->dwMask & DBIM_BKCOLOR) {
		// Use the default background color by removing this flag.
		pdbi->dwMask &= ~DBIM_BKCOLOR;
	}

	return S_OK;
}

//
// IDeskBand2
//
STDMETHODIMP DeskBand::CanRenderComposited(BOOL* pfCanRenderComposited) {
	*pfCanRenderComposited = TRUE;

	return S_OK;
}

STDMETHODIMP DeskBand::SetCompositionState(BOOL fCompositionEnabled) {
	m_fCompositionEnabled = fCompositionEnabled;

	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);

	return S_OK;
}

STDMETHODIMP DeskBand::GetCompositionState(BOOL* pfCompositionEnabled) {
	*pfCompositionEnabled = m_fCompositionEnabled;

	return S_OK;
}

//
// IPersist
//
STDMETHODIMP DeskBand::GetClassID(CLSID* pclsid) {
	*pclsid = CLSID_DeskBand;

	return S_OK;
}

//
// IPersistStream
//
STDMETHODIMP DeskBand::IsDirty() {
	return m_fIsDirty
		? S_OK
		: S_FALSE;
}

STDMETHODIMP DeskBand::Load(IStream* /*pStm*/) {
	return S_OK;
}

STDMETHODIMP DeskBand::Save(IStream* /*pStm*/, BOOL fClearDirty) {
	if (fClearDirty) {
		m_fIsDirty = FALSE;
	}

	return S_OK;
}

STDMETHODIMP DeskBand::GetSizeMax(ULARGE_INTEGER* /*pcbSize*/) {
	return E_NOTIMPL;
}

//
// IObjectWithSite
//
STDMETHODIMP DeskBand::SetSite(IUnknown* pUnkSite) {
	HRESULT hr = S_OK;

	m_hWndParent = NULL;

	if (m_pSite) {
		m_pSite->Release();
		m_pSite = NULL;
	}
	if (m_pInputObjectSite) {
		m_pInputObjectSite->Release();
		m_pInputObjectSite = NULL;
	}

	if (pUnkSite) {
		m_pSite = pUnkSite;
		m_pSite->AddRef();

		IOleWindow* pow;
		hr = pUnkSite->QueryInterface(IID_IOleWindow, reinterpret_cast<void**>(&pow));
		if (SUCCEEDED(hr)) {
			hr = pow->GetWindow(&m_hWndParent);
			if (SUCCEEDED(hr)) {
				WNDCLASSW wc = { 0 };
				wc.style = CS_DBLCLKS;
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				wc.hInstance = g_hInst;
				wc.lpfnWndProc = WndProcW;
				wc.lpszClassName = LPSZ_CLASS_NAME;

				RegisterClassW(&wc);

				g_shutdown = false;

				if (CreateWindowExW(
						WS_EX_NOREDIRECTIONBITMAP,
						LPSZ_CLASS_NAME,
						NULL,
						WS_CHILD,
						0,
						0,
						0,
						0,
						m_hWndParent,
						NULL,
						g_hInst,
						this) == NULL) {
					hr = E_FAIL;
				}
			}

			pow->Release();
		}

		if (SUCCEEDED(hr)) {
			pUnkSite->QueryInterface(IID_PPV_ARGS(&m_pInputObjectSite));
		}
	}

	return hr;
}

STDMETHODIMP DeskBand::GetSite(REFIID riid, void** ppv) {
	HRESULT hr = E_FAIL;

	if (m_pSite) {
		hr = m_pSite->QueryInterface(riid, ppv);
	} else {
		*ppv = NULL;
	}

	return hr;
}

//
// IInputObject
//
STDMETHODIMP DeskBand::UIActivateIO(BOOL fActivate, MSG*) {
	if (fActivate) {
		SetFocus(m_hWnd);
	}

	return S_OK;
}

STDMETHODIMP DeskBand::HasFocusIO() {
	return m_fHasFocus ? S_OK : S_FALSE;
}

STDMETHODIMP DeskBand::TranslateAcceleratorIO(MSG*) {
	return S_FALSE;
};

LRESULT CALLBACK DeskBand::WndProcW(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	if (g_shutdown) {
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	auto shutdown = [uMsg](long long errCode)->LRESULT {
		::Windows::ErrorMessageBox(g_hInst, errCode);

		if (uMsg == WM_NCCREATE) {
			return FALSE;
		}
		g_shutdown = true;

		return OK;
	};
	auto log = [](const char* message)->void {
		WCHAR temp[MAX_PATH];
		if (GetTempPathW(MAX_PATH, temp)) {
			::std::wstring path{ temp };
			::std::fstream log{ path.append(L"Voicemeeter.DeskBand.log"), log.out | log.app };
			if (log.is_open()) {
				log << message << ::std::endl;
			}
		}
	};
	try {
		DeskBand* pWnd{ ::Windows::wGetWindowLongPtrW<DeskBand>(hWnd, GWLP_USERDATA) };
		switch (uMsg) {
		case WM_NCCREATE: {
			pWnd = reinterpret_cast<DeskBand*>(reinterpret_cast<LPCREATESTRUCTW>(lParam)->lpCreateParams);
			pWnd->m_hWnd = hWnd;
			pWnd->m_pCompositionTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pDirtyTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_pRemoteTimer.reset(new ::Windows::Timer{ hWnd });
			pWnd->m_lpTimer.emplace(pWnd->m_pCompositionTimer->get_Id(), pWnd->m_pCompositionTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pDirtyTimer->get_Id(), pWnd->m_pDirtyTimer.get());
			pWnd->m_lpTimer.emplace(pWnd->m_pRemoteTimer->get_Id(), pWnd->m_pRemoteTimer.get());
			pWnd->m_pMixer.reset(new ::Voicemeeter::Adapters::Multiclient::Cherry{});
			pWnd->m_pRemote.reset(new ::Voicemeeter::Clients::Remote::Cherry{ *pWnd->m_pRemoteTimer, *pWnd->m_pMixer });
			::Voicemeeter::Clients::UI::D2D::Cherry builder{
				hWnd,
				*pWnd, *pWnd,
				*pWnd->m_pCompositionTimer, *pWnd->m_pDirtyTimer,
				*pWnd->m_pMixer
			};
			if (pWnd->m_pRemote->get_Type() == ::Voicemeeter::Clients::Remote::Type::Voicemeeter) {
				builder
					.WithNetwork(false)
					.WithIgnoredStrip(3)
					.WithIgnoredStrip(5);
			}
			RECT taskbar{};
			if (GetClientRect(pWnd->m_hWndParent, &taskbar) && taskbar.right < taskbar.bottom) {
				builder.WithDirection(::Voicemeeter::UI::Direction::Down);
			}
			pWnd->m_pScene = builder.Build();
			::Windows::wSetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		} break;
		case WM_SETFOCUS: {
			pWnd->m_fHasFocus = TRUE;
			if (pWnd->m_pInputObjectSite) {
				pWnd->m_pInputObjectSite->OnFocusChangeIS(static_cast<IOleWindow*>(pWnd), pWnd->m_fHasFocus);
			}
		} break;
		case WM_KILLFOCUS: {
			pWnd->m_fHasFocus = FALSE;
			if (pWnd->m_pInputObjectSite) {
				pWnd->m_pInputObjectSite->OnFocusChangeIS(static_cast<IOleWindow*>(pWnd), pWnd->m_fHasFocus);
			}
		} break;
		case WM_CONTEXTMENU: {

		} return OK;
		case WM_TIMER: {
			pWnd->m_lpTimer[static_cast<UINT_PTR>(wParam)]
				->Elapse();
		} return OK;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = ::Windows::wBeginPaint(hWnd, &ps);
			pWnd->m_pScene->Redraw(
				::std::valarray<double>{
					static_cast<double>(ps.rcPaint.left),
					static_cast<double>(ps.rcPaint.top)
				},
				(ps.rcPaint.right && ps.rcPaint.bottom
					? ::std::valarray<double>{
						static_cast<double>(ps.rcPaint.right),
						static_cast<double>(ps.rcPaint.bottom)
					} : pWnd->m_pScene->get_Size())
			);
			EndPaint(hWnd, &ps);
		} return OK;
		case WM_PRINTCLIENT: {
			pWnd->m_pScene->Redraw(
				pWnd->m_pScene->get_Position(),
				pWnd->m_pScene->get_Size()
			);
		} return OK;
		case WM_DIRTY: {
			pWnd->m_pScene->Redraw();
		} return OK;
		case WM_SIZE: {
			pWnd->m_pScene->Resize({
				static_cast<double>(LOWORD(lParam)),
				static_cast<double>(HIWORD(lParam))
			});
		} return OK;
		case WM_LBUTTONDOWN: {
			pWnd->m_pScene->MouseLDown({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_LBUTTONDBLCLK: {
			pWnd->m_pScene->MouseLDouble({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_MBUTTONDOWN: {
			pWnd->m_pScene->MouseMDown({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_MBUTTONDBLCLK: {
			pWnd->m_pScene->MouseMDouble({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_RBUTTONDOWN: {
			pWnd->m_pScene->MouseRDown({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} break;
		case WM_RBUTTONDBLCLK: {
			pWnd->m_pScene->MouseRDouble({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_MOUSEWHEEL: {
			POINT point{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::Windows::wScreenToClient(hWnd, &point);

			pWnd->m_pScene->MouseWheel({
				static_cast<double>(point.x),
				static_cast<double>(point.y)
			}, GET_WHEEL_DELTA_WPARAM(wParam));
		} return OK;
		case WM_MOUSEMOVE: {
			pWnd->m_pScene->MouseMove({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_LBUTTONUP: {
			pWnd->m_pScene->MouseLUp({
				static_cast<double>(GET_X_LPARAM(lParam)),
				static_cast<double>(GET_Y_LPARAM(lParam))
			});
		} return OK;
		case WM_ERASEBKGND: {
			if (pWnd->m_fCompositionEnabled) {
				::Windows::wDrawThemeParentBackground(hWnd, reinterpret_cast<HDC>(wParam), NULL);

				return 1;
			}
		} break;
		}
	}
	catch (const ::Windows::Error& e) {
		log(e.what());

		return shutdown(e.code());
	}
	catch (const ::std::exception& e) {
		log(e.what());

		return shutdown(MSG_ERR_GENERAL);
	}
	catch (...) {
		return shutdown(MSG_ERR_GENERAL);
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}