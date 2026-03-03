#ifndef VOICEMEETER_ADAPTERS_MIXER_HPP
#define VOICEMEETER_ADAPTERS_MIXER_HPP

#include <bitset>
#include <utility>

namespace Voicemeeter {
	namespace Adapters {
		template<typename TLayout>
		class Mixer : public TLayout {
		public:
			template<typename ...Args>
			inline explicit Mixer(Args &&...args)
				: TLayout{ ::std::forward<Args>(args) ... } {

			};
			Mixer(Mixer const &) = delete;
			Mixer(Mixer &&) = delete;

			inline ~Mixer() = default;

			Mixer & operator=(Mixer const &) = delete;
			Mixer & operator=(Mixer &&) = delete;

			template<typename TLayout::Strips From, typename TLayout::Strips To>
			inline bool get_Plug() const {
				return _plugs[
					From * TLayout::OutputSize
					+ To - TLayout::InputSize
				];
			};
			template<typename TLayout::Strips From, typename TLayout::Strips To>
			inline void set_Plug(bool value) {
				_plugs[
					From * TLayout::OutputSize
					+ To - TLayout::InputSize
				] = value;
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
