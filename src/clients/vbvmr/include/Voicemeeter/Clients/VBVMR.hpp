#ifndef VOICEMEETER_CLIENTS_VBVMR_HPP
#define VOICEMEETER_CLIENTS_VBVMR_HPP

#include <memory>
#include <stdexcept>

#include "Voicemeeter/Clients/_/VBVMR.hpp"

namespace Voicemeeter {
	namespace Clients {
		template<
			typename TTimer,
			typename TMixer>
		using VBVMR = _::VBVMR::Proxy<TTimer, TMixer>;

		template<
			typename TTimer,
			typename TMixer>
		class VBVMRBuilder final {
		public:
			using VBVMR = VBVMR<TTimer, TMixer>;

			inline VBVMRBuilder()
				: _timer{ nullptr }
				, _mixer{ nullptr } {

			};
			VBVMRBuilder(VBVMRBuilder const &) = delete;
			VBVMRBuilder(VBVMRBuilder &&) = delete;

			inline ~VBVMRBuilder() = default;

			VBVMRBuilder & operator=(VBVMRBuilder const &) = delete;
			VBVMRBuilder & operator=(Remote &&) = delete;

			inline VBVMRBuilder & set_Timer(TTimer &value) {
				_timer = &value;
				return *this;
			};
			inline VBVMRBuilder & set_Mixer(TMixer &value) {
				_mixer = &value;
				return *this;
			};

			::std::unique_ptr<VBVMR> Build() const {
				if (!_timer) {
					throw ::std::runtime_error{ "Timer is not set" };
				}
				if (!_mixer) {
					throw ::std::runtime_error{ "Mixer is not set" };
				}
				return ::std::make_unique<
					VBVMR>(*_timer, *_mixer);
			};

		private:
			TTimer *_timer;
			TMixer *_mixer;
		};
	}
}

#endif
