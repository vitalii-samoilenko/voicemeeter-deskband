#include "Deskband.h"

#include "ClassFactory.h"

using namespace Voicemeeter::Windows;

extern long g_cDllRef;

ClassFactory::ClassFactory()
{
    m_cRef = 1;
    InterlockedIncrement(&g_cDllRef);
}

ClassFactory::~ClassFactory()
{
    InterlockedDecrement(&g_cDllRef);
}

//
// IUnknown
//
STDMETHODIMP ClassFactory::QueryInterface(REFIID riid, void** ppv)
{
    HRESULT hr = S_OK;

    if (IsEqualIID(IID_IUnknown, riid) || IsEqualIID(IID_IClassFactory, riid))
    {
        *ppv = static_cast<IUnknown*>(this);
        AddRef();
    }
    else
    {
        hr = E_NOINTERFACE;
        *ppv = NULL;
    }

    return hr;
}

STDMETHODIMP_(ULONG) ClassFactory::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) ClassFactory::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}

//
// IClassFactory
//
STDMETHODIMP ClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    HRESULT hr = CLASS_E_NOAGGREGATION;

    if (!pUnkOuter)
    {
        hr = E_OUTOFMEMORY;

        DeskBand* pDeskBand = new DeskBand();
        if (pDeskBand)
        {
            hr = pDeskBand->QueryInterface(riid, ppv);
            pDeskBand->Release();
        }
    }

    return hr;
}

STDMETHODIMP ClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        InterlockedIncrement(&g_cDllRef);
    }
    else
    {
        InterlockedDecrement(&g_cDllRef);
    }
    return S_OK;
}
