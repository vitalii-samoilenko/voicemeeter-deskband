#include <fstream>
#include <string>
#include <vector>

constexpr size_t PixelSize{ 3 * sizeof(float) };
constexpr size_t TextWidth{ 90 };
constexpr size_t TextHeight{ 48 };
constexpr size_t Width{ TextWidth * 3 };
constexpr size_t Height{ 52 + TextHeight * 2 + TextHeight * 72 / 3 };
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
		52, 52);
	combine(atlas,
		"plug_frame.bit",
		52, 0,
		43, 21);
	combine(atlas,
		"knob_indicator.bit",
		52, 21,
		30, 30);
	combine(atlas,
		"vban_frame.bit",
		95, 0,
		82, 48);
	combine(atlas,
		"P.bit",
		0 * TextWidth, 52 + 0 * TextHeight,
		TextWidth, TextHeight);
	combine(atlas,
		"V.bit",
		1 * TextWidth, 52 + 0 * TextHeight,
		TextWidth, TextHeight);
	combine(atlas,
		"A1.bit",
		2 * TextWidth, 52 + 0 * TextHeight,
		TextWidth, TextHeight);
	combine(atlas,
		"A2.bit",
		0 * TextWidth, 52 + 1 * TextHeight,
		TextWidth, TextHeight);
	combine(atlas,
		"B1.bit",
		1 * TextWidth, 52 + 1 * TextHeight,
		TextWidth, TextHeight);
	combine(atlas,
		"B2.bit",
		2 * TextWidth, 52 + 1 * TextHeight,
		TextWidth, TextHeight);
	for (size_t i{ 0 }; i < 70 + 1; ++i) {
		::std::string bit{ ::std::to_string(i) + ".bit" };
		combine(atlas,
			bit.c_str(),
			(i % 3) * TextWidth, 52 + ((6 + i) / 3) * TextHeight,
			TextWidth, TextHeight);
	}
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
