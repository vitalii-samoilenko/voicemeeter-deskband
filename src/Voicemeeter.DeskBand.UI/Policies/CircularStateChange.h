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
				class CircularStateChange final : public IStateChange<TState> {
					static_assert(
						::std::is_arithmetic_v<::estd::remove_cvref_t<TState>>,
						"TState must be of arithmetic type");

				public:
					CircularStateChange() = default;
					CircularStateChange(const CircularStateChange&) = delete;
					CircularStateChange(CircularStateChange&&) = delete;

					~CircularStateChange() = default;

					CircularStateChange& operator=(const CircularStateChange&) = delete;
					CircularStateChange& operator=(CircularStateChange&&) = delete;

					virtual bool SetDefault(::estd::remove_cvref_t<TState>& state) const override {
						if (state == Default) {
							return false;
						}
						state = Default;
						return true;
					}
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