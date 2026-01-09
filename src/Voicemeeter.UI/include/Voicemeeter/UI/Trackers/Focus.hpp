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
					, _tracked{ nullptr }
					, _trackedId{ nullptr }
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
					return _trackedId == &target
						? _mode
						: FocusT::None;
				};
				template<typename TComponent>
				inline void set_Track(TComponent &target, FocusT value) {
					if (_trackedId == &target) {
						if (_mode == value) {
							return;
						}
					} else if (value == FocusT::None) {
						return;
					}
					if (_mode == FocusT::Fixed) {
						if (value != FocusT::Fixed) {
							_inputTracker.DisableInputTrack();
						}
					} else if (value == FocusT::Fixed) {
						_inputTracker.EnableInputTrack();
					}
					if (_tracked) {
						if (value == FocusT::None || _trackedId != &target) {
							_tracked->Focus(FocusT::None);
							_tracked = nullptr;
							_trackedId = nullptr;
						}
					}
					if (value != FocusT::None) {
						if (_trackedId == nullptr) {
							_tracked = ::std::make_unique<
								Adapters::Component<TComponent>>(
									target);
							_trackedId = &target;
						}
						_tracked->Focus(value);
					}
					_mode = value;
				};

				inline void Focus(FocusT mode) {
					if (mode != FocusT::None) {
						return;
					}
					if (_mode == FocusT::Fixed) {
						_inputTracker.DisableInputTrack();
					}
					_tracked.Focus(FocusT::None);
					_tracked = nullptr;
					_trackedId = nullptr;
					_mode = FocusT::None;
				};
				bool MouseLDown(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseLDown(point);
				};
				bool MouseLDouble(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseLDouble(point);
				};
				bool MouseLUp(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseLUp(point);
				};
				bool MouseMDown(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseMDown(point);
				};
				bool MouseMDouble(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseMDouble(point);
				};
				bool MouseRDown(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseRDown(point);
				};
				bool MouseRDouble(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseRDouble(point);
				};
				bool MouseWheel(::std::valarray<int> const &point, int delta) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseWheel(point, delta);
				};
				bool MouseMove(::std::valarray<int> const &point) {
					return _mode == FocusT::Fixed
						&& _tracked->MouseMove(point);
				};

			private:
				using FocusT = ::Voicemeeter::Focus;

				TInputTracker &_inputTracker;
				::std::unique_ptr<Adapters::IComponent> _tracked;
				void const *_trackedId;
				FocusT _mode;
			};
		}
	}
}

#endif
