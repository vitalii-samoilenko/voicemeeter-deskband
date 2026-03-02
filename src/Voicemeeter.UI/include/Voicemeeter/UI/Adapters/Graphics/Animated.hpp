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

					template<typename ...Args>
					inline explicit Context(Args &&...args)
						: TBundle{ ::std::forward<Args>(args) ... }
						, _context{} {

					};
					Context(Context const &) = delete;
					Context(Context &&) = delete;

					inline ~Context() = default;

					Context & operator=(Context const &) = delete;
					Context & operator=(Context &&) = delete;

					inline context_t const & get_AnimationContext() const {
						return _context.value();
					};
					inline void set_AnimationContext(context_t const &value) {
						_context = value;
					};

				private:
					::std::optional<context_t> _context;
				};

				template<
					typename TBundle,
					typename TToolkit,
					typename TFrame,
					typename ...Is>
				class Animated : public TBundle {
				public:
					template<typename ...Args>
					inline Animated(
						TToolkit &toolkit,
						Is &&...is,
						TFrame &&frame = TFrame{},
						Args &&...args)
						: TBundle{ ::std::forward<Args>(args) ... }
						, _toolkit{ toolkit }
						, _point(Zero, (size(is) + ...))
						, _vertex{ _point }
						, _is{ ::std::forward<Is>(is) ... }
						, _frame{ ::std::move(frame) } {

					};
					Animated() = delete;
					Animated(Animated const &) = delete;
					Animated(Animated &&) = delete;

					inline ~Animated() = default;

					Animated & operator()(Animated const &) = delete;
					Animated & operator()(Animated &&) = delete;

					inline vec_t const & get_AnimationPosition() const {
						return _point;
					};
					inline vec_t const & get_AnimationSize() const {
						return _vertex;
					};

					inline void set_AnimationSize(vec_t const &value) {
						if (are_all(_vertex == value)) {
							return;
						}
						_vertex = value;
						Enqueue();
					};

					inline void operator()() {
						::std::apply([
							&toolkit = _toolkit,
							&point = _point,
							&vertex = _vertex
						](Is &...is)->void {
							num_t traveled{
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
							(((distance2 = sum(sub(animationVertex2, is)))
							,(rI = traveled2 < distance2
								? sqrt(push(distance2) / traveled2)
								: One)
							,(sub(&point, is) = (
									sub(point, is) * (rI - One)
									+ push(sub(vertex, is))
								) / rI))
							, ...);
						}, _is);
						_frame(this);
						TBundle::operator()();
						if (are_all(_point == _vertex)) {
							return;
						}
						Enqueue();
					};

				private:
					TToolkit &_toolkit;
					vec_t _point;
					vec_t _vertex;
					::std::tuple<Is ...> _is;
					TFrame _frame;

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
