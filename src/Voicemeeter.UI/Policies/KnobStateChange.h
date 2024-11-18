#pragma once

#include <utility>

#include "../States/Knob.h"
#include "IStateChange.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<
				int Default,
				int Min,
				int Max,
				int Delta>
			class KnobStateChange : public IStateChange<States::Knob> {
			public:
				KnobStateChange() = default;
				KnobStateChange(const KnobStateChange&) = delete;
				KnobStateChange(KnobStateChange&&) = delete;

				~KnobStateChange() = default;

				KnobStateChange& operator=(const KnobStateChange&) = delete;
				KnobStateChange& operator=(KnobStateChange&&) = delete;

				virtual bool SetDefault(States::Knob& state) const override {
					if (state.gain == Default) {
						return false;
					}
					state.gain = Default;
					//state.hold = true;
					return true;
				}
				virtual bool SetNext(States::Knob& state) const override {
					if (Max == state.gain) {
						return false;
					} else if (Max - Delta < state.gain) {
						state.gain = Max;
					} else {
						state.gain += Delta;
					}
					state.hold = true;
					return true;
				};
				virtual bool SetPrevious(States::Knob& state) const override {
					if (state.gain == Min) {
						return false;
					} else if (state.gain < Min + Delta) {
						state.gain = Min;
					} else {
						state.gain -= Delta;
					}
					state.hold = true;
					return true;
				};
				virtual bool Set(States::Knob& dst, States::Knob& src) const override {
					bool result{ false };

					if (dst.gain != src.gain) {
						if (Max < src.gain) {
							if (Max != dst.gain) {
								dst.gain = Max;
								result = true;
							}
						} else if (src.gain < Min) {
							if (dst.gain != Min) {
								dst.gain = Min;
								result = true;
							}
						} else {
							dst.gain = src.gain;
							result = true;
						}
					}
					if (dst.level.size() != src.level.size()
						|| (dst.level != src.level).max()) {
						dst.level = ::std::move(src.level);
						result = true;
					}
					if (dst.toggle != src.toggle) {
						dst.toggle = src.toggle;
						result = true;
					}
					if (dst.hold != src.hold) {
						dst.hold = src.hold;
						result = true;
					}

					return result;
				};
			};
		}
	}
}