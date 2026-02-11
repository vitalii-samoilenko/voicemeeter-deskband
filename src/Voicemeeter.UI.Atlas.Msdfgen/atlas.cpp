#include <fstream>
#include <vector>

int main(int argc, char const *argv[]) {
	size_t blockWidth{ 32 };
	size_t blockHeight{ 16 };
	size_t bidth{ 3 };
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
		size_t frameWidth{ 32 };
		size_t frameHeight{ 32 };
		size_t frameRowStride{ pixelSize * frameWidth };
		::std::fstream knob_frame{
			"knob_frame.bit",
			::std::ios::binary | ::std::ios::in
		};
		knob_frame.seekg(headerSize);
		for (size_t i{ 0 }; i < frameHeight; ++i) {
			knob_frame.read(
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
