#pragma once

#include <type_traits>

#include "estd/type_traits.h"

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
			class KnobStateChange final : public IStateChange<States::Knob> {
			public:
				KnobStateChange() = default;
				KnobStateChange(const KnobStateChange&) = delete;
				KnobStateChange(KnobStateChange&&) = delete;

				~KnobStateChange() = default;

				KnobStateChange& operator=(const KnobStateChange&) = delete;
				KnobStateChange& operator=(KnobStateChange&&) = delete;

				virtual bool SetDefault(States::Knob& state) const override {
					if (state.level == Default) {
						return false;
					}
					state.level = Default;
					return true;
				}
				virtual bool SetNext(States::Knob& state) const override {
					if (Max == state.level) {
						return false;
					} else if (Max - Delta < state.level) {
						state.level = Max;
					} else {
						state.level += Delta;
					}
					return true;
				};
				virtual bool SetPrevious(States::Knob& state) const override {
					if (state.level == Min) {
						return false;
					} else if (state.level < Min + Delta) {
						state.level = Min;
					} else {
						state.level -= Delta;
					}
					return true;
				};
				virtual bool Set(States::Knob& dst, States::Knob& src) const override {
					if (dst == src) {
						return false;
					}
					if (dst.pinned != src.pinned) {
						dst = src;
					} else if (Max < src.level) {
						if (Max == dst.level) {
							return false;
						}
						dst.level = Max;
					} else if (src.level < Min) {
						if (dst.level == Min) {
							return false;
						}
						dst.level = Min;
					} else {
						dst.level = src.level;
					}
					return true;
				};
			};
		}
	}
}