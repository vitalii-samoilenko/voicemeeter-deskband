#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "Alloc.h"
#include "LzmaEnc.h"

int main(int argc, char *argv[]) {
	::std::fstream hpp{
		"WUI/Layouts/Loader.hpp",
		::std::ios::out | ::std::ios::trunc
	};
	{
		hpp << "#ifndef WUI_LAYOUTS_LOADER_HPP" << ::std::endl;
		hpp << "#define WUI_LAYOUTS_LOADER_HPP" << ::std::endl;
		hpp << ::std::endl;
		hpp << "namespace WUI {" << ::std::endl;
		hpp << "	namespace Layouts {" << ::std::endl;
		hpp << "		namespace Loader {" << ::std::endl;
	}
	::std::fstream rc{
		"Loader.rc",
		::std::ios::out | ::std::ios::trunc
	};
	{
		rc << "LANGUAGE 0x9, 0x409" << ::std::endl;
	}
	for (int i{ 1 }; i < argc; ++i) {
		::std::filesystem::path file{ argv[i] };
		{
			size_t size{ ::std::filesystem::file_size(file) };
			::std::vector<uint8_t> ibuffer(size);
			::std::vector<uint8_t> obuffer(size);
			uint8_t header[LZMA_PROPS_SIZE + 8];
			for (size_t i{ 0 }; i < 8; ++i) {
				header[LZMA_PROPS_SIZE + i] = static_cast<uint8_t>(
					size >> (8 * i));
			}
			{
				::std::fstream in{
					file.string(),
					::std::ios::binary | ::std::ios::in
				};
				in.read(reinterpret_cast<char *>(ibuffer.data()), size);
			}
			{
				CLzmaEncProps props;
				::LzmaEncProps_Init(&props);
				size_t hsize{ LZMA_PROPS_SIZE };
				if(!(SZ_OK == ::LzmaEncode(
						obuffer.data(), &size, ibuffer.data(), size,
						&props, &header[0], &hsize,
						1, NULL,
						&g_Alloc, &g_Alloc))) {
					throw ::std::runtime_error{ "Compression error" };
				}
			}
			{
				::std::fstream out{
					file.filename().string(),
					::std::ios::binary | ::std::ios::out | ::std::ios::trunc
				};
				out.write(reinterpret_cast<char *>(header), sizeof(header));
				out.write(reinterpret_cast<char *>(obuffer.data()), size);
			}
		}
		{
			hpp << "			namespace " << file.extension().string().substr(1) << " {" << ::std::endl;
			hpp << "				constexpr size_t " << file.stem().string() << "{ " << i << " };" << ::std::endl;
			hpp << "			}" << ::std::endl;
		}
		{
			rc << i << " 256 " << '"' << file.filename().string() << '"' << ::std::endl;
		}
	}
	{
		hpp << "		}" << ::std::endl;
		hpp << "	}" << ::std::endl;
		hpp << "}" << ::std::endl;
		hpp << ::std::endl;
		hpp << "#endif" << ::std::endl;
	}
};
