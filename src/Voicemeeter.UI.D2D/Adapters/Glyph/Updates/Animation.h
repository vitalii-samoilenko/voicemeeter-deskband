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
						template<size_t Dimension, typename TScale, typename TGlyph, typename TState>
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

							void Update(const TState& state) {
								if (Finished()) {
									m_point = ::std::chrono::high_resolution_clock::now();
								}

								OnUpdate(state);
							};
							virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
								if (!Finished()) {
									::std::chrono::high_resolution_clock::time_point now{ ::std::chrono::high_resolution_clock::now() };
									m_vertex += (::std::chrono::duration_cast<::std::chrono::microseconds>(now - m_point).count() * m_velocity);
									m_vertex[m_vertex < 0LL] = 0LL;
									const ::std::valarray<long long>& animationBaseVertex{ get_AnimationBaseSize() };
									auto maxVertex = animationBaseVertex < m_vertex;
									m_vertex[maxVertex] = animationBaseVertex[maxVertex];
									m_velocity[m_vertex == 0LL || m_vertex == animationBaseVertex] = 0LL;
									m_point = now;

									OnFrame();
								}
								TGlyph::Redraw(point, vertex);
								if (!Finished()) {
									m_dirtyTracker.set_Dirty(*this, true);
								}
							}

						protected:
							template<typename... Args>
							explicit Animation(
								Trackers::IDirty& dirtyTracker,
								Args&&... args
							) : TGlyph{ ::std::forward<Args>(args)... }
							  , m_dirtyTracker{ dirtyTracker }
							  , m_point{}
							  , m_vertex{}
							  , m_velocity{} {
								m_vertex.resize(Dimension);
								m_velocity.resize(Dimension);
							}

							~Animation() = default;

							inline ::std::valarray<long long>& get_Velocity() {
								return m_velocity;
							}
							inline const ::std::valarray<long long>& get_AnimationSize() const {
								return m_vertex;
							}
							virtual const ::std::valarray<long long>& get_AnimationBaseSize() const = 0;

							virtual void OnUpdate(const TState& state) = 0;
							virtual void OnFrame() = 0;

						private:
							Trackers::IDirty& m_dirtyTracker;
							::std::chrono::high_resolution_clock::time_point m_point;
							::std::valarray<long long> m_vertex;
							::std::valarray<long long> m_velocity;

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