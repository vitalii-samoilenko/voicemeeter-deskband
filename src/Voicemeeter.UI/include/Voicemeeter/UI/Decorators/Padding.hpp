#ifndef VOICEMEETER_UI_DECORATORS_PADDING_HPP
#define VOICEMEETER_UI_DECORATORS_PADDING_HPP

#include <tuple>
#include <utility>

#include "wheel.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Decorators {
			template<
				typename TComponent,
				typename TScale>
			class Padding : public TComponent {
			public:
				template<typename... Args>
				inline Padding(
					vector_t &&basePaddingPoint,
					vector_t &&basePaddingVertex,
					TScale &&scale = TScale{},
					Args &&...args)
					: TComponent{ ::std::forward<Args>(args) ... }
					, _point{ 0, 0 }
					, _vertex{ 0, 0 }
					, _baseVertex{ basePaddingPoint + TComponent::get_BaseSize() + basePaddingVertex }
					, _paddingPoint{}
					, _paddingVertex{}
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

				inline vector_t const & get_Position() const {
					return _point;
				};
				inline vector_t const & get_Size() const {
					return _vertex;
				};
				inline vector_t const & get_BaseSize() const {
					return _baseVertex;
				};

				inline void Rescale(vector_t const &vertex) {
					::std::tie(_paddingPoint, ::std::ignore, _paddingVertex) = _scale(vertex,
							_basePaddingPoint, TComponent::get_BaseSize(), _basePaddingVertex);
					TComponent::Move(_point + _paddingPoint);
					TComponent::Rescale(vertex - _paddingPoint - _paddingVertex);
					_vertex = _paddingPoint + TComponent::get_Size() + _paddingVertex;
				};
				inline void Move(vector_t const &point) {
					_point = point;
					TComponent::Move(point + _paddingPoint);
				};

			private:
				vector_t _point;
				vector_t _vertex;
				vector_t _baseVertex;
				vector_t _paddingPoint;
				vector_t _paddingVertex;
				vector_t _basePaddingPoint;
				vector_t _basePaddingVertex;
				TScale _scale;
			};
		}
	}
}

#endif
