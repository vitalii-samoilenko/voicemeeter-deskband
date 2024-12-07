#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "Graphics/ICanvas.h"
#include "IComponent.h"
#include "IScene.h"
#include "Trackers/IDirty.h"
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
				::std::unique_ptr<Trackers::IDirty>& pDirtyTracker,
				::std::unique_ptr<Trackers::IInput>& pInputTracker,
				::std::unique_ptr<Trackers::IFocus>& pFocusTracker,
				::std::unique_ptr<TCanvas>& pCanvas,
				::std::unique_ptr<IComponent>& pComposition
			) : m_pDirtyTracker{ ::std::move(pDirtyTracker) }
			  , m_pInputTracker{ ::std::move(pInputTracker) }
			  , m_pFocusTracker{ ::std::move(pFocusTracker) }
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
				m_pInputTracker->set_Focus(value);
				m_pFocusTracker->set_Focus(value);
			}
			virtual const ::std::valarray<double>& get_Position() const override {
				return m_pCanvas->get_Position();
			};
			virtual const ::std::valarray<double>& get_Size() const override {
				return m_pCanvas->get_Size();
			};
			virtual const ::std::valarray<double>& get_BaseSize() const override {
				return m_pComposition->get_BaseSize();
			};

			virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
				m_pComposition->Redraw(point, vertex);
			};
			virtual void Resize(const ::std::valarray<double>& vertex) override {
				m_pCanvas->Resize(vertex);
				m_pComposition->Rescale(vertex);
			};
			virtual void Rescale(const ::std::valarray<double>& vertex) override {
				m_pComposition->Rescale(vertex);
				m_pCanvas->Resize(m_pComposition->get_Size());
			};
			virtual bool MouseLDown(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseLDown(point)
					|| m_pFocusTracker->MouseLDown(point)
					|| m_pComposition->MouseLDown(point);
			};
			virtual bool MouseLDouble(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseLDouble(point)
					|| m_pFocusTracker->MouseLDouble(point)
					|| m_pComposition->MouseLDouble(point);
			};
			virtual bool MouseMDown(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseMDown(point)
					|| m_pFocusTracker->MouseMDown(point)
					|| m_pComposition->MouseMDown(point);
			};
			virtual bool MouseMDouble(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseMDouble(point)
					|| m_pFocusTracker->MouseMDouble(point)
					|| m_pComposition->MouseMDouble(point);
			};
			virtual bool MouseRDown(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseRDown(point)
					|| m_pFocusTracker->MouseRDown(point)
					|| m_pComposition->MouseRDown(point);
			};
			virtual bool MouseRDouble(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseRDouble(point)
					|| m_pFocusTracker->MouseRDouble(point)
					|| m_pComposition->MouseRDouble(point);
			};
			virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override {
				return m_pInputTracker->MouseWheel(point, delta)
					|| m_pFocusTracker->MouseWheel(point, delta)
					|| m_pComposition->MouseWheel(point, delta);
			};
			virtual bool MouseMove(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseMove(point)
					|| m_pFocusTracker->MouseMove(point)
					|| m_pComposition->MouseMove(point);
			};
			virtual bool MouseLUp(const ::std::valarray<double>& point) override {
				return m_pInputTracker->MouseLUp(point)
					|| m_pFocusTracker->MouseLUp(point)
					|| m_pComposition->MouseLUp(point);
			};
			virtual void Redraw() {
				m_pDirtyTracker->Redraw(
					m_pDirtyTracker->get_Position(),
					m_pDirtyTracker->get_Size()
				);
			}

		protected:
			const ::std::unique_ptr<TCanvas> m_pCanvas;
			const ::std::unique_ptr<Trackers::IDirty> m_pDirtyTracker;

		private:
			const ::std::unique_ptr<Trackers::IInput> m_pInputTracker;
			const ::std::unique_ptr<Trackers::IFocus> m_pFocusTracker;
			const ::std::unique_ptr<IComponent> m_pComposition;
		};
	}
}