#pragma once

#include <utility>
#include <vector>

#include <windows.h>

#include "Voicemeeter.DeskBand.UI/Graphics/IGlyph.h"

#include "BundleGlyph.h"
#include "Canvas.h"
#include "FrameGlyph.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace UI {
			namespace Graphics {
				namespace D2D {
					class CanvasBuilder final {
					public:
						CanvasBuilder() = default;
						CanvasBuilder(const CanvasBuilder&) = default;
						CanvasBuilder(CanvasBuilder&&) = default;

						~CanvasBuilder() = default;

						CanvasBuilder& operator=(const CanvasBuilder&) = delete;
						CanvasBuilder& operator=(CanvasBuilder&&) = delete;

						inline CanvasBuilder& ForWindow(HWND hWnd) {
							m_hWnd = hWnd;

							return *this;
						};
						inline Canvas* Build() {
							return new Canvas{
								m_hWnd
							};
						};

					private:
						HWND m_hWnd;
					};

					template<typename TBundle>
					class BundleGlyphBuilder final {
					public:
						BundleGlyphBuilder() = default;
						BundleGlyphBuilder(const BundleGlyphBuilder&) = delete;
						BundleGlyphBuilder(BundleGlyphBuilder&&) = delete;

						~BundleGlyphBuilder() = default;

						BundleGlyphBuilder& operator=(const BundleGlyphBuilder&) = delete;
						BundleGlyphBuilder& operator=(BundleGlyphBuilder&&) = delete;

						inline BundleGlyphBuilder& ForCanvas(Canvas& canvas) {
							m_canvas = canvas;

							return *this;
						};
						inline BundleGlyphBuilder& WithBaseSize(const ::linear_algebra::vector& baseVertex) {
							m_baseVertex = baseVertex;

							return *this;
						};
						inline BundleGlyphBuilder& WithBundle(::estd::remove_cvref_t<TBundle>& bundle) {
							m_bundle = bundle;

							return *this;
						};
						template<typename TParam>
						inline BundleGlyph<TBundle, TParam>* Build() {
							return new BundleGlyph{
								m_canvas,
								m_baseVertex,
								m_bundle
							};
						};

					private:
						Canvas& m_canvas;
						const ::linear_algebra::vector& m_baseVertex;
						::estd::remove_cvref_t<TBundle>& m_bundle;
					};

					class FrameGlyphBuilder final {
					public:
						FrameGlyphBuilder() = default;
						FrameGlyphBuilder(const FrameGlyphBuilder&) = delete;
						FrameGlyphBuilder(FrameGlyphBuilder&&) = delete;

						~FrameGlyphBuilder() = default;

						FrameGlyphBuilder& operator=(const FrameGlyphBuilder&) = delete;
						FrameGlyphBuilder& operator=(FrameGlyphBuilder&&) = delete;

						inline FrameGlyphBuilder& WithFrame(::std::unique_ptr<IGlyph> pFrame) {
							m_cpFrame.emplace_back(::std::move(pFrame));

							return *this;
						};
						inline FrameGlyph* Build() {
							return new FrameGlyph{
								m_cpFrame.begin(),
								m_cpFrame.end(),
							};
						};

					private:
						::std::vector<::std::unique_ptr<IGlyph>> m_cpFrame;
					};
				}
			}
		}
	}
}