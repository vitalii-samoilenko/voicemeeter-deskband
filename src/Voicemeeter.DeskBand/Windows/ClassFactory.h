#pragma once

#include <unknwn.h> // for IClassFactory
#include <windows.h>

namespace Voicemeeter {
    namespace Windows {
        class ClassFactory : public IClassFactory {
        public:
            // IUnknown
            STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
            STDMETHODIMP_(ULONG) AddRef();
            STDMETHODIMP_(ULONG) Release();

            // IClassFactory
            STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv);
            STDMETHODIMP LockServer(BOOL fLock);

            ClassFactory();

        protected:
            ~ClassFactory();

        private:
            LONG m_cRef;
        };
    }
}