#pragma once

#include "estd/type_traits.h"

#include "Voicemeeter.UI/Graphics/IGlyph.h"
#include "Windows/Wrappers.h"

#include "../Trackers/Dirty.h"
#include "Palette.h"

using namespace ::Voicemeeter::UI::Graphics;

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				template<typename TScale>
				class Glyph : public IGlyph {
					static_assert(
						::estd::is_invocable_r<::std::valarray<double>, TScale, const ::std::valarray<double>&, const ::std::valarray<double>&>(),
						"TScale must be invocable with const valarray<double>&, const valarray<double>& arguments and valarray<double> return type");

				public:
					Glyph() = delete;
					Glyph(const Glyph&) = delete;
					Glyph(Glyph&&) = delete;

					~Glyph() = default;

					Glyph& operator=(const Glyph&) = delete;
					Glyph& operator=(Glyph&&) = delete;

					virtual const ::std::valarray<double>& get_Position() const override {
						return m_point;
					};
					virtual const ::std::valarray<double>& get_Size() const override {
						return m_vertex;
					};
					virtual const ::std::valarray<double>& get_BaseSize() const override {
						return m_baseVertex;
					};

					virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override {
						m_palette.get_pD2dDeviceContext()
							->SetTransform(::D2D1::IdentityMatrix());
						m_palette.get_pD2dDeviceContext()
							->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);
						m_palette.get_pD2dDeviceContext()
							->FillRectangle(
								::D2D1::RectF(
									static_cast<FLOAT>(point[0]),
									static_cast<FLOAT>(point[1]),
									static_cast<FLOAT>(point[0] + vertex[0]),
									static_cast<FLOAT>(point[1] + vertex[1])
								),
								m_palette.get_pBrush<Palette>(
									[this](ID2D1Brush** ppBrush)->void {
										ID2D1SolidColorBrush* pBrush{ nullptr };
										::Windows::ThrowIfFailed(m_palette.get_pD2dDeviceContext()
											->CreateSolidColorBrush(
												m_palette.get_Theme()
												.Background,
												&pBrush
											), "Brush creation failed");
										*ppBrush = pBrush;
									}));
						m_palette.get_pD2dDeviceContext()
							->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
						m_palette.get_pD2dDeviceContext()
							->SetTransform(
								::D2D1::Matrix3x2F::Scale(
									static_cast<FLOAT>(m_vertex[0] / m_baseVertex[0]),
									static_cast<FLOAT>(m_vertex[1] / m_baseVertex[1]))
								* ::D2D1::Matrix3x2F::Translation(
									static_cast<FLOAT>(m_point[0]),
									static_cast<FLOAT>(m_point[1])));
					};
					virtual void Move(const ::std::valarray<double>& point) override {
						m_point = point;
						m_dirtyTracker.set_Dirty(*this, true);
					};
					virtual void Rescale(const ::std::valarray<double>& vertex) override {
						m_vertex = m_baseVertex * m_scale(m_baseVertex, vertex);
						m_dirtyTracker.set_Dirty(*this, true);
					};

				protected:
					Glyph(
						const Palette& palette,
						Trackers::Dirty& dirtyTracker,
						const ::std::valarray<double>& baseVertex,
						const TScale& scale = {}
					) : m_palette{ palette }
					  , m_dirtyTracker{ dirtyTracker }
					  , m_point{ 0., 0. }
					  , m_vertex{ baseVertex }
					  , m_baseVertex{ baseVertex }
					  , m_scale{ scale } {

					};

					inline const Palette& get_Palette() const {
						return m_palette;
					};
					inline Trackers::Dirty& get_DirtyTracker() {
						return m_dirtyTracker;
					};

				private:
					const Palette& m_palette;
					Trackers::Dirty& m_dirtyTracker;
					::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					const ::std::valarray<double> m_baseVertex;
					const TScale m_scale;
				};
			}
		}
	}
}