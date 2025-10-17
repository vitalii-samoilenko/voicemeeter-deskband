#ifndef VOICEMEETER_UI_DECORATORS_COMPONENT_PADDING_HPP
#define VOICEMEETER_UI_DECORATORS_COMPONENT_PADDING_HPP

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
						::std::valarray<double> &&basePaddingPoint,
						::std::valarray<double> &&basePaddingVertex,
						TScale &&scale = TScale{},
						Args &&...args)
						: TComponent{ ::std::forward<Args>(args)... }
						, _position{ TComponent::get_Position() }
						, _vertex{ TComponent::get_BaseSize() + basePaddingPoint + basePaddingVertex }
						, _baseVertex{ _vertex }
						, _paddingPoint{ basePaddingPoint }
						, _paddingVertex{ basePaddingVertex }
						, _basePaddingPoint{ ::std::move(basePaddingPoint) }
						, _basePaddingVertex{ ::std::move(basePaddingVertex) }
						, _scale{ ::std::move(scale) } {
						TComponent::Move(_position + _basePaddingPoint);
					};
					Padding() = delete;
					Padding(Padding const &) = delete;
					Padding(Padding &&) = delete;

					inline ~Padding() = default;

					Padding & operator=(Padding const &) = delete;
					Padding & operator=(Padding &&) = delete;

					inline ::std::valarray<double> const & get_Position() const {
						return _position;
					};
					inline ::std::valarray<double> const & get_Size() const {
						return _vertex;
					};
					inline ::std::valarray<double> const & get_BaseSize() const {
						return _baseVertex;
					};

					inline void Rescale(::std::valarray<double> const &vertex) {
						::std::valarray<double> scale{ _scale(_baseVertex, vertex) };
						_paddingPoint = scale * _basePaddingPoint;
						_paddingVertex = scale * _basePaddingVertex;
						TComponent::Move(_position + _paddingPoint);
						TComponent::Rescale(vertex - _paddingPoint - _paddingVertex);
						_vertex = TComponent::get_Size() + _paddingPoint + _paddingVertex;
					};
					inline void Move(::std::valarray<double> const &point) {
						_position = point;
						TComponent::Move(point + _paddingPoint);
					};

				private:
					::std::valarray<double> _position;
					::std::valarray<double> _vertex;
					::std::valarray<double> _baseVertex;
					::std::valarray<double> _paddingPoint;
					::std::valarray<double> _paddingVertex;
					::std::valarray<double> _basePaddingPoint;
					::std::valarray<double> _basePaddingVertex;
					TScale _scale;
				};
			}
		}
	}
}

#endif
