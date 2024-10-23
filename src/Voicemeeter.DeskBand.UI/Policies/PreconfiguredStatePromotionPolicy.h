#pragma once

#include <utility>

#include "estd/type_traits.h"

#include "IStatePromotionPolicy.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Policies {
				template<typename TState, typename TFunc>
				class PreconfiguredStatePromotionPolicy final : public IStatePromotionPolicy<TState> {
					static_assert(
						::estd::is_invocable_r<void, ::estd::remove_cvref_t<TFunc>, const ::estd::remove_cvref_t<TState>&>(),
						"TFunc must be invocable with const TState& argument and void return type");

				public:
					explicit PreconfiguredStatePromotionPolicy(
						::estd::remove_cvref_t<TFunc>& onPromote
					) : m_onPromote{ ::std::move(onPromote) } {

					}
					PreconfiguredStatePromotionPolicy() = delete;
					PreconfiguredStatePromotionPolicy(const PreconfiguredStatePromotionPolicy&) = delete;
					PreconfiguredStatePromotionPolicy(PreconfiguredStatePromotionPolicy&&) = delete;

					~PreconfiguredStatePromotionPolicy() = default;

					PreconfiguredStatePromotionPolicy& operator=(const PreconfiguredStatePromotionPolicy&) = delete;
					PreconfiguredStatePromotionPolicy& operator=(PreconfiguredStatePromotionPolicy&&) = delete;

					virtual void Promote(const ::estd::remove_cvref_t<TState>& state) const override {
						m_onPromote(state);
					};

				private:
					::estd::remove_cvref_t<TFunc> m_onPromote;
				};
			}
		}
	}
}