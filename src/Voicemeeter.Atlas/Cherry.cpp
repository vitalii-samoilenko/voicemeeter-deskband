#include "Windows/Wrappers.h"

#include "Cherry.h"
#include "Fields.h"

using namespace ::Voicemeeter::Atlas;

Cherry::Cherry(
	HMODULE hModule
) : m_pField{ ::Windows::wLockResource(
		::Windows::wLoadResource(
			hModule,
			::Windows::wFindResourceW(
				hModule,
				MAKEINTRESOURCE(IDR_CHERRY), L"Field"))) } {

}