#include <fstream>
#include <vector>

int main(int argc, char const *argv[]) {
	size_t blockWidth{ 41 };
	size_t blockHeight{ 22 };
	size_t bidth{ 5 };
	size_t beight{ 2 };
	size_t width{ bidth * blockWidth };
	size_t height{ beight * blockHeight };
	size_t pixelSize{ 3 * 4 };
	size_t rowStride{ pixelSize * width };
	size_t headerSize{ 16 };
	::std::vector<float> atlas(height * rowStride);
	{
		size_t iOffset{ 0 * blockHeight };
		size_t jOffset{ 0 * blockWidth };
		size_t frameWidth{ 40 };
		size_t frameHeight{ 40 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"knob_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		bit.seekg(headerSize);
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		size_t iOffset{ 0 * blockHeight };
		size_t jOffset{ 1 * blockWidth };
		size_t frameWidth{ 41 };
		size_t frameHeight{ 19 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"plug_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		bit.seekg(headerSize);
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		size_t iOffset{ 1 * blockHeight };
		size_t jOffset{ 1 * blockWidth };
		size_t frameWidth{ 19 };
		size_t frameHeight{ 19 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"knob_indicator.bit",
			::std::ios::binary | ::std::ios::in
		};
		bit.seekg(headerSize);
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			bit.read(
				reinterpret_cast<char *>(
					&atlas[0])
				+ rowStride * (iOffset + i) + pixelSize * jOffset,
				frameRowStride);
		}
	}
	{
		size_t iOffset{ 0 * blockHeight };
		size_t jOffset{ 2 * blockWidth };
		size_t frameWidth{ 78 };
		size_t frameHeight{ 44 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream bit{
			"vban_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		bit.seekg(headerSize);
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
