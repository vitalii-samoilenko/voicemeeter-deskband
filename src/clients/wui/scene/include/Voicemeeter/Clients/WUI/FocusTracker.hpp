#ifndef VOICEMEETER_CLIENTS_WUI_FOCUSTRACKER_HPP
#define VOICEMEETER_CLIENTS_WUI_FOCUSTRACKER_HPP

#include <memory>
#include <stdexcept>

#include "WUI/Trackers/Focus.hpp"

namespace Voicemeeter {
	namespace Clients {
		namespace WUI {
			template<typename TInputTracker>
			using FocusTracker = ::WUI::Trackers::Focus<TInputTracker>;

			template<typename TInputTracker>
			class FocusTrackerBuilder {
			public:
				using FocusTracker = FocusTracker<TInputTracker>;

				FocusTrackerBuilder(FocusTrackerBuilder const &) = delete;
				FocusTrackerBuilder(FocusTrackerBuilder &&) = delete;

				inline ~FocusTrackerBuilder() = default;

				FocusTrackerBuilder & operator=(FocusTrackerBuilder const &) = delete;
				FocusTrackerBuilder & operator=(FocusTrackerBuilder &&) = delete;

				inline FocusTrackerBuilder & set_InputTracker(TInputTracker &value) {
					_inputTracker = &value;
					return *this;
				};

			protected:
				inline FocusTrackerBuilder() = default;

				inline ::std::unique_ptr<FocusTracker> Build() {
					if (!_inputTracker) {
						throw ::std::runtime_error{ "Input tracker is not set" };
					}
					return ::std::make_unique<
						FocusTracker>(*_inputTracker);
				};

			private:
				TInputTracker *_inputTracker;
			};
		}
	}
}

#endif
