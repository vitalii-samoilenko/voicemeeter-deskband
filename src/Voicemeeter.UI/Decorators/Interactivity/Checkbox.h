#pragma once

#include <type_traits>
#include <utility>

#include "../../Controls/Checkbox.h"
#include "../../Trackers/Focus.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<
					typename TGlyph,
					typename TChangeNotify,
					typename TGlyphUpdate,
					typename TCheckbox>
				class Checkbox : public TCheckbox {
					static_assert(
						::std::is_base_of_v<Controls::Checkbox<TGlyph, TChangeNotify, TGlyphUpdate>, TCheckbox>,
						"TCheckbox must be derived from Checkbox");

				public:
					template<typename... Args>
					explicit Checkbox(
						Trackers::Focus& focusTracker,
						Args&& ...args
					) : TCheckbox{ ::std::forward<Args>(args)... }
					  , m_focusTracker{ focusTracker } {

					}
					Checkbox() = delete;
					Checkbox(const Checkbox&) = delete;
					Checkbox(Checkbox&&) = delete;

					~Checkbox() = default;

					Checkbox& operator=(const Checkbox&) = delete;
					Checkbox& operator=(Checkbox&&) = delete;

					virtual bool MouseLDown(const ::std::valarray<double>& point) override {
						m_focusTracker.set_Track(*this, true);

						TCheckbox::SetNext();

						return true;
					}
					virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
						TCheckbox::SetNext();

						return true;
					}

				private:
					Trackers::Focus& m_focusTracker;
				};
			}
		}
	}
}