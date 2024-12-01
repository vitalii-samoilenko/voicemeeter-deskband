#pragma once

#include <cmath>

#include "../../Line.h"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename Specification>
			class Line : private ::Voicemeeter::Line {
				using Base = ::Voicemeeter::Line;
				using Manager = ::Voicemeeter::Multiclient::Manager<Specification>;

			public:
				inline Line(
					const Manager& manager,
					size_t id
				) : Base{}
				  , m_manager{ manager }
				  , m_id{ id } {

				};
				Line() = delete;
				Line(const Line&) = delete;
				inline Line(Line&&) = default;

				inline ~Line() = default;

				Line& operator=(const Line&) = delete;
				Line& operator=(Line&&) = delete;

				inline size_t get_Id() const {
					return m_id;
				};
				using Base::get_Level;
				template<typename TClient>
				inline void set_Level(double value) {
					if (::std::abs(Base::get_Level() - value) < 0.0001) {
						return;
					}
					Base::set_Level(value);
					for (const auto& client_subscription : m_manager) {
						if (client_subscription->first == &typeid(TClient)) {
							continue;
						}
						client_subscription->second
							.on_Level(m_id)
								(value);
					}
				};

			private:
				const Manager& m_manager;
				size_t m_id;
			};
		}
	}
}