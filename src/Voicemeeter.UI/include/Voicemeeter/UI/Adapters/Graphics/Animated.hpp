#ifndef VOICEMEETER_UI_ADAPTERS_GRAPHICS_ANIMATED_HPP
#define VOICEMEETER_UI_ADAPTERS_GRAPHICS_ANIMATED_HPP

#include <optional>
#include <tuple>
#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Graphics {
				template<
					typename TBundle,
					typename TValue>
				class Context : public TBundle {
				public:
					using context_t = TValue;

					template<typename... Args>
					inline explicit Context(Args &&...args)
						: TBundle{ ::std::forward<Args>(args) ... } {

					};
					Context(Context const &) = delete;
					Context(Context &&) = delete;

					inline ~Context() = default;

					Context & operator=(Context const &) = delete;
					Context & operator=(Context &&) = delete;

					inline context_t const & get_AnimationContext() const {
						return _value;
					};
					inline void set_AnimationContext(context_t const &value) {
						_value = value;
					};

				private:
					::std::optional<context_t> _value;
				};

				template<
					typename TBundle,
					typename TToolkit,
					typename TUpdate,
					typename... TSubSpaces>
				class Animated : public TBundle {
				public:
					template<typename... Args>
					inline Animated(
						TToolkit &toolkit,
						TSubSpaces &&...subSpaces,
						TUpdate &&update = TUpdate{},
						Args &&...args)
						: TBundle{ ::std::forward<Args>(args) ... }
						, _toolkit{ toolkit }
						, _point((subSpaces.size() + ...), 0)
						, _vertex{ _point }
						, _subSpaces{ ::std::forward<TSubSpaces>(subSpaces) ... }
						, _update{ ::std::move(update) } {

					};
					Animated() = delete;
					Animated(Animated const &) = delete;
					Animated(Animated &&) = delete;

					inline ~Animated() = default;

					Animated & operator()(Animated const &) = delete;
					Animated & operator()(Animated &&) = delete;

					inline vector_t const & get_AnimationPosition() const {
						return _point;
					};
					inline vector_t const & get_AnimationSize() const {
						return _vertex;
					};

					inline void set_AnimationSize(vector_t const &vertex) {
						if (_vertex == vertex) {
							return;
						}
						_vertex = vertex;
						Enqueue();
					};

					inline void operator()() {
						::std::apply([
							&toolkit = _toolkit,
							&point = _point,
							&vertex = _vertex
						](TSubSpaces &...subSpaces)->void {
							num_t taveled{
								toolkit.get_Stopwatch()
									.get_Elapsed()
							};
							if (!is_safe(traveled)) {
								point = vertex;
								return;
							}
							num_t traveled2{ traveled * traveled };
							auto animationVertex = vertex - point;
							auto animationVertex2 = animationVertex * animationVertex;
							num_t distance2{ 0 };
							num_t rI{ 0 };
							(((distance2 = sum(animationVertex2[subSpaces]))
							,(rI = traveled2 < distance2
								? sqrt(push(distance2) / traveled2)
								: One)
							,(point[subSpaces] = (
									point[subSpaces] * (rI - One)
									+ push(vertex[subSpaces])
								) / rI))
							, ...);
						}, _subSpaces);
						_update(*this);
						TBundle::operator()();
						if (_point == _vertex) {
							return;
						}
						Enqueue();
					};

				private:
					TToolkit &_toolkit;
					vector_t _point;
					vector_t _vertex;
					::std::tuple<TSubSpaces ...> _subSpaces;
					TUpdate _update;

					inline void Enqueue() {
						TBundle::set_Invalid();
						TBundle::get_Slot()
							.overwrite(*this);
					};
				};
			}
		}
	}
}

#endif
