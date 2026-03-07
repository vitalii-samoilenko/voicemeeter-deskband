#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "msdfgen.h"

constexpr size_t ChannelsSize{ 3 };
constexpr size_t PixelSize{ ChannelsSize * sizeof(float) };
constexpr size_t TextJOffset{ 3 };
constexpr size_t TextIOffset{ 3 };
constexpr size_t TextWidth{ 90 };
constexpr size_t TextHeight{ 48 };
constexpr size_t Width{ TextWidth * ChannelsSize };
constexpr size_t Height{ 52 + TextHeight * 2 + TextHeight * 72 / 3 };
constexpr size_t RowStride{ PixelSize * Width };
constexpr size_t Size{ Height * RowStride };
constexpr size_t Range{ 4 };
constexpr double Angle{ 3. };
constexpr double Scale{ 1. };

::msdfgen::Bitmap<float, ChannelsSize> generate(
	char const *path,
	size_t jOffset, size_t iOffset
	size_t frameWidth, size_t frameHeight) {
	::msdfgen::Shape shape{};
	{
		::std::fstream desc{
			path,
			::std::ios::in
		};
		::std::stringstream temp{};
		for (::std::string line{}; ::std::getline(desc, line);) {
			temp << line << ::std::endl;
		}
		::msdfgen::readShapeDescription(temp.str().c_str(), shape);
	}
	shape.normalize();
	::msdfgen::edgeColoringByDistance(shape, Angle);
	::msdfgen::Bitmap<float, ChannelsSize> msdf{ frameWidth, frameHeight };
	::msdfgen::SDFTransformation transform{
		::msdfgen::Projection{
			Scale,
			::msdfgen::Vector2{
				jOffset,
				iOffset
			}
		},
		Range
	};
	::msdfgen::generateMSDF(msdf, shape, transform);
	return msdf;
};
void combine(::std::vector<float> &atlas,
	::msdfgen::Bitmap<float, ChannelsSize> const &msdf,
	size_t jOffset, size_t iOffset) {
	size_t frameRowStride{ PixelSize * msdf.width() };
	for (size_t i{ 0 }; i < msdf.height(); ++i) {
		::std::memcpy(reinterpret_cast<char *>(&atlas[0])
				+ RowStride * (iOffset + i) + PixelSize * jOffset,
			msdf(0, i),
			frameRowStride);
	}
};

int main(int argc, char const *argv[]) {
	::std::string baseDir{ argv[1] };
	::std::vector<float> atlas(Size);
	combine(atlas,
		generate(
			(baseDir + "/knob_frame.desc").c_str(),
			2, 2,
			52, 52),
		0, 0);
	combine(atlas,
		generate(
			(baseDir + "/plug_frame.desc").c_str(),
			1, 1,
			43, 21),
		52, 0);
	combine(atlas,
		generate(
			(baseDir + "/knob_indicator.desc").c_str(),
			4, 4,
			30, 30),
		52, 21);
	combine(atlas,
		generate(
			(baseDir + "/vban_frame.desc").c_str(),
			2, 2,
			82, 48),
		95, 0);
	combine(atlas,
		generate(
			"P.desc",
			textjoffset, textioffset,
			textwidth, textheight),
		0 * TextWidth, 52 + 0 * TextHeight);
	combine(atlas,
		generate(
			"V.desc",
			textjoffset, textioffset,
			textwidth, textheight),
		1 * TextWidth, 52 + 0 * TextHeight);
	combine(atlas,
		generate(
			"A1.desc",
			textjoffset, textioffset,
			textwidth, textheight),
		2 * TextWidth, 52 + 0 * TextHeight);
	combine(atlas,
		generate(
			"A2.desc",
			textjoffset, textioffset,
			textwidth, textheight),
		0 * TextWidth, 52 + 1 * TextHeight);
	combine(atlas,
		generate(
			"B1.desc",
			textjoffset, textioffset,
			textwidth, textheight),
		1 * TextWidth, 52 + 1 * TextHeight);
	combine(atlas,
		generate(
			"B2.desc",
			textjoffset, textioffset,
			textwidth, textheight),
		2 * TextWidth, 52 + 1 * TextHeight);
	for (size_t i{ 0 }; i < 70 + 1; ++i) {
		::std::string desc{ ::std::to_string(i) + ".desc" };
		combine(atlas,
			generate(
				desc.c_str(),
				textjoffset, textioffset,
				textwidth, textheight),
			(i % 3) * TextWidth, 52 + ((6 + i) / 3) * TextHeight);
	}
	{
		::std::fstream bit{
			"Atlas.bit",
			::std::ios::binary | ::std::ios::out
		};
		bit.write(
			reinterpret_cast<char *>(
				&atlas[0]),
			Size);
	}
	return 0;
};
