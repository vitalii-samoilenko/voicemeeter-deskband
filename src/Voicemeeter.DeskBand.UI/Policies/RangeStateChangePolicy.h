#pragma once

#include <type_traits>

#include "estd/type_traits.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState, TState Min, TState Max, TState Delta>
				class RangeStateChangePolicy final : public IStateChangePolicy<TState> {
					static_assert(
						::std::is_arithmetic_v<TState>,
						"TState must be of arithmetic type"));

				public:
					RangeStateChangePolicy() = default;
					RangeStateChangePolicy(const RangeStateChangePolicy&) = delete;
					RangeStateChangePolicy(RangeStateChangePolicy&&) = delete;

					~RangeStateChangePolicy() = default;

					RangeStateChangePolicy& operator=(const RangeStateChangePolicy&) = delete;
					RangeStateChangePolicy& operator=(RangeStateChangePolicy&&) = delete;

					virtual bool SetNext(::estd::remove_cvref_t<TState>& state) const override {
						if (Max == state) {

							return false;
						} else if (Max - Delta < state) {
							state = Max;
						} else {
							state += Delta;
						}

						return true;
					};
					virtual bool SetPrevious(::estd::remove_cvref_t<TState>& state) const override {
						if (state == Min) {

							return false;
						} else if (state < Min + Delta) {
							state = Min;
						} else {
							state -= Delta;
						}

						return true;
					};
					virtual bool Set(::estd::remove_cvref_t<TState>& dst, ::estd::remove_cvref_t<TState>& src) const override {
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
}