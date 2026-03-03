#ifndef WUI_GRAPHICS_LOADER_HPP
#define WUI_GRAPHICS_LOADER_HPP

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "Windows/API.hpp"

#include "Alloc.h"
#include "LzmaDec.h"

namespace WUI {
	namespace Graphics {
		class Loader final {
		public:
			inline explicit Loader(HMODULE hModule)
				: _hModule{ hModule } {

			};
			Loader() = delete;
			Loader(Loader const &) = delete;
			Loader(Loader &&) = delete;

			inline ~Loader() = default;

			Loader & operator=(Loader const &) = delete;
			Loader & operator=(Loader &&) = delete;

			class resource {
			public:
				resource() = delete;
				resource(resource const &) = delete;
				inline resource(resource && other)
					: _data{ other._data }
					, _size{ other._size } {
					other._data = nullptr;
				};

				inline ~resource() {
					delete[] _data;
				};

				resource & operator=(resource const &) = delete;
				resource & operator=(resource &&) = delete;

				inline uint8_t * data() const {
					return _data;
				};
				inline size_t size() const {
					return _size;
				};

			private:
				friend class Loader;

				uint8_t *_data;
				size_t _size;

				inline explicit resource(size_t size)
					: _data{ new uint8_t[size] }
					, _size{ size } {

				};
			};

			inline resource LoadResource(size_t id) const {
				constexpr int IdTypeLoader{ 256 };
				HRSRC hRsrc{
					::Windows::FindResourceW(_hModule,
						MAKEINTRESOURCEW(id),
						MAKEINTRESOURCEW(IdTypeLoader))
				};
				uint8_t *archive{
					reinterpret_cast<uint8_t *>(
						::Windows::LockResource(
							::Windows::LoadResource(
								_hModule, hRsrc)))
				};
				size_t asize{
					::Windows::SizeofResource(
						_hModule, hRsrc)
				};
				uint8_t header[LZMA_PROPS_SIZE + 8];
				size_t hsize{ sizeof(header) };
				::std::memcpy(header, archive, hsize);
				archive += hsize;
				asize -= hsize;
				size_t size{ 0 };
				for (size_t i{ 0 }; i < 8; ++i) {
					size += static_cast<size_t>(
						header[LZMA_PROPS_SIZE + i]) << (i * 8);
				}
				resource target{ size };
				ELzmaStatus status{ LZMA_STATUS_NOT_SPECIFIED };
				if (!(SZ_OK == ::LzmaDecode(
						target._data, &size, archive, &asize,
						header, LZMA_PROPS_SIZE,
						LZMA_FINISH_END, &status,
						&g_Alloc)
					&& status == LZMA_STATUS_FINISHED_WITH_MARK)) {
					throw ::std::runtime_error{ "Decompression error" };
				}
				return target;
			};

		private:
			HMODULE _hModule;
		};
	}
}

#endif
