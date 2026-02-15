#include <array>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

constexpr size_t Height{ 42 };
constexpr size_t Width{ 2 * Height  };

::std::regex g_coord{ R"(((?:-)?\d+(?:\.\d+)?), ((?:-)?\d+(?:\.\d+)?))" };
double g_asc{};
double g_desc{};
double g_scale{};
::std::array<double, 255> g_adv{};
::std::array<double, 255 * 255> g_kern{};

void rescale(char c,
	::std::string const &baseDir) {
	double offset{
		(
			Width - g_scale * g_adv[c]
		) / 2.
	};
	::std::string fileName{ c };
	fileName.append(".desc");
	::std::fstream input{
		baseDir + "/" + fileName,
		::std::ios::in
	};
	::std::fstream output{
		fileName,
		::std::ios::out
	};
	::std::string line{};
	while (::std::getline(input, line)) {
		::std::smatch match{};
		if (::std::regex_search(line, match, g_coord)) {
			double x{ ::std::atof(match[1].str().c_str()) };
			double y{ ::std::atof(match[2].str().c_str()) };
			::std::ostringstream re{};
			re << ::std::setprecision(15)
				<< (x * g_scale + offset)
				<< ", "
				<< ((y - g_desc) * g_scale);
			line = match.prefix().str() + re.str() + match.suffix().str();
		}
		output << line;
	}
};
void combine_and_rescale(char c1, char c2,
	::std::string const &baseDir) {
	double width12{
		g_adv[c1]
		+ g_adv[c2]
		+ g_kern[c1 * 255 + c2]
	};
	double offset{
		(
			Width - g_scale * width12
		) / 2.
	};
	::std::string fileName1{ c1 };
	fileName1.append(".desc");
	::std::string fileName2{ c2 };
	fileName2.append(".desc");
	::std::string fileName12{ c1 };
	fileName12.append(::std::string{ c2 });
	fileName12.append(".desc");
	::std::fstream input1{
		baseDir + "/" + fileName1,
		::std::ios::in
	};
	::std::fstream input2{
		baseDir + "/" + fileName2,
		::std::ios::in
	};
	::std::fstream output{
		fileName12,
		::std::ios::out
	};
	::std::string line{};
	while (::std::getline(input1, line)) {
		::std::smatch match{};
		if (::std::regex_search(line, match, g_coord)) {
			double x{ ::std::atof(match[1].str().c_str()) };
			double y{ ::std::atof(match[2].str().c_str()) };
			::std::ostringstream re{};
			re << ::std::setprecision(15)
				<< (x * g_scale + offset)
				<< ", "
				<< ((y - g_desc) * g_scale);
			line = match.prefix().str() + re.str() + match.suffix().str();
		}
		output << line;
	}
	while (::std::getline(input2, line)) {
		::std::smatch match{};
		if (::std::regex_search(line, match, g_coord)) {
			double x{ ::std::atof(match[1].str().c_str()) };
			double y{ ::std::atof(match[2].str().c_str()) };
			::std::ostringstream re{};
			re << ::std::setprecision(15)
				<< ((x + g_adv[c1] + g_kern[c1 * 255 + c2]) * g_scale + offset)
				<< ", "
				<< ((y - g_desc) * g_scale);
			line = match.prefix().str() + re.str() + match.suffix().str();
		}
		output << line;
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
		g_scale = Height / (g_asc - g_desc);
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
	rescale('P', baseDir);
	rescale('V', baseDir);
	combine_and_rescale('A', '1', baseDir);
	combine_and_rescale('A', '2', baseDir);
	combine_and_rescale('B', '1', baseDir);
	combine_and_rescale('B', '2', baseDir);
	for (char c{ '0' }; c < '9' + 1; ++c) {
		rescale(c, baseDir);
	}
	for (char c1{ '1' }; c1 < '6' + 1; ++c1) {
		rescale(c1, baseDir);
		for (char c2{ '0' };  c2 < '9' + 1; ++c2) {
			combine_and_rescale(c1, c2, baseDir);
		}
	}
	combine_and_rescale('7', '0', baseDir);
	return 0;
};
