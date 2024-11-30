#pragma once

#include <type_traits>
#include <utility>

#include "../../Multiclient/Manager.h"
#include "../../Strip.h"
#include "Line.h"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename Specification, typename StripSpecification, typename TLine>
			class Strip : private ::Voicemeeter::Strip<StripSpecification, TLine> {
				using Base = ::Voicemeeter::Strip<StripSpecification, TLine>;
				using Manager = ::Voicemeeter::Multiclient::Manager<Specification>;

				static_assert(
					::std::is_base_of_v<Line<Specification>, TLine>,
					"TLine must be derived from Line");

			public:
				template<typename... Args>
				inline Strip(
					const Manager& manager,
					size_t id,
					Args&& ...args
				) : Base{ ::std::forward<Args>(args)... }
				  , m_manager{ manager }
				  , m_id{ id } {

				};
				Strip() = delete;
				Strip(const Strip&) = delete;
				inline Strip(Strip&&) = default;

				inline ~Strip() = default;

				Strip& operator=(const Strip&) = delete;
				Strip& operator=(Strip&&) = delete;

				inline size_t get_Id() const {
					return m_id;
				};
				using Base::get_Gain;
				template<typename TClient>
				inline void set_Gain(double value) {
					if (::std::abs(Base::get_Gain() - value) < 0.01) {
						return;
					}
					Base::set_Gain(value);
					for (auto& client_subscription : m_manager) {
						if (client_subscription->first == typeid(TClient)) {
							continue;
						}
						client_subscription->second
							.on_Gain(m_id)
								(value);
					}
				};
				using Base::get_Mute;
				template<typename TClient>
				inline void set_Mute(bool value) {
					if (Base::get_Mute() == value) {
						return;
					}
					Base::set_Mute(value);
					for (auto& client_subscription : m_manager) {
						if (client_subscription->first == typeid(TClient)) {
							continue;
						}
						client_subscription->second
							.on_Mute(m_id)
								(value);
					}
				};

				using Base::begin;
				using Base::end;

			private:
				const Manager& m_manager;
				size_t m_id;
			};
		}
	}
}