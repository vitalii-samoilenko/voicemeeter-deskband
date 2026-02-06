#include <filesystem>
#include <fstream>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}
	::std::fstream hpp{
		"Voicemeeter/UI/Layouts/Loader.hpp",
		::std::ios::out | ::std::ios::trunc
	};
	hpp << "#ifndef VOICEMEETER_UI_LAYOUTS_LOADER_HPP" << ::std::endl;
	hpp << "#define VOICEMEETER_UI_LAYOUTS_LOADER_HPP" << ::std::endl;
	hpp << ::std::endl;
	hpp << "namespace Voicemeeter {" << ::std::endl;
	hpp << "	namespace UI {" << ::std::endl;
	hpp << "		namespace Layouts {" << ::std::endl;
	hpp << "			namespace Loader {" << ::std::endl;
	for (int i{ 1 }; i < argc; ++i) {
		::std::filesystem::path file{ argv[i] };
		hpp << "				namespace " << file.extension().string().substr(1) << " {" << ::std::endl;
		hpp << "					constexpr size_t " << file.stem().string() << "{ " << i << " };" << ::std::endl;
		hpp << "				}" << ::std::endl;
	}
	hpp << "			}" << ::std::endl;
	hpp << "		}" << ::std::endl;
	hpp << "	}" << ::std::endl;
	hpp << "}" << ::std::endl;
	hpp << ::std::endl;
	hpp << "#endif" << ::std::endl;
	return 0;
};
