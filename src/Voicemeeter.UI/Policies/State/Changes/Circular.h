#pragma once

#include <type_traits>

#include "../IChange.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					template<typename TState,
						TState Default,
						TState Min,
						TState Max,
						TState Delta>
					class Circular final : public IChange<TState> {
						static_assert(
							::std::is_arithmetic_v<TState>,
							"TState must be of arithmetic type");

					public:
						Circular() = default;
						Circular(const Circular&) = delete;
						Circular(Circular&&) = delete;

						~Circular() = default;

						Circular& operator=(const Circular&) = delete;
						Circular& operator=(Circular&&) = delete;

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
	}
}