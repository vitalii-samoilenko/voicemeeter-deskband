#ifndef VOICEMEETER_UI_SCENE_HPP
#define VOICEMEETER_UI_SCENE_HPP

#include <memory>
#include <utility>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		template<
			typename TFocusTracker,
			typename TCanvas,
			typename TComposition>
		class Scene {
		public:
			inline Scene(
				::std::unique_ptr<TFocusTracker> &&focusTracker,
				::std::unique_ptr<TCanvas> &&canvas,
				::std::unique_ptr<TComposition> &&composition)
				: _focusTracker{ ::std::move(focusTracker) }
				, _canvas{ ::std::move(canvas) }
				, _composition{ ::std::move(composition) } {

			};
			Scene() = delete;
			Scene(Scene const &) = delete;
			Scene(Scene &&) = delete;

			inline ~Scene() = default;

			Scene & operator=(Scene const &) = delete;
			Scene & operator=(Scene &&) = delete;

			inline ::std::valarray<double> const & get_Position() const {
				return _canvas->get_Position();
			};
			inline ::std::valarray<double> const & get_Size() const {
				return _canvas->get_Size();
			};
			inline ::std::valarray<double> const & get_BaseSize() const {
				return _composition->get_BaseSize();
			};

			inline void Redraw(::std::valarray<double> const & point, ::std::valarray<double> const & vertex) {
				_canvas->Redraw(point, vertex);
			};
			inline void Resize(::std::valarray<double> const & vertex) {
				_canvas->Resize(vertex);
				_composition->Rescale(vertex);
			};
			inline void Rescale(::std::valarray<double> const & vertex) {
				_composition->Rescale(vertex);
				_canvas->Resize(_conposition->get_Size());
			};
			inline void Focus(Focus mode) {
				_focusTracker->Focus(mode);
			};
			inline bool MouseLDown(::std::valarray<double> const & point) {
				return _focusTracker->MouseLDown(point)
					|| _composition->MouseLDown(point);
			};
			inline bool MouseLDouble(::std::valarray<double> const & point) {
				return _focusTracker->MouseLDouble(point)
					|| _composition->MouseLDouble(point);
			};
			inline bool MouseLUp(::std::valarray<double> const & point) {
				return _focusTracker->MouseLUp(point)
					|| _composition->MouseLUp(point);
			};
			inline bool MouseMDown(::std::valarray<double> const & point) {
				return _focusTracker->MouseMDown(point)
					|| _composition->MouseMDown(point);
			};
			inline bool MouseMDouble(::std::valarray<double> const & point) {
				return _focusTracker->MouseMDouble(point)
					|| _composition->MouseMDouble(point);
			};
			inline bool MouseRDown(::std::valarray<double> const & point) {
				return _focusTracker->MouseRDown(point)
					|| _composition->MouseRDown(point);
			};
			inline bool MouseRDouble(::std::valarray<double> const & point) {
				return _focusTracker->MouseRDouble(point)
					|| _composition->MouseRDouble(point);
			};
			inline bool MouseWheel(::std::valarray<double> const & point, int delta) {
				return _focusTracker->MouseWheel(point, delta)
					|| _composition->MouseWheel(point, delta);
			};
			inline bool MouseMove(::std::valarray<double> const & point) {
				return _focusTracker->MouseMove(point)
					|| _composition->MouseMove(point);
			};

		private:
			::std::unique_ptr<TFocusTracker> _focusTracker;
			::std::unique_ptr<TCanvas> _canvas;
			::std::unique_ptr<TComposition> _composition;
		};
	}
}

#endif
