#ifndef VOICEMEETER_CLIENTS_UI_COMPOSITION_HPP
#define VOICEMEETER_CLIENTS_UI_COMPOSITION_HPP

#include <bitset>
#include <memory>
#include <stdexcept>

#include "wheel.hpp"

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

				CompositionBuilder(CompositionBuilder const &) = delete;
				CompositionBuilder(CompositionBuilder &&) = delete;

				inline ~CompositionBuilder() = default;

				CompositionBuilder & operator=(CompositionBuilder const &) = delete;
				CompositionBuilder & operator=(CompositionBuilder &&) = delete;

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
				inline CompositionBuilder & set_PaddingPosition(vector_t const &value) {
					_paddingPoint = value;
					return *this;
				};
				inline CompositionBuilder & set_PaddingSize(vector_t const &value) {
					_paddingVertex = value;
					return *this;
				};

			protected:
				inline CompositionBuilder()
					: _timer{ nullptr }
					, _mixer{ nullptr }
					, _enabled{}
					, _paddingPoint{ 0, 0 }
					, _paddingVertex{ 0, 0 } {
					_enabled.set();
				};

				inline ::std::unique_ptr<Composition> Build(
					TToolkit &toolkit,
					TFocusTracker &focusTracker) {
					if (!_timer) {
						throw ::std::runtime_error{ "Timer is not set" };
					}
					if (!_mixer) {
						throw ::std::runtime_error{ "Mixer is not set" };
					}
					return _::Controls::Subscribe(
						*_timer, *_mixer, toolkit, focusTracker,
						_enabled, _paddingPoint, _paddingVertex);
				};

			private:
				TTimer *_timer;
				TMixer *_mixer;
				::std::bitset<
					_::Controls::flags::offset
					+ TLayout::InputSize
					+ TLayout::OutputSize
				> _enabled;
				vector_t _paddingPoint;
				vector_t _paddingVertex;
			};
		}
	}
}

#endif
