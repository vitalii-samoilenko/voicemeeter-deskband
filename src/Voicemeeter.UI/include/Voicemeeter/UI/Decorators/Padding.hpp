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
					, _paddingPoint{ 0, 0 }
					, _paddingVertex{ 0, 0 }
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
					return TComponent::get_Position() - _paddingPoint;
				};
				inline vector_t const & get_Size() const {
					return _paddingPoint + TComponent::get_Size() + _paddingVertex;
				};
				inline vector_t const & get_BaseSize() const {
					return _basePaddingPoint + TComponent::get_BaseSize() + _basePaddingVertex;
				};

				inline void Rescale(vector_t const &vertex) {
					vector_t point{ get_Position() };
					::std::tie(_paddingPoint, ::std::ignore, _paddingVertex) = _scale(vertex,
							_basePaddingPoint, TComponent::get_BaseSize(), _basePaddingVertex);
					TComponent::Rescale(vertex - _paddingPoint - _paddingVertex);
					Move(point);
				};
				inline void Move(vector_t const &point) {
					TComponent::Move(point + _paddingPoint);
				};

			private:
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
