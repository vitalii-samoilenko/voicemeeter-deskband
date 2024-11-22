#pragma once

#include <chrono>
#include <string>
#include <utility>

#include "Voicemeeter.UI/Trackers/IDirty.h"

#include "../../../Graphics/Glyphs/Plug.h"
#include "../IUpdate.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Adapters {
				namespace Glyph {
					namespace Updates {
						template<size_t Dimension, typename TGlyph, typename TState>
						class Animation : public IUpdate<TGlyph, TState> {
						public:
							Animation() = delete;
							Animation(const Animation&) = delete;
							Animation(Animation&&) = delete;

							Animation& operator=(const Animation&) = delete;
							Animation& operator=(Animation&&) = delete;

							virtual void Update(const TState& state) override {
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
									const ::std::valarray<long long>& aBaseVertex{ get_AnimationBaseSize() };
									auto maxVertex = aBaseVertex < m_vertex;
									m_vertex[maxVertex] = aBaseVertex[maxVertex];
									m_velocity[m_vertex == 0LL || m_vertex == aBaseVertex] = 0LL;
									m_point = now;

									OnFrame();
								}
								IUpdate<TGlyph, TState>::Redraw(point, vertex);
								if (!Finished()) {
									m_dirtyTracker.set_Dirty(*this, true);
								}
							}

						protected:
							template<typename... Args>
							explicit Animation(
								Trackers::IDirty& dirtyTracker,
								Args&&... args
							) : IUpdate<TGlyph, TState>{ ::std::forward<Args>(args)... }
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