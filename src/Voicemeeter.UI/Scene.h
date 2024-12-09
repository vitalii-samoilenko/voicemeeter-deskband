#pragma once

#include <memory>

#include "Graphics/ICanvas.h"
#include "IComponent.h"
#include "IScene.h"
#include "Trackers/Focus.h"
#include "Trackers/Input.h"

namespace Voicemeeter {
	namespace UI {
		class Scene : public IScene {
		public:
			Scene(
				::std::unique_ptr<Trackers::Input>& pInputTracker,
				::std::unique_ptr<Trackers::Focus>& pFocusTracker,
				::std::unique_ptr<Graphics::ICanvas>& pCanvas,
				::std::unique_ptr<IComponent>& pComposition
			);
			Scene() = delete;
			Scene(const Scene&) = delete;
			Scene(Scene&&) = delete;

			~Scene() = default;

			Scene& operator=(const Scene&) = delete;
			Scene& operator=(Scene&&) = delete;

			virtual void set_Focus(bool value) override;
			virtual const ::std::valarray<double>& get_Position() const override;
			virtual const ::std::valarray<double>& get_Size() const override;
			virtual const ::std::valarray<double>& get_BaseSize() const override;

			virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;
			virtual void Resize(const ::std::valarray<double>& vertex) override;
			virtual void Rescale(const ::std::valarray<double>& vertex) override;
			virtual bool MouseLDown(const ::std::valarray<double>& point) override;
			virtual bool MouseLDouble(const ::std::valarray<double>& point) override;
			virtual bool MouseMDown(const ::std::valarray<double>& point) override;
			virtual bool MouseMDouble(const ::std::valarray<double>& point) override;
			virtual bool MouseRDown(const ::std::valarray<double>& point) override;
			virtual bool MouseRDouble(const ::std::valarray<double>& point) override;
			virtual bool MouseWheel(const ::std::valarray<double>& point, int delta) override;
			virtual bool MouseMove(const ::std::valarray<double>& point) override;
			virtual bool MouseLUp(const ::std::valarray<double>& point) override;

		private:
			const ::std::unique_ptr<Graphics::ICanvas> m_pCanvas;
			const ::std::unique_ptr<Trackers::Input> m_pInputTracker;
			const ::std::unique_ptr<Trackers::Focus> m_pFocusTracker;
			const ::std::unique_ptr<IComponent> m_pComposition;
		};
	}
}