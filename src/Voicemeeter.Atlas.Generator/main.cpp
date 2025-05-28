#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include "fstream"

#include "Field/Encoder.h"
#include "Mask/Cherry.h"

using namespace ::Voicemeeter::Atlas::Generator;

int main() {
	constexpr size_t Factor{ 100 };

	Mask::Cherry mask{
		true,
		{ static_cast<double>(Factor), static_cast<double>(Factor) }
	};

	::Microsoft::WRL::ComPtr<IWICBitmapLock> pLock{ nullptr };
	::Windows::ThrowIfFailed(mask.get_pBitmap()
		->Lock(
			nullptr,
			WICBitmapLockRead,
			&pLock
	), "Bitmap lock failed");

	UINT size{ 0U };
	WICInProcPointer pSrc{ nullptr };
	::Windows::ThrowIfFailed(pLock->GetDataPointer(
		&size, &pSrc
	), "Failed to get bitmap data");

	Field::Encoder encoder{
		pSrc,
		mask.get_Height(), mask.get_Width(),
		Factor
	};

	::std::fstream file{
		L"Cherry.bit",
		file.out | file.binary | file.trunc
	};
	if (file.is_open()) {
		file.write(
			reinterpret_cast<const char*>(encoder.get_pDistance()),
			encoder.get_size() * sizeof(float));
	}
}