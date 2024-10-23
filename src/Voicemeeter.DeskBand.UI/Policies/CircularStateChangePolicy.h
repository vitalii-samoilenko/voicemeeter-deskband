#pragma once

#include <type_traits>

#include "estd/type_traits.h"

#include "IStateChangePolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState, TState Min, TState Max, TState Delta>
				class CircularStateChangePolicy final : public IStateChangePolicy<TState> {
					static_assert(
						::std::is_arithmetic_v<TState>,
						"TState must be of arithmetic type"));

				public:
					CircularStateChangePolicy() = default;
					CircularStateChangePolicy(const CircularStateChangePolicy&) = delete;
					CircularStateChangePolicy(CircularStateChangePolicy&&) = delete;

					~CircularStateChangePolicy() = default;

					CircularStateChangePolicy& operator=(const CircularStateChangePolicy&) = delete;
					CircularStateChangePolicy& operator=(CircularStateChangePolicy&&) = delete;

					virtual bool SetNext(::estd::remove_cvref_t<TState>& state) const override {
						if (Max - Delta < state) {
							state = Min;
						} else {
							state += Delta;
						}

						return true;
					};
					virtual bool SetPrevious(::estd::remove_cvref_t<TState>& state) const override {
						if (state < Min + Delta) {
							state = Max;
						} else {
							state -= Delta;
						}

						return true;
					};
					virtual bool Set(::estd::remove_cvref_t<TState>& dst, ::estd::remove_cvref_t<TState>& src) const override {
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