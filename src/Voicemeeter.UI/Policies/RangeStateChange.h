#pragma once

#include <type_traits>

#include "IStateChange.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TState,
				TState Default,
				TState Min,
				TState Max,
				TState Delta>
			class RangeStateChange final : public IStateChange<TState> {
				static_assert(
					::std::is_arithmetic_v<TState>,
					"TState must be of arithmetic type");

			public:
				RangeStateChange() = default;
				RangeStateChange(const RangeStateChange&) = delete;
				RangeStateChange(RangeStateChange&&) = delete;

				~RangeStateChange() = default;

				RangeStateChange& operator=(const RangeStateChange&) = delete;
				RangeStateChange& operator=(RangeStateChange&&) = delete;

				virtual bool SetDefault(TState& state) const override {
					if (state == Default) {
						return false;
					}
					state = Default;
					return true;
				}
				virtual bool SetNext(TState& state) const override {
					if (Max == state) {
						return false;
					} else if (Max - Delta < state) {
						state = Max;
					} else {
						state += Delta;
					}
					return true;
				};
				virtual bool SetPrevious(TState& state) const override {
					if (state == Min) {
						return false;
					} else if (state < Min + Delta) {
						state = Min;
					} else {
						state -= Delta;
					}
					return true;
				};
				virtual bool Set(TState& dst, TState& src) const override {
					if (dst == src) {
						return false;
					}
					if (Max < src) {
						if (Max == dst) {
							return false;
						}
						dst = Max;
					} else if (src < Min) {
						if (dst == Min) {
							return false;
						}
						dst = Min;
					} else {
						dst = src;
					}
					return true;
				};
			};
		}
	}
}