#pragma once

#include <memory>

#include "estd/linear_algebra.h"

#include "Voicemeeter.DeskBand.UI/Graphics/IGlyph.h"

#include "../Canvas.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					class InstantRendering final : public IGlyph {
					public:
						InstantRendering(
							Canvas& pCanvas,
							::std::unique_ptr<IGlyph> pGlyph
						);
						InstantRendering(const InstantRendering&) = delete;
						InstantRendering(InstantRendering&&) = delete;

						~InstantRendering() = default;

						InstantRendering& operator=(const InstantRendering&) = delete;
						InstantRendering& operator=(InstantRendering&&) = delete;

						virtual const ::linear_algebra::vectord& get_Position() const override final;
						virtual const ::linear_algebra::vectord& get_Size() const override final;
						virtual const ::linear_algebra::vectord& get_BaseSize() const override final;

						virtual void Redraw(const ::linear_algebra::vectord& point, const ::linear_algebra::vectord& vertex) override final;
						virtual void Move(const ::linear_algebra::vectord& point) override final;
						virtual void Rescale(const ::linear_algebra::vectord& vertex) override final;

					private:
						Canvas& m_canvas;
						::std::unique_ptr<IGlyph> m_pGlyph;
					};
				}
			}
		}
	}
}