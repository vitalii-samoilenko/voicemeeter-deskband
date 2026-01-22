#ifndef VOICEMEETER_ADAPTERS_MIXER_HPP
#define VOICEMEETER_ADAPTERS_MIXER_HPP

#include <bitset>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		template<typename TLayout>
		class Mixer : public TLayout {
		public:
			template<typename... Args>
			inline explicit Mixer(Args &&...args)
				: TLayout{ ::std::forward<Args>(args) ... } {

			};
			Mixer(Mixer const &) = delete;
			Mixer(Mixer &&) = delete;

			inline ~Mixer() = default;

			Mixer & operator=(Mixer const &) = delete;
			Mixer & operator=(Mixer &&) = delete;

			template<typename TIBus, typename TOBus>
			inline bool get_Plug(TIBus const &input, TOBus const &output) const {
				size_t i{ TLayout::get_Index(input) };
				size_t j{ TLayout::get_Index(output) };
				return _plugs[TLayout::OutputSize * i + j];
			};
			template<typename TIBus, typename TOBus>
			inline void set_Plug(TIBus const &input, TOBus const &output, bool value) {
				size_t i{ TLayout::get_Index(input) };
				size_t j{ TLayout::get_Index(output) };
				_plugs[TLayout::OutputSize * i + j] = value;
			};

		private:
			::std::bitset<
				TLayout::InputSize
				* TLayout::OutputSize
			> _plugs;
		};
	}
}

#endif
