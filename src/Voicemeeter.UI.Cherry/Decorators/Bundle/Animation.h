#pragma once

#include <type_traits>

#include "../../Graphics/Palette.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Decorators {
				namespace Bundle {
					template<
						typename TInstrumentation,
						typename TBundle>
					class Animation : public TBundle {
						using Bundle = Graphics::Bundle<TInstrumentation>;

						static_assert(
							::std::is_base_of_v<Bundle, TBundle>,
							"TBundle must be derived from Bundle");

					public:
						Animation() = delete;
						Animation(const Animation&) = delete;
						Animation(Animation&&) = delete;

						Animation& operator=(const Animation&) = delete;
						Animation& operator=(Animation&&) = delete;

						inline ::std::valarray<long long>& get_Velocity() {
							return m_velocity;
						};
						inline const ::std::valarray<long long>& get_AnimationSize() const {
							return m_vertex;
						};
						inline const ::std::valarray<long long>& get_AnimationBaseSize() const {
							return m_baseVertex;
						};

						virtual void Execute() override {
							if (m_transition) {
								m_vertex += TBundle::get_Palette()
									.get_Timer()
										.get_Elapsed()
											.count() * m_velocity;
								m_vertex[m_vertex < 0LL] = 0LL;
								auto maxVertex = m_baseVertex < m_vertex;
								m_vertex[maxVertex] = m_baseVertex[maxVertex];
								OnFrame();
							}
							TBundle::Execute();
							if (m_transition = (m_velocity != 0LL).max()) {
								TBundle::get_Palette()
									.get_Queue() += *this;
							}
						};

					protected:
						template<typename... Args>
						explicit Animation(
							const ::std::valarray<long long>& baseVertex,
							Args&&... args
						) : TBundle{ ::std::forward<Args>(args)... }
							, m_vertex{}
							, m_baseVertex{ baseVertex }
							, m_velocity{}
							, m_transition{} {
							m_vertex.resize(m_baseVertex.size());
							m_velocity.resize(m_baseVertex.size());
						}

						~Animation() = default;

						virtual void OnFrame() = 0;

					private:
						::std::valarray<long long> m_vertex;
						const ::std::valarray<long long> m_baseVertex;
						::std::valarray<long long> m_velocity;
						bool m_transition;
					};
				}
			}
		}
	}
}