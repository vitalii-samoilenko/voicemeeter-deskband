#ifndef VOICEMEETER_CLIENTS_UI_COMPOSITION_HPP
#define VOICEMEETER_CLIENTS_UI_COMPOSITION_HPP

#include <bitset>
#include <exception>
#include <memory>

#include "Voicemeeter/Clients/UI/_/Controls.hpp"
#include "Voicemeeter/UI/Adapters/IComponent.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			template<
				typename TTimer,
				typename TMixer,
				typename TToolkit,
				typename TFocusTracker>
			using Composition = _::Controls::Composition<
				TTimer,
				TMixer,
				TToolkit,
				TFocusTracker>;
			
			template<
				typename TTimer,
				typename TMixer,
				typename TToolkit,
				typename TFocusTracker>
			class CompositionBuilder {
			public:
				using TLayout = TMixer;

				using Composition = Composition<
					TTimer,
					TMixer,
					TToolkit,
					TFocusTracker>;

				ControlsBuilder(ControlsBuilder const &) = delete;
				ControlsBuilder(ControlsBuilder &&) = delete;

				inline ~ControlsBuilder() = default;

				ControlsBuilder & operator=(ControlsBuilder const &) = delete;
				ControlsBuilder & operator=(ControlsBuilder &&) = delete;

				inline CompositionBuilder & set_Timer(TTimer &value) {
					_timer = &value;
					return *this;
				};
				inline CompositionBuilder & set_Mixer(TMixer &value) {
					_mixer = &value;
					return *this;
				};
				inline CompositionBuilder & set_Animated(bool value) {
					_enabled.set(_::Controls::flags::animated,
						value);
					return *this;
				};
				inline CompositionBuilder & set_Vban(bool value) {
					_enabled.set(_::Controls::flags::vban,
						value);
					return *this;
				};
				template<typename TLayout::Strips Target>
				inline CompositionBuilder & set_Enabled(bool value) {
					_enabled.set(_::Controls::flags::offset + Target,
						value);
					return *this;
				};

			protected:
				inline ControlsBuilder()
					: _timer{ nullptr }
					, _mixer{ nullptr }
					, _enabled{} {
					_enabled.set();
				};

				inline ::std::unique_ptr<Composition> Build(
					TToolkit &toolkit,
					TFocusTracker &focusTracker) {
					if (!_timer) {
						throw ::std::exception{ "Timer is not set" };
					}
					if (!_mixer) {
						throw ::std::exception{ "Mixer is not set" };
					}
					return _::Controls::Subscribe(
						*_timer, *_mixer, toolkit, focusTracker,
						_enabled);
				};

			private:
				TTimer *_timer;
				TMixer *_mixer;
				::std::bitset<
					_::Controls::flag::offset
					+ TLayout::InputSize
					+ TLayout::OutputSize
				> _enabled;
			};
		}
	}
}

#endif
