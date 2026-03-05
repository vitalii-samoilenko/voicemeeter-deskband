#include <filesystem>
#include <fstream>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}
	::std::fstream rc{
		"Loader.rc",
		::std::ios::out | ::std::ios::trunc
	};
	rc << "LANGUAGE 0x9, 0x409" << ::std::endl;
	for (int i{ 1 }; i < argc; ++i) {
		::std::filesystem::path file{ argv[i] };
		rc << i << " 256 " << '"' << file.filename().string() << '"' << ::std::endl;
	}
	return 0;
}
