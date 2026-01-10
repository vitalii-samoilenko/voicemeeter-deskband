#ifndef VOICEMEETER_UI_ADAPTERS_ANIMATED_HPP
#define VOICEMEETER_UI_ADAPTERS_ANIMATED_HPP

#include <cmath>
#include <tuple>
#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			namespace Graphics {
				template<
					typename TBundle,
					typename TToolkit,
					typename TUpdateFrame,
					typename... TSubSpaces>
				class Animated : public TBundle {
				public:
					template<typename... Args>
					inline Animated(
						TToolkit &toolkit,
						TSubSpaces &&...subSpaces,
						TUpdateFrame &&updateFrame = TUpdateFrame{},
						Args &&...args)
						: TBundle{ ::std::forward<Args>(args)... }
						, _toolkit{ toolkit }
						, _animationPoint(0, (subSpaces.size() + ...))
						, _animationVertex{ _animationPoint }
						, _subSpaces{ ::std::forward<TSubSpaces>(subSpaces)... }
						, _updateFrame{ ::std::move(updateFrame) } {

					};
					Animated() = delete;
					Animated(Animated const &) = delete;
					Animated(Animated &&) = delete;

					inline ~Animated() = default;

					Animated & operator()(Animated const &) = delete;
					Animated & operator()(Animated &&) = delete;

					inline ::std::valarray<int> const & get_AnimationPosition() const {
						return _animationPoint;
					};
					inline ::std::valarray<int> const & get_AnimationSize() const {
						return _animationVertex;
					};

					inline void set_AnimationSize(::std::valarray<int> const &vertex) {
						if (vertex == _animationVertex) {
							return;
						}
						_animationVertex = vertex;
						Enqueue();
					};

					inline void operator()() {
						::std::apply([
							traveled = _toolkit.get_Stopwatch()
								.get_Elapsed(),
							&animationPoint = _animationPoint,
							&animationVertex = _animationVertex
						](TSubSpaces &...subSpaces)->void {
							auto remainingVertex = animationVertex - animationPoint;
							auto remainingVertexQ = remainingVertex * remainingVertex;
							int traveledQ{ traveled * traveled };
							int remainingQ{ 0 };
							(((remainingQ = remainingVertexQ[subSpaces].sum())
							,(animationPoint = traveledQ < remainingQ
								? animationPoint + ::std::sqrt(
									traveledQ * SCALING_FACTOR / remainingQ
									* SCALING_FACTOR) * remainingVertex
									/ SCALING_FACTOR
								: animationVertex))
							, ...);
						}, _subSpaces);
						_updateFrame(*this);
						TBundle::operator()();
						if (_animationPoint == _animationVertex) {
							return;
						}
						Enqueue();
					};

				private:
					TToolkit &_toolkit;
					::std::valarray<int> _animationPoint;
					::std::valarray<int> _animationVertex;
					::std::tuple<TSubSpaces...> _subSpaces;
					TUpdateFrame _updateFrame;

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
