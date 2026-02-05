#include <fstream>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}
	::std::fstream rc{
		"Loader.rc",
		::std::ius::out | ::std::ios::trunc
	};
	rc << "LANGUAGE 0x9, 0x409" << ::std::endl;
	for (int i{ 1 }; i < argc; ++i) {
		rc << i << " 256 " << '"' << argv[i] << '"' << ::std::endl;
	}
	return 0;
}
