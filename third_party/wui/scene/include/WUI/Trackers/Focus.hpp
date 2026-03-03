#ifndef WUI_TRACKERS_FOCUS_HPP
#define WUI_TRACKERS_FOCUS_HPP

#include <memory>

#include "wheel.hpp"

#include "WUI/Adapters/IComponent.hpp"
#include "WUI/Focus.hpp"

namespace WUI {
	namespace Trackers {
		template<typename TInputTracker>
		class Focus final {
		public:
			inline explicit Focus(TInputTracker &inputTracker)
				: _inputTracker{ inputTracker }
				, _tracked{ nullptr }
				, _trackedId{ nullptr }
				, _mode{ UI::Focus::None } {

			};
			Focus() = delete;
			Focus(Focus const &) = delete;
			Focus(Focus &&) = delete;

			inline ~Focus() = default;

			Focus & operator=(Focus const &) = delete;
			Focus & operator=(Focus &&) = delete;

			template<typename TComponent>
			inline UI::Focus get_Track(TComponent &target) const {
				return _trackedId == &target
					? _mode
					: UI::Focus::None;
			};
			template<typename TComponent>
			inline void set_Track(TComponent &target, UI::Focus value) {
				if (_trackedId == &target) {
					if (_mode == value) {
						return;
					}
				} else if (value == UI::Focus::None) {
					return;
				}
				if (_mode == UI::Focus::Fixed) {
					if (value != UI::Focus::Fixed) {
						_inputTracker.DisableInputTrack();
					}
				} else if (value == UI::Focus::Fixed) {
					_inputTracker.EnableInputTrack();
				}
				if (_trackedId) {
					if (value == UI::Focus::None || _trackedId != &target) {
						_tracked->set_Focus(UI::Focus::None);
						_tracked = nullptr;
						_trackedId = nullptr;
					}
				}
				if (value != UI::Focus::None) {
					if (!_trackedId) {
						_tracked = ::std::make_unique<
							Adapters::Component<TComponent>>(
							&target);
						_trackedId = &target;
					}
					_tracked->set_Focus(value);
				}
				_mode = value;
			};
			template<typename TComponent>
			inline void up_Track(TComponent &target, UI::Focus value) {
				if (get_Track(target) < value) {
					set_Track(target, value);
				}
			};
			template<typename TComponent>
			inline void down_Track(TComponent &target, UI::Focus value) {
				if (value < get_Track(target)) {
					set_Track(target, value);
				}
			};

			inline void set_Focus(UI::Focus value) {
				if (value != UI::Focus::None) {
					return;
				}
				if (_mode == UI::Focus::Fixed) {
					_inputTracker.DisableInputTrack();
				}
				_tracked->set_Focus(UI::Focus::None);
				_tracked = nullptr;
				_trackedId = nullptr;
				_mode = UI::Focus::None;
			};

			bool MouseLDown(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseLDown(point);
			};
			bool MouseLDouble(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseLDouble(point);
			};
			bool MouseLUp(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseLUp(point);
			};
			bool MouseMDown(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseMDown(point);
			};
			bool MouseMDouble(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseMDouble(point);
			};
			bool MouseRDown(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseRDown(point);
			};
			bool MouseRDouble(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseRDouble(point);
			};
			bool MouseWheel(vec_t const &point, num_t delta) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseWheel(point, delta);
			};
			bool MouseMove(vec_t const &point) {
				return _mode == UI::Focus::Fixed
					&& _tracked->MouseMove(point);
			};

		private:
			TInputTracker &_inputTracker;
			::std::unique_ptr<Adapters::IComponent> _tracked;
			void const *_trackedId;
			UI::Focus _mode;
		};
	}
}

#endif
