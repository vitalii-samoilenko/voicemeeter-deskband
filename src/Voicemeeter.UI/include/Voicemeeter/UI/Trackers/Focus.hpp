#ifndef VOICEMEETER_UI_TRACKERS_FOCUS_HPP
#define VOICEMEETER_UI_TRACKERS_FOCUS_HPP

#include <memory>
#include <stdexcept>
#include <valarray>

#include "Voicemeeter/UI/Adapters/IComponent.hpp"
#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			template<typename TInputTracker>
			class Focus final {
			public:
				inline explicit Focus(TInputTracker &inputTracker)
					: _inputTracker{ inputTracker }
					, _target{ nullptr }
					, _mode{ FocusT::None } {

				};
				Focus() = delete;
				Focus(Focus const &) = delete;
				Focus(Focus &&) = delete;

				inline ~Focus() = default;

				Focus & operator=(Focus const &) = delete;
				Focus & operator=(Focus &&) = delete;

				template<typename TComponent>
				inline FocusT get_Track(TComponent const &target) const {
					return _tracked && _tracked->get_Target() == &target
						? _mode
						: FocusT::None;
				};
				template<typename TComponent>
				inline void set_Track(TComponent &target, FocusT value) {
					enum Owner{ None, Tracked, Target };
					enum Request{ Give, Take };
					Owner owner{
						_tracked
							? _tracked->get_Target() == &target
								? Target
								: Tracked
							: None
					};
					Request request{
						value == FocusT::None
							? Give
							: Take
					};
					if (owner == Target) {
						if (_mode == value) {
							return;
						}
					} else if (request == Give) {
						return;
					}
					if (_mode == FocusT::Fixed) {
						if (value != FocusT::Fixed) {
							_inputTracker.DisableInputTrack();
						}
					} else if (value == FocusT::Fixed) {
						_inputTracked.EnableInputTrack();
					}
					if (request == Give || owner == Tracked) {
						_tracked->Focus(FocusT::None);
						_tracked = nullptr;
					}
					if (request == Take) {
						if (owner != Target) {
							_tracked = ::std::make_unique<
								Adapters::Component<TComponent>>(
									target);
						}
						_tracked->Focus(value);
					}
					_mode = value;
				};

				inline void Focus(FocusT mode) {
					if (mode != focust::None) {
						return;
					}
					if (_mode == FocusT::Fixed) {
						_inputTracker.DisableInputTrack();
					}
					_tracked.Focus(FocusT::None);
					_tracked = nullptr;
					_mode = FocusT::None;
				};
				bool MouseLDown(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseLDown(point)
						: false;
				};
				bool MouseLDouble(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseLDouble(point)
						: false;
				};
				bool MouseLUp(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseLUp(point)
						: false;
				};
				bool MouseMDown(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseMDown(point)
						: false;
				};
				bool MouseMDouble(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseMDouble(point)
						: false;
				};
				bool MouseRDown(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseRDown(point)
						: false;
				};
				bool MouseRDouble(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseRDouble(point)
						: false;
				};
				bool MouseWheel(::std::valarray<double> const &point, int delta) {
					return _mode == FocusT::Fixed
						? _tracked->MouseWheel(point, delta)
						: false;
				};
				bool MouseMove(::std::valarray<double> const &point) {
					return _mode == FocusT::Fixed
						? _tracked->MouseMove(point)
						: false;
				};

			private:
				using FocusT = ::Voicemeeter::Focus;

				TInputTracker &_inputTracker;
				::std::unique_ptr<Adapters::IComponent> _tracked;
				FocusT _mode;
			};
		}
	}
}

#endif
