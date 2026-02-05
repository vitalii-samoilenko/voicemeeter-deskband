#ifndef VOICEMEETER_UI_LOADER_HPP
#define VOICEMEETER_UI_LOADER_HPP

#include "Windows/API.hpp"

namespace Voicemeeter {
	namespace UI {
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
				inline resource(resource &&) = default;

				inline ~resource() = default;

				resource & operator=(resource const &) = delete;
				resource & operator=(resource &&) = delete;

				inline void * data() const {
					return _data;
				};
				inline size_t size() const {
					return _size;
				};

			private:
				friend class Loader;

				void *_data;
				size_t _size;

				inline resource(
					void *data,
					size_t size)
					: _data{ data }
					, _size{ size } {

				};
			};

			inline resource LoadResource(size_t id) const {
				constexpr int IdTypeLoader{ 256 };
				HRSRC hRsrc{
					::Windows::FindResourceW(_hModule,
						MAKEINTRESOURCE(id),
						MAKEINTRESOURCE(IdTypeLoader))
				};
				return resource{
					::Windows::LockResource(
						::Windows::LoadResource(
							_hModule, hRsrc)),
					static_cast<size_t>(
						::Windows::SizeofResource(
							_hModule, hRsrc))
				};
			};

		private:
			HMODULE _hModule;
		};
	}
}

#endif
