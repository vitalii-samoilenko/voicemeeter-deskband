#ifndef VOICEMEETER_UI_ADAPTERS_GRAPHICS_ANIMATED_HPP
#define VOICEMEETER_UI_ADAPTERS_GRAPHICS_ANIMATED_HPP

#include <cmath>
#include <optional>
#include <tuple>
#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Graphics {
				template<
					typename TBundle,
					typename TValue>
				class Context : public TBundle {
				public:
					template<typename... Args>
					inline explicit Context(Args &&...args)
						: TBundle{ ::std::forward<Args>(args)... } {

					};
					Context(Context const &) = delete;
					Context(Context &&) = delete;

					inline ~Context() = default;

					Context & operator=(Context const &) = delete;
					Context & operator=(Context &&) = delete;

					inline TValue const & get_AnimationContext() const {
						return _value;
					};
					inline void set_AnimationContext(TValue const &value) {
						_value = value;
					};

				private:
					::std::optional<TValue> _value;
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
						: TBundle{ ::std::forward<Args>(args)... }
						, _toolkit{ toolkit }
						, _point(0, (subSpaces.size() + ...))
						, _vertex{ _point }
						, _subSpaces{ ::std::forward<TSubSpaces>(subSpaces)... }
						, _update{ ::std::move(update) } {

					};
					Animated() = delete;
					Animated(Animated const &) = delete;
					Animated(Animated &&) = delete;

					inline ~Animated() = default;

					Animated & operator()(Animated const &) = delete;
					Animated & operator()(Animated &&) = delete;

					inline ::std::valarray<int> const & get_AnimationPosition() const {
						return _point;
					};
					inline ::std::valarray<int> const & get_AnimationSize() const {
						return _vertex;
					};

					inline void set_AnimationSize(::std::valarray<int> const &vertex) {
						if (vertex == _vertex) {
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
							int taveled{
								toolkit.get_Stopwatch()
									.get_Elapsed()
							};
							if (HALF_LIFE < traveled) {
								point = vertex;
								return;
							}
							int traveled2{ traveled * traveled };
							auto animationVertex = vertex - point;
							auto animationVertex2 = animationVertex * animationVertex;
							int distance2{ 0 };
							int r{ 0 };
							int rI{ 0 };
							(((distance2 = animationVertex2[subSpaces].sum())
							,(r = traveled2 < distance2
								? ::std::sqrt(
									traveled2 * SCALING_FACTOR / distance2
									* SCALING_FACTOR)
								: SCALING_FACTOR)
							,(rI = SCALING_FACTOR - r)
							,(point[subSpaces] = (point[subSpaces] * rI
								+ vertex[subSpaces] * r) / SCALING_FACTOR))
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
					static constexpr int HALF_LIFE{ 46340 };

					TToolkit &_toolkit;
					::std::valarray<int> _point;
					::std::valarray<int> _vertex;
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
