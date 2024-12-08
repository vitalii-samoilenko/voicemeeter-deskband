#pragma once

#include <chrono>
#include <type_traits>

#include "Voicemeeter.UI/Trackers/IDirty.h"

#include "../../../Graphics/Glyph.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<typename TScale, typename TGlyph, typename TState>
						class Animation : public TGlyph {
							static_assert(
								::std::is_base_of_v<Graphics::Glyph<TScale>, TGlyph>,
								"TGlyph must be derived from Glyph");

						public:
							Animation() = delete;
							Animation(const Animation&) = delete;
							Animation(Animation&&) = delete;

							Animation& operator=(const Animation&) = delete;
							Animation& operator=(Animation&&) = delete;

							virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
								::std::chrono::high_resolution_clock::time_point now{ ::std::chrono::high_resolution_clock::now() };
								if (m_transition) {
									m_vertex += (::std::chrono::duration_cast<::std::chrono::microseconds>(now - m_point).count() * m_velocity);
									m_vertex[m_vertex < 0LL] = 0LL;
									auto maxVertex = m_baseVertex < m_vertex;
									m_vertex[maxVertex] = m_baseVertex[maxVertex];
									m_velocity[m_vertex == 0LL || m_vertex == m_baseVertex] = 0LL;
									OnFrame();
								}
								m_point = now;
								TGlyph::Redraw(point, vertex);
								if (m_transition = !Finished()) {
									m_dirtyTracker.set_Dirty(*this, true);
								}
							}

						protected:
							template<typename... Args>
							explicit Animation(
								const ::std::valarray<long long>& baseVertex,
								Trackers::IDirty& dirtyTracker,
								Args&&... args
							) : TGlyph{ ::std::forward<Args>(args)... }
							  , m_dirtyTracker{ dirtyTracker }
							  , m_point{}
							  , m_vertex{}
							  , m_baseVertex{ baseVertex }
							  , m_velocity{}
							  , m_transition{} {
								m_vertex.resize(m_baseVertex.size());
								m_velocity.resize(m_baseVertex.size());
							}

							~Animation() = default;

							inline ::std::valarray<long long>& get_Velocity() {
								return m_velocity;
							};
							inline const ::std::valarray<long long>& get_AnimationSize() const {
								return m_vertex;
							};
							inline const ::std::valarray<long long>& get_AnimationBaseSize() const {
								return m_baseVertex;
							};

							virtual void OnFrame() = 0;

						private:
							Trackers::IDirty& m_dirtyTracker;
							::std::chrono::high_resolution_clock::time_point m_point;
							::std::valarray<long long> m_vertex;
							const ::std::valarray<long long> m_baseVertex;
							::std::valarray<long long> m_velocity;
							bool m_transition;

							inline bool Finished() const {
								return (m_velocity == 0LL)
									.min();
							};
						};
					}
				}
			}
		}
	}
}