#include <fstream>
#include <vector>

int main(int argc, char const *argv[]) {
	size_t width{ 173 };
	size_t height{ 50 };
	size_t pixelSize{ 3 * 4 };
	size_t rowStride{ pixelSize * width };
	::std::vector<float> atlas(height * rowStride);
	{
		size_t jOffset{ 0 };
		size_t iOffset{ 0 };
		size_t frameWidth{ 50 };
		size_t frameHeight{ 50 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"knob_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		size_t jOffset{ 50 };
		size_t iOffset{ 0 };
		size_t frameWidth{ 43 };
		size_t frameHeight{ 21 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"plug_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		size_t jOffset{ 50 };
		size_t iOffset{ 22 };
		size_t frameWidth{ 24 };
		size_t frameHeight{ 24 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"knob_indicator.bit",
			::std::ios::binary | ::std::ios::in
		};
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		size_t jOffset{ 93 };
		size_t iOffset{ 0 };
		size_t frameWidth{ 80 };
		size_t frameHeight{ 46 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"vban_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		::std::fstream out{
			"Atlas.bit",
			::std::ios::binary | ::std::ios::out
		};
		out.write(
			reinterpret_cast<char *>(
				&atlas[0]),
			height * rowStride);
	}
	return 0;
};
