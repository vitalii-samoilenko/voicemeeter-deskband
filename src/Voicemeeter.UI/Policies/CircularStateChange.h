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
			class CircularStateChange final : public IStateChange<TState> {
				static_assert(
					::std::is_arithmetic_v<TState>,
					"TState must be of arithmetic type");

			public:
				CircularStateChange() = default;
				CircularStateChange(const CircularStateChange&) = delete;
				CircularStateChange(CircularStateChange&&) = delete;

				~CircularStateChange() = default;

				CircularStateChange& operator=(const CircularStateChange&) = delete;
				CircularStateChange& operator=(CircularStateChange&&) = delete;

				virtual bool SetDefault(TState& state) const override {
					if (state == Default) {
						return false;
					}
					state = Default;
					return true;
				}
				virtual bool SetNext(TState& state) const override {
					if (Max - Delta < state) {
						state = Min;
					} else {
						state += Delta;
					}
					return true;
				};
				virtual bool SetPrevious(TState& state) const override {
					if (state < Min + Delta) {
						state = Max;
					} else {
						state -= Delta;
					}
					return true;
				};
				virtual bool Set(TState& dst, TState& src) const override {
					if (dst == src) {
						return false;
					}
					if (src < Min) {
						dst = Max;
					} else if (Max < src) {
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