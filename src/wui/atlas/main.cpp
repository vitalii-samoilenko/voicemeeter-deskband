#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "msdfgen.h"

namespace Atlas {
	namespace Offset {
		constexpr size_t Width{ 1 };
		constexpr size_t Height{ 1 };
	}
	namespace Pixel {
		constexpr size_t Range{ 4 };
		constexpr size_t Channels{ 3 };
		constexpr size_t Size{ Channels * sizeof(float) };
	}
	namespace Knob {
		namespace Frame {
			constexpr size_t X{ 0 };
			constexpr size_t Y{ 0 };
			constexpr size_t Scale{ 2 }; // 48 / 40
			constexpr size_t Width{ 48 };
			constexpr size_t Height{ 48 };
			namespace Offset {
				constexpr size_t Width{ Scale * Atlas::Offset::Width };
				constexpr size_t Height{ Scale * Atlas::Offset::Height };
			}
		}
	}
	namespace Plug {
		namespace Frame {
			constexpr size_t X{
				Knob::Frame::X
				+ Knob::Frame::Offset::Width
				+ Knob::Frame::Width
				+ Knob::Frame::Offset::Width
			};
			constexpr size_t Y{ Knob::Frame::Y };
			constexpr size_t Scale{ 1 };
			constexpr size_t Width{ Scale * 41 };
			constexpr size_t Height{ Sclae * 19 };
			namespace Offset {
				constexpr size_t Width{ Scale * Atlas::Offset::Width };
				constexpr size_t Height{ Scale * Atlas::Offset::Height };
			}
		}
	}
	namespace Knob {
		namespace Indicator {
			constexpr size_t X{ Plug::Frame::X };
			constexpr size_t Y{
				Plug::Frame::Y
				+ Plug::Frame::Offset::Height
				+ Plug::Frame::Height
				+ Plug::Frame::Offset::Height
			};
			constexpr size_t Scale{ 4 };
			constexpr size_t Width{ Scale * 11 / 2 };
			constexpr size_t Height{ Scale * 11 / 2 };
			namespace Offset {
				constexpr size_t Width{ Scale * Atlas::Offset::Width };
				constexpr size_t Height{ Scale * Atlas::Offset::Height };
			}
		}
	}
	namespace Vban {
		namespace Frame {
			constexpr size_t X{
				Plug::Frame::X
				+ Plug::Frame::Offset::Width
				+ Plug::Frame::Width
				+ Plug::Frame::Offset::Width
			};
			constexpr size_t Y{ Plug::Frame::Y };
			constexpr size_t Scale{ 2 };
			constexpr size_t Width{ Scale * 39 };
			constexpr size_t Height{ Scale * 22 };
			namespace Offset 
				constexpr size_t Width{ Scale * Atlas::Offset::Width };
				constexpr size_t Height{ Scale * Atlas::Offset::Height };
			}
		}
	}
	namespace Label {
		constexpr size_t X{ 0 };
		constexpr size_t Y{
			Knob::Frame::Y
			+ Knob::Frame::Offset::Height
			+ Knob::Frame::Height
			+ Knob::Frame::Offset::Height
		};
		constexpr size_t Stride{ 3 };
		constexpr size_t Count{ 6 + 72 };
		constexpr size_t Scale{ 3 };
		constexpr size_t Height{ Scale * 12 };
		constexpr size_t Width{ 2 * Height };
		namespace Offset {
			constexpr size_t Width{ Scale * Atlas::Offset::Width };
			constexpr size_t Height{ Scale * Atlas::Offset::Height };
		}
	}
	constexpr size_t Width{
		Label::Stride * (
			Label::Offset::Width
			+ Label::Width
			+ Label::Offset::Width)
	};
	constexpr size_t Height{
		Label::Y
		+ Label::Count / Label::Stride * (
			Label::Offset::Height
			+ Label::Height
			+ Label::Offset::Height)
	};
}

::std::regex g_coord{ R"(((?:-)?\d+(?:\.\d+)?), ((?:-)?\d+(?:\.\d+)?))" };
double g_asc{};
double g_desc{};
double g_scale{};
::std::array<double, 255> g_adv{};
::std::array<double, 255 * 255> g_kern{};

::std::string load(char const *desc,
	::std::string const &baseDir) {
	::std::string fileName{ c };
	::std::fstream input{
		baseDir + "/" + fileName,
		::std::ios::in
	};
	::std::ostringstream output{};
	for (::std::string line{}; ::std::getline(input, line);) {
		output << line << ::std::endl;
	}
	return output.str();
};
::std::string rescale(char c,
	::std::string const &baseDir) {
	double offset{
		(
			Atlas::Label::Width
			- g_scale * g_adv[c]
		) / 2.
	};
	::std::string fileName{ c };
	fileName.append(".desc");
	::std::fstream input{
		baseDir + "/" + fileName,
		::std::ios::in
	};
	::std::ostringstream output{};
	output << ::std::setprecision(15);
	for (::std::string line{}; ::std::getline(input, line);) {
		::std::smatch match{};
		if (::std::regex_search(line, match, g_coord)) {
			double x{ ::std::atof(match[1].str().c_str()) };
			double y{ ::std::atof(match[2].str().c_str()) };
			output << match.prefix().str()
				<< (x * g_scale + offset)
				<< ", "
				<< ((y - g_desc) * g_scale)
				<< match.suffix().str()
				<< ::std::endl;
		} else {
			output << line << ::std::endl;
		}
	}
	return output.str();
};
::std::string combine_and_rescale(char c1, char c2,
	::std::string const &baseDir) {
	double width12{
		g_adv[c1]
		+ g_adv[c2]
		+ g_kern[c1 * 255 + c2]
	};
	double offset{
		(
			Atlas::Label::Width
			- g_scale * width12
		) / 2.
	};
	::std::string fileName1{ c1 };
	fileName1.append(".desc");
	::std::string fileName2{ c2 };
	fileName2.append(".desc");
	::std::fstream input1{
		baseDir + "/" + fileName1,
		::std::ios::in
	};
	::std::fstream input2{
		baseDir + "/" + fileName2,
		::std::ios::in
	};
	::std::ostringstream output{};
	output << ::std::setprecision(15);
	for (::std::string line{}; ::std::getline(input1, line);) {
		::std::smatch match{};
		if (::std::regex_search(line, match, g_coord)) {
			double x{ ::std::atof(match[1].str().c_str()) };
			double y{ ::std::atof(match[2].str().c_str()) };
			output << match.prefix().str()
				<< (x * g_scale + offset)
				<< ", "
				<< ((y - g_desc) * g_scale)
				<< match.suffix().str();
		} else {
			output << line << ::std::endl;
		}
	}
	for (::std::string line{}; ::std::getline(input2, line);) {
		::std::smatch match{};
		if (::std::regex_search(line, match, g_coord)) {
			double x{ ::std::atof(match[1].str().c_str()) };
			double y{ ::std::atof(match[2].str().c_str()) };
			output << match.prefix().str()
				<< ((x + g_adv[c1] + g_kern[c1 * 255 + c2]) * g_scale + offset)
				<< ", "
				<< ((y - g_desc) * g_scale)
				<< match.suffix().str();
		} else {
			output << line << ::std::endl;
		}
	}
	return output.str();
};
::msdfgen::Bitmap<float, ChannelsSize> generate(
	char const *input,
	size_t jOffset, size_t iOffset
	size_t width, size_t height) {
	::msdfgen::Shape shape{};
	::msdfgen::readShapeDescription(input, shape);
	shape.normalize();
	::msdfgen::edgeColoringByDistance(shape, 3.);
	::msdfgen::Bitmap<float, ChannelsSize> msdf{
		jOffset + width + jOffset,
		iOffset + height + iOffset
	};
	::msdfgen::SDFTransformation transform{
		::msdfgen::Projection{
			1.,
			::msdfgen::Vector2{
				jOffset,
				iOffset
			}
		},
		Atlas::Pixel::Range
	};
	::msdfgen::generateMSDF(msdf, shape, transform);
	return msdf;
};
void combine(::std::vector<float> &atlas,
	::msdfgen::Bitmap<float, Atlas::Pixel::Channels> const &msdf,
	size_t jOffset, size_t iOffset) {
	size_t msdfRowStride{ Atlas::Pixel::Size * msdf.width() };
	for (size_t i{ 0 }; i < msdf.height(); ++i) {
		::std::memcpy(reinterpret_cast<char *>(&atlas[0])
				+ Atlas::Pixel::Size * Atlas::Width * (iOffset + i)
				+ Atlas::Pixel::Size * jOffset,
			msdf(0, i),
			msdfRowStride);
	}
};

int main(int argc, char const *argv[]) {
	::std::string baseDir{ argv[1] };
	{
		::std::fstream input{
			baseDir + "/labels.info",
			::std::ios::in
		};
		input >> g_asc >> g_desc;
		g_scale = Atlas::Label::Height / (g_asc - g_desc);
		size_t c;
		double a;
		while (input >> c >> a) {
			g_adv[c] = a;
		}
	}
	{
		::std::fstream input{
			baseDir + "/labels.kern",
			::std::ios::in
		};
		size_t c1;
		size_t c2;
		double d;
		while (input >> c1 >> c2 >> d) {
			g_kern[c1 * 255 + c2] = d;
		}
	}
	::std::vector<float> atlas(Size);
	combine(atlas,
		generate(
			load("knob_frame.desc", baseDir).c_str(),
			Atlas::Knob::Frame::Offset::Width,
			Atlas::Knob::Frame::Offset::Height,
			Atlas::Knob::Frame::Width,
			Atlas::Knob::Frame::Height),
		Atlas::Knob::Frame::X, Atlas::Knob::Frame::Y);
	combine(atlas,
		generate(
			load("plug_frame.desc", baseDir).c_str(),
			Atlas::Plug::Frame::Offset::Width,
			Atlas::Plug::Frame::Offset::Height,
			Atlas::Plug::Frame::Width,
			Atlas::Plug::Frame::Height),
		Atlas::Plug::Frame::X, Atlas::Plug::Frame::Y);
	combine(atlas,
		generate(
			load("knob_indicator.desc", baseDir).c_str(),
			Atlas::Knob::Indicator::Offset::Width,
			Atlas::Knob::Indicator::Offset::Height,
			Atlas::Knob::Indicator::Width,
			Atlas::Knob::Indicator::Height),
		Atlas::Knob::Indicator::X, Atlas::Knob::Indicator::Y);
	combine(atlas,
		generate(
			load("vban_frame.desc", baseDir).c_str(),
			Atlas::Vban::Frame::Offset::Width,
			Atlas::Vban::Frame::Offset::Height,
			Atlas::Vban::Frame::Width,
			Atlas::Vban::Frame::Height),
		Atlas::Vban::Frame::X, Atlas::Vban::Frame::Y);
	combine(atlas,
		generate(
			rescale('P', baseDir).c_str(),
			Atlas::Label::Offset::Width,
			Atlas::Label::Offset::Height,
			Atlas::Label::Width,
			Atlas::Label::Height),
		Atlas::Label::X
		+ (0 % Atlas::Label::Stride) * (
			Atlas::Label::Offset::Width
			+ Atlas::Label::Width
			+ Atlas::Label::Offset::Width),
		Atlas::Label::Y
		+ (0 / Atlas::Label::Stride) * (
			Atlas::Label::Offset::Height
			+ Atlas::Label::Height
			+ Atlas::Label::Offset::Height));
	combine(atlas,
		generate(
			rescale('V', baseDir).c_str(),
			Atlas::Label::Offset::Width,
			Atlas::Label::Offset::Height,
			Atlas::Label::Width,
			Atlas::Label::Height),
		Atlas::Label::X
		+ (1 % Atlas::Label::Stride) * (
			Atlas::Label::Offset::Width
			+ Atlas::Label::Width
			+ Atlas::Label::Offset::Width),
		Atlas::Label::Y
		+ (1 / Atlas::Label::Stride) * (
			Atlas::Label::Offset::Height
			+ Atlas::Label::Height
			+ Atlas::Label::Offset::Height));
	combine(atlas,
		generate(
			combine_and_rescale('A', '1', baseDir).c_str(),
			Atlas::Label::Offset::Width,
			Atlas::Label::Offset::Height,
			Atlas::Label::Width,
			Atlas::Label::Height),
		Atlas::Label::X
		+ (2 % Atlas::Label::Stride) * (
			Atlas::Label::Offset::Width
			+ Atlas::Label::Width
			+ Atlas::Label::Offset::Width),
		Atlas::Label::Y
		+ (2 / Atlas::Label::Stride) * (
			Atlas::Label::Offset::Height
			+ Atlas::Label::Height
			+ Atlas::Label::Offset::Height));
	combine(atlas,
		generate(
			combine_and_rescale('A', '2', baseDir).c_str(),
			Atlas::Label::Offset::Width,
			Atlas::Label::Offset::Height,
			Atlas::Label::Width,
			Atlas::Label::Height),
		Atlas::Label::X
		+ (3 % Atlas::Label::Stride) * (
			Atlas::Label::Offset::Width
			+ Atlas::Label::Width
			+ Atlas::Label::Offset::Width),
		Atlas::Label::Y
		+ (3 / Atlas::Label::Stride) * (
			Atlas::Label::Offset::Height
			+ Atlas::Label::Height
			+ Atlas::Label::Offset::Height));
	combine(atlas,
		generate(
			combine_and_rescale('B', '1', baseDir).c_str(),
			Atlas::Label::Offset::Width,
			Atlas::Label::Offset::Height,
			Atlas::Label::Width,
			Atlas::Label::Height),
		Atlas::Label::X
		+ (4 % Atlas::Label::Stride) * (
			Atlas::Label::Offset::Width
			+ Atlas::Label::Width
			+ Atlas::Label::Offset::Width),
		Atlas::Label::Y
		+ (4 / Atlas::Label::Stride) * (
			Atlas::Label::Offset::Height
			+ Atlas::Label::Height
			+ Atlas::Label::Offset::Height));
	combine(atlas,
		generate(
			combine_and_rescale('B', '2', baseDir).c_str(),
			Atlas::Label::Offset::Width,
			Atlas::Label::Offset::Height,
			Atlas::Label::Width,
			Atlas::Label::Height),
		Atlas::Label::X
		+ (5 % Atlas::Label::Stride) * (
			Atlas::Label::Offset::Width
			+ Atlas::Label::Width
			+ Atlas::Label::Offset::Width),
		Atlas::Label::Y
		+ (5 / Atlas::Label::Stride) * (
			Atlas::Label::Offset::Height
			+ Atlas::Label::Height
			+ Atlas::Label::Offset::Height));
	for (size_t i{ 0 }; i < 10; ++i) {
		combine(atlas,
			generate(
				rescale('0' + i, baseDir).c_str(),
				Atlas::Label::Offset::Width,
				Atlas::Label::Offset::Height,
				Atlas::Label::Width,
				Atlas::Label::Height),
			Atlas::Label::X
			+ ((6 + i) % Atlas::Label::Stride) * (
				Atlas::Label::Offset::Width
				+ Atlas::Label::Width
				+ Atlas::Label::Offset::Width),
			Atlas::Label::Y
			+ ((6 + i) / Atlas::Label::Stride) * (
				Atlas::Label::Offset::Height
				+ Atlas::Label::Height
				+ Atlas::Label::Offset::Height));
	}
	for (size_t i{ 10 }; i < 70 + 1; ++i) {
		combine(atlas,
			generate(
				combine_and_rescale(i / 10, i % 10, baseDir).c_str(),
				Atlas::Label::Offset::Width,
				Atlas::Label::Offset::Height,
				Atlas::Label::Width,
				Atlas::Label::Height),
			Atlas::Label::X
			+ ((6 + i) % Atlas::Label::Stride) * (
				Atlas::Label::Offset::Width
				+ Atlas::Label::Width
				+ Atlas::Label::Offset::Width),
			Atlas::Label::Y
			+ ((6 + i) / Atlas::Label::Stride) * (
				Atlas::Label::Offset::Height
				+ Atlas::Label::Height
				+ Atlas::Label::Offset::Height));
	}
	{
		::std::fstream bit{
			"Atlas.bit",
			::std::ios::binary | ::std::ios::out
		};
		bit.write(
			reinterpret_cast<char *>(
				&atlas[0]),
			Atlas::Pixel::Size * Atlas::Width
			* Atlas::Height);
	}
	return 0;
};
