#include <fstream>
#include <vector>

constexpr size_t PixelSize{ 3 * sizeof(float) };
//constexpr size_t Width{ 173 };
constexpr size_t Width{ 198 };
//constexpr size_t Height{ 50 };
constexpr size_t Height{ 118 };
constexpr size_t RowStride{ PixelSize * Width };
constexpr size_t Size{ Height * RowStride };

void combine(::std::vector<float> &atlas,
	char const *path,
	size_t jOffset, size_t iOffset,
	size_t frameWidth, size_t frameHeight) {
	size_t frameRowStride{ PixelSize * frameWidth };
	::std::fstream bit{
		path,
		::std::ios::binary | ::std::ios::in
	};
	for (size_t i{ 0 }; i < frameHeight; ++i) {
		bit.read(
			reinterpret_cast<char *>(
				&atlas[0])
			+ RowStride * (iOffset + i) + PixelSize * jOffset,
			frameRowStride);
	}
};

int main(int argc, char const *argv[]) {
	::std::vector<float> atlas(Size);
	combine(atlas,
		"knob_frame.bit",
		0, 0,
		50, 50);
	combine(atlas,
		"plug_frame.bit",
		50, 0,
		43, 21);
	combine(atlas,
		"knob_indicator.bit",
		50, 22,
		24, 24);
	combine(atlas,
		"vban_frame.bit",
		93, 0,
		80, 46);
	combine(atlas,
		"P.bit",
		0 * 66, 50 + 0 * 34,
		66, 34);
	combine(atlas,
		"V.bit",
		1 * 66, 50 + 0 * 34,
		66, 34);
	combine(atlas,
		"A1.bit",
		2 * 66, 50 + 0 * 34,
		66, 34);
	combine(atlas,
		"A2.bit",
		0 * 66, 50 + 1 * 34,
		66, 34);
	combine(atlas,
		"B1.bit",
		1 * 66, 50 + 1 * 34,
		66, 34);
	combine(atlas,
		"B2.bit",
		2 * 66, 50 + 1 * 34,
		66, 34);
	{
		::std::fstream out{
			"Atlas.bit",
			::std::ios::binary | ::std::ios::out
		};
		out.write(
			reinterpret_cast<char *>(
				&atlas[0]),
			Size);
	}
	return 0;
};
