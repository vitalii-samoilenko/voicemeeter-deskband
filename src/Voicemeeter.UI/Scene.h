#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "Graphics/ICanvas.h"
#include "IComponent.h"
#include "IScene.h"
#include "Trackers/IFocus.h"
#include "Trackers/IInput.h"

namespace Voicemeeter {
	namespace UI {
		template<typename TCanvas>
		class Scene : public IScene {
			static_assert(
				::std::is_base_of_v<Graphics::ICanvas, TCanvas>,
				"TCanvas must be derived from ICanvas");

		public:
			Scene(
				::std::unique_ptr<Trackers::IInput>& inputTracker,
				::std::unique_ptr<Trackers::IFocus>& focusTracker,
				::std::unique_ptr<TCanvas>& pCanvas,
				::std::unique_ptr<IComponent>& pComposition
			) : m_inputTracker{ ::std::move(inputTracker) }
			  , m_focusTracker{ ::std::move(focusTracker) }
			  , m_pCanvas{ ::std::move(pCanvas) }
			  , m_pComposition{ ::std::move(pComposition) } {

			};
			Scene() = delete;
			Scene(const Scene&) = delete;
			Scene(Scene&&) = delete;

			~Scene() = default;

			Scene& operator=(const Scene&) = delete;
			Scene& operator=(Scene&&) = delete;

			virtual void set_Focus(bool value) override {
				m_inputTracker->set_Focus(value);
				m_focusTracker->set_Focus(value);
			}
			virtual const ::std::valarray<double>& get_Position() const override {
				return m_pCanvas->get_Position();
			};
			virtual const ::std::valarray<double>& get_Size() const override {
				return m_pCanvas->get_Size();
			};

			virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
				m_pCanvas->Redraw(point, vertex);
				m_pComposition->Redraw(point, vertex);
			};
			virtual void Resize(const ::std::valarray<double>& vertex) override {
				m_pCanvas->Resize(vertex);
				m_pComposition->Rescale(vertex);
			};
			virtual bool MouseLDown(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseLDown(point)
					|| m_focusTracker->MouseLDown(point)
					|| m_pComposition->MouseLDown(point);
			};
			virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseLDouble(point)
					|| m_focusTracker->MouseLDouble(point)
					|| m_pComposition->MouseLDouble(point);
			};
			virtual bool MouseMDown(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseMDown(point)
					|| m_focusTracker->MouseMDown(point)
					|| m_pComposition->MouseMDown(point);
			};
			virtual bool MouseMDouble(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseMDouble(point)
					|| m_focusTracker->MouseMDouble(point)
					|| m_pComposition->MouseMDouble(point);
			};
			virtual bool MouseRDown(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseRDown(point)
					|| m_focusTracker->MouseRDown(point)
					|| m_pComposition->MouseRDown(point);
			};
			virtual bool MouseRDouble(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseRDouble(point)
					|| m_focusTracker->MouseRDouble(point)
					|| m_pComposition->MouseRDouble(point);
			};
			virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
				return m_inputTracker->MouseWheel(point, delta)
					|| m_focusTracker->MouseWheel(point, delta)
					|| m_pComposition->MouseWheel(point, delta);
			};
			virtual bool MouseMove(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseMove(point)
					|| m_focusTracker->MouseMove(point)
					|| m_pComposition->MouseMove(point);
			};
			virtual bool MouseLUp(const ::std::valarray<double>& point) override {
				return m_inputTracker->MouseLUp(point)
					|| m_focusTracker->MouseLUp(point)
					|| m_pComposition->MouseLUp(point);
			};

		protected:
			::std::unique_ptr<TCanvas> m_pCanvas;

		private:
			::std::unique_ptr<Trackers::IInput> m_inputTracker;
			::std::unique_ptr<Trackers::IFocus> m_focusTracker;
			::std::unique_ptr<IComponent> m_pComposition;
		};
	}
}