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
					typename ...TSubSpaces>
				class Animated : public TBundle {
				public:
					template<typename ...Args>
					inline Animated(
						TToolkit &toolkit,
						TSubSpaces &&...subSpaces,
						TFrame &&frame = TFrame{},
						Args &&...args)
						: TBundle{ ::std::forward<Args>(args) ... }
						, _toolkit{ toolkit }
						, _point(Zero, (subSpaces.size() + ...))
						, _vertex{ _point }
						, _subSpaces{ ::std::forward<TSubSpaces>(subSpaces) ... }
						, _frame{ ::std::move(frame) } {

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

					inline void set_AnimationSize(vector_t const &value) {
						if (all(_vertex == value)) {
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
						](TSubSpaces &...subSpaces)->void {
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
							(((distance2 = sum(vector_t{ animationVertex2[subSpaces] }))
							,(rI = traveled2 < distance2
								? sqrt(push(distance2) / traveled2)
								: One)
							,(point[subSpaces] = (
									vector_t{ point[subSpaces] } * (rI - One)
									+ push(vector_t{ vertex[subSpaces] })
								) / rI))
							, ...);
						}, _subSpaces);
						_frame(this);
						TBundle::operator()();
						if (all(_point == _vertex)) {
							return;
						}
						Enqueue();
					};

				private:
					TToolkit &_toolkit;
					vector_t _point;
					vector_t _vertex;
					::std::tuple<TSubSpaces ...> _subSpaces;
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
