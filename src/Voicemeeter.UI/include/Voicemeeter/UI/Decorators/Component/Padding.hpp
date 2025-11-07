#ifndef VOICEMEETER_UI_DECORATORS_COMPONENT_PADDING_HPP
#define VOICEMEETER_UI_DECORATORS_COMPONENT_PADDING_HPP

#include <tuple>
#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			namespace Component {
				template<
					typename TComponent,
					typename TScale>
				class Padding : public TComponent {
				public:
					template<typename... Args>
					inline Padding(
						::std::valarray<int> &&basePaddingPoint,
						::std::valarray<int> &&basePaddingVertex,
						TScale &&scale = TScale{},
						Args &&...args)
						: TComponent{ ::std::forward<Args>(args)... }
						, _point{ 0, 0 }
						, _vertex{ basePaddingPoint + TComponent::get_BaseSize() + basePaddingVertex }
						, _baseVertex{ _vertex }
						, _paddingPoint{ basePaddingPoint }
						, _paddingVertex{ basePaddingVertex }
						, _basePaddingPoint{ ::std::move(basePaddingPoint) }
						, _basePaddingVertex{ ::std::move(basePaddingVertex) }
						, _scale{ ::std::move(scale) } {

					};
					Padding() = delete;
					Padding(Padding const &) = delete;
					Padding(Padding &&) = delete;

					inline ~Padding() = default;

					Padding & operator=(Padding const &) = delete;
					Padding & operator=(Padding &&) = delete;

					inline ::std::valarray<int> const & get_Position() const {
						return _point;
					};
					inline ::std::valarray<int> const & get_Size() const {
						return _vertex;
					};
					inline ::std::valarray<int> const & get_BaseSize() const {
						return _baseVertex;
					};

					inline void Rescale(::std::valarray<int> const &vertex) {
						::std::tie(_paddingPoint, ::std::ignore, _paddingVertex) =
							_scale(vertex, _basePaddingPoint, TComponent::get_BaseSize(), _basePaddingVertex);
						TComponent::Move(_point + _paddingPoint);
						TComponent::Rescale(vertex - _paddingPoint - _paddingVertex);
						_vertex = _paddingPoint + TComponent::get_Size() + _paddingVertex;
					};
					inline void Move(::std::valarray<int> const &point) {
						_point = point;
						TComponent::Move(point + _paddingPoint);
					};

				private:
					::std::valarray<int> _point;
					::std::valarray<int> _vertex;
					::std::valarray<int> _baseVertex;
					::std::valarray<int> _paddingPoint;
					::std::valarray<int> _paddingVertex;
					::std::valarray<int> _basePaddingPoint;
					::std::valarray<int> _basePaddingVertex;
					TScale _scale;
				};
			}
		}
	}
}

#endif
