#include <array>
#include <fstream>
#include <string>

int main(int argc, char const *argv[]) {
	::std::string baseDir{ argv[1] };
	double scale{ 32. };
	::std::array<double, 255> adv{};
	::std::array<double, 255 * 255> kern{};
	{
		::std::fstream info{
			baseDir + "labels.info",
			::std::ios::in
		};
		char c;
		double d;
		info >> c >> d;
		adv[c] = d;
	}
	{
		::std::fstream kern{
			baseDir + "labels.kern",
			::std::ios::in
		};
		char c1;
		char c2;
		double d;
		kern >> c1 >> c2 >> d;
		kern[c1 * 255 + c2] = d;
	}
	{
		::std::fstream shape1{
			baseDir + "P.desc",
			::std::ios::in
		};
		::std::fstream output{
			"P.desc",
			::std::io::out
		};
		output << ::std::setprecision(15);
	}
	return 0;
};
