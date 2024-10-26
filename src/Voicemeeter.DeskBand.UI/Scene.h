#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "estd/linear_algebra.h"

#include "IComponent.h"
#include "IInputTracker.h"
#include "IScene.h"
#include "Graphics/ICanvas.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			template<class TCanvas>
			class Scene : public IScene {
				static_assert(
					::std::is_base_of_v<Graphics::ICanvas, TCanvas>,
					"TCanvas must be derived from ICanvas");

			public:
				Scene(
					::std::unique_ptr<IInputTracker> inputTracker,
					::std::unique_ptr<TCanvas> pCanvas,
					::std::unique_ptr<IComponent> pComposition
				) : m_inputTracker{ ::std::move(inputTracker) }
				  , m_pCanvas{ ::std::move(pCanvas) }
				  , m_pComposition{ ::std::move(pComposition) } {

				};
				Scene() = delete;
				Scene(const Scene&) = delete;
				Scene(Scene&&) = delete;

				~Scene() = default;

				Scene& operator=(const Scene&) = delete;
				Scene& operator=(Scene&&) = delete;

				virtual const ::linear_algebra::vectord& get_Position() const override {
					return m_pCanvas->get_Position();
				};
				virtual const ::linear_algebra::vectord& get_Size() const override {
					return m_pCanvas->get_Size();
				};

				virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override {
					m_pCanvas->Redraw(point, vertex);
					m_pComposition->Redraw(point, vertex);
				};
				virtual void Resize(const ::linear_algebra::vectord& vertex) override {
					m_pCanvas->Resize(vertex);
					m_pComposition->Rescale(vertex);
				};
				virtual bool MouseLDown(const ::linear_algebra::vectord& point) override {
					return m_inputTracker->MouseLDown(point)
						|| m_pComposition->MouseLDown(point);
				};
				virtual bool MouseLDouble(const ::linear_algebra::vectord& point) override {
					return m_inputTracker->MouseLDouble(point)
						|| m_pComposition->MouseLDouble(point);
				};
				virtual bool MouseRDown(const ::linear_algebra::vectord& point) override {
					return m_inputTracker->MouseRDown(point)
						|| m_pComposition->MouseRDown(point);
				};
				virtual bool MouseWheel(const ::linear_algebra::vectord& point, int delta) override {
					return m_inputTracker->MouseWheel(point, delta)
						|| m_pComposition->MouseWheel(point, delta);
				};
				virtual bool MouseMove(const ::linear_algebra::vectord& point) override {
					return m_inputTracker->MouseMove(point)
						|| m_pComposition->MouseMove(point);
				};
				virtual bool MouseLUp(const ::linear_algebra::vectord& point) override {
					return m_inputTracker->MouseLUp(point)
						|| m_pComposition->MouseLUp(point);
				};

			protected:
				::std::unique_ptr<TCanvas> m_pCanvas;

			private:
				::std::unique_ptr<IInputTracker> m_inputTracker;
				::std::unique_ptr<IComponent> m_pComposition;
			};
		}
	}
}