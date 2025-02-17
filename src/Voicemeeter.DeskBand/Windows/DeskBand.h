#pragma once

#include <memory>
#include <unordered_map>

#include <shlobj.h> // for IDeskband2, IObjectWithSite, IPesistStream, and IInputObject
#include <windows.h>

#include "Environment/IInputTracker.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"
#include "Voicemeeter.Clients.Remote/Cherry.h"
#include "Voicemeeter.Clients.UI.D2D/Cherry.h"
#include "Voicemeeter.Clients.UI.D3D12/Cherry.h"
#include "Windows/Timer.h"

namespace Voicemeeter {
	namespace Windows {
		class DeskBand final
			: public ::Environment::IInputTracker
			, public IDeskBand2
			, public IPersistStream
			, public IObjectWithSite
			, public IInputObject {
		public:
			DeskBand();
			DeskBand(const DeskBand&) = delete;
			DeskBand(DeskBand&&) = delete;

			DeskBand& operator=(const DeskBand&) = delete;
			DeskBand& operator=(DeskBand&&) = delete;

			virtual void EnableInputTrack() override;
			virtual void DisableInputTrack() override;

			// IUnknown
			STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
			STDMETHODIMP_(ULONG) AddRef();
			STDMETHODIMP_(ULONG) Release();

			// IOleWindow
			STDMETHODIMP GetWindow(HWND* phwnd);
			STDMETHODIMP ContextSensitiveHelp(BOOL);

			// IDockingWindow
			STDMETHODIMP ShowDW(BOOL fShow);
			STDMETHODIMP CloseDW(DWORD);
			STDMETHODIMP ResizeBorderDW(const RECT*, IUnknown*, BOOL);

			// IDeskBand (needed for all deskbands)
			STDMETHODIMP GetBandInfo(DWORD dwBandID, DWORD, DESKBANDINFO* pdbi);

			// IDeskBand2 (needed for glass deskband)
			STDMETHODIMP CanRenderComposited(BOOL* pfCanRenderComposited);
			STDMETHODIMP SetCompositionState(BOOL fCompositionEnabled);
			STDMETHODIMP GetCompositionState(BOOL* pfCompositionEnabled);

			// IPersist
			STDMETHODIMP GetClassID(CLSID* pclsid);

			// IPersistStream
			STDMETHODIMP IsDirty();
			STDMETHODIMP Load(IStream* pStm);
			STDMETHODIMP Save(IStream* pStm, BOOL fClearDirty);
			STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pcbSize);

			// IObjectWithSite
			STDMETHODIMP SetSite(IUnknown* pUnkSite);
			STDMETHODIMP GetSite(REFIID riid, void** ppv);

			// IInputObject
			STDMETHODIMP UIActivateIO(BOOL fActivate, MSG*);
			STDMETHODIMP HasFocusIO();
			STDMETHODIMP TranslateAcceleratorIO(MSG*);

		private:
			LONG                m_cRef;                 // ref count of deskband
			IUnknown*           m_pSite;                // IObjectWithSite site
			IInputObjectSite*   m_pInputObjectSite;     // parent site that contains deskband
			BOOL                m_fHasFocus;            // whether deskband window currently has focus
			BOOL                m_fIsDirty;             // whether deskband setting has changed
			BOOL                m_fCompositionEnabled;  // whether glass is currently enabled in deskband
			DWORD               m_dwBandID;             // ID of deskband
			HWND                m_hWnd;                 // main window of deskband
			HWND                m_hWndParent;           // parent window of deskband
			RECT m_rc;
			::std::unique_ptr<::Windows::Timer> m_pCompositionTimer;
			::std::unique_ptr<::Windows::Timer> m_pRenderTimer;
			::std::unique_ptr<::Windows::Timer> m_pRemoteTimer;
			::std::unordered_map<UINT_PTR, ::Windows::Timer*> m_lpTimer;
			::std::unique_ptr<::Voicemeeter::Adapters::Multiclient::Cherry> m_pMixer;
			::std::unique_ptr<::Voicemeeter::Clients::Remote::Cherry> m_pRemote;
			::Voicemeeter::UI::Scene* m_pScene;
			::std::unique_ptr<::Voicemeeter::UI::D2D::Scene> m_pD2dScene;
			::std::unique_ptr<::Voicemeeter::UI::D3D12::Scene> m_pD3d12Scene;

			~DeskBand();

			static LRESULT CALLBACK WndProcW(
				HWND hWnd,
				UINT uMsg,
				WPARAM wParam,
				LPARAM lParam
			);
		};
	}
}