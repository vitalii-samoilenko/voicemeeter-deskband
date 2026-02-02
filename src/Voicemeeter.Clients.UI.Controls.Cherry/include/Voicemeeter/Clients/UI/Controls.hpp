#ifndef VOICEMEETER_CLIENTS_UI_CONTROLS_HPP
#define VOICEMEETER_CLIENTS_UI_CONTROLS_HPP

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
			using Controls = _::Controls::Composition<
				TTimer,
				TMixer,
				TToolkit,
				TFocusTracker>;
			
			template<
				typename TTimer,
				typename TMixer,
				typename TToolkit,
				typename TFocusTracker>
			class ControlsBuilder {
			public:
				using TLayout = TMixer;

				using Controls = Controls<
					TTimer,
					TMixer,
					TToolkit,
					TFocusTracker>;

				ControlsBuilder(ControlsBuilder const &) = delete;
				ControlsBuilder(ControlsBuilder &&) = delete;

				inline ~ControlsBuilder() = default;

				ControlsBuilder & operator=(ControlsBuilder const &) = delete;
				ControlsBuilder & operator=(ControlsBuilder &&) = delete;

				inline void set_Timer(TTimer &value) {
					_timer = &value;
				};
				inline void set_Mixer(TMixer &value) {
					_mixer = &value;
				};
				inline void set_Animated(bool value) {
					_enabled.set(_::Controls::flag::animated,
						value);
				};
				inline void set_Vban(bool value) {
					_enabled.set(_::Controls::flag::vban,
						value);
				};
				template<typename TLayout::Strips Target>
				inline void set_Enabled(bool value) {
					_enabled.set(_::Controls::flag::offset + Target,
						value);
				};

			protected:
				inline ControlsBuilder() = default;

				inline ::std::unique_ptr<Controls> Build(
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
