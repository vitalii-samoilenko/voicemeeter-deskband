#pragma once

#include <type_traits>
#include <utility>

#include "../../Direction.h"
#include "../../Controls/Slider.h"
#include "../../IInputTracker.h"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Interactivity {
				template<UI::Direction Direction, typename TSlider, typename TGlyph>
				class Slider : public TSlider {
					static_assert(
						::std::is_base_of_v<Controls::Slider<TGlyph>, TSlider>,
						"TSlider must be derived from Slider<TGlyph>");

				public:
					template<typename... Args>
					explicit Slider(
						IInputTracker& inputTracker,
						Args&& ...args
					) : T{ ::std::forward<Args>(args)... }
						, m_inputTracker{ inputTracker } {

					}
					Slider() = delete;
					Slider(const Slider&) = delete;
					Slider(Slider&&) = delete;

					~Slider() = default;

					Slider& operator=(const Slider&) = delete;
					Slider& operator=(Slider&&) = delete;

					virtual bool MouseLDown(const ::std::valarray<double>& point) override {
						m_inputTracker.set_Track(*this, true);
						m_inputTracker.set_Position(point);

						return true;
					};
					virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
						TSlider::SetDefault();

						return true;
					};
					virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
						int level{ TSlider::get_State() + delta * 5 };

						TSlider::Set(level, true);

						return true;
					};
					virtual bool MouseMove(const ::std::valarray<double>& point) override {
						if (!m_inputTracker.get_Track(*this)) {
							return true;
						}

						OnMouseMove(point, UI::Direction_Tag<Direction>{});

						m_inputTracker.set_Position(point);

						return true;
					};
					virtual bool MouseLUp(const ::std::valarray<double>& point) override {
						m_inputTracker.set_Track(*this, false);
					};

				private:
					IInputTracker& m_inputTracker;

					void OnMouseMove(const ::std::valarray<double>& point, const UI::Direction_Tag<UI::Direction::Right>&) {
						double scale{ TSlider::get_Size()[0] / TSlider::get_BaseSize()[0] };
						int state{ TSlider::get_State() + static_cast<int>((point[0] - m_inputTracker.get_Position()[0]) * 100. / scale)};
						TSlider::Set(state, true);
					}

					void OnMouseMove(const ::std::valarray<double>& point, const Direction_Tag<UI::Direction::Down>&) {
						double scale{ TSlider::get_Size().y / TSlider::get_BaseSize().y };
						int state{ TSlider::get_State() + static_cast<int>((point[1] - m_inputTracker.get_Position()[1]) * 100. / scale)};
						TSlider::Set(state, true);
					}
				};
			}
		}
	}
}