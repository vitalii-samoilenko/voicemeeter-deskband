#pragma once

#include <type_traits>

#include "estd/type_traits.h"

#include "IStateChange.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState,
					::estd::remove_cvref_t<TState> Default,
					::estd::remove_cvref_t<TState> Min,
					::estd::remove_cvref_t<TState> Max,
					::estd::remove_cvref_t<TState> Delta>
				class RangeStateChange final : public IStateChange<TState> {
					static_assert(
						::std::is_arithmetic_v<::estd::remove_cvref_t<TState>>,
						"TState must be of arithmetic type");

				public:
					RangeStateChange() = default;
					RangeStateChange(const RangeStateChange&) = delete;
					RangeStateChange(RangeStateChange&&) = delete;

					~RangeStateChange() = default;

					RangeStateChange& operator=(const RangeStateChange&) = delete;
					RangeStateChange& operator=(RangeStateChange&&) = delete;

					virtual bool SetDefault(::estd::remove_cvref_t<TState>& state) const override {
						if (state == Default) {
							return false;
						}
						state = Default;
						return true;
					}
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