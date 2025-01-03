#pragma once

#include <cmath>
#include <limits>
#include <utility>
#include <valarray>

#include "estd/type_traits.h"

#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <wrl/client.h>

#include "Windows/Wrappers.h"

namespace Voicemeeter {
	namespace Atlas {
		template<typename Specification>
		class Dynamic {
		public:
			Dynamic() = delete;
			Dynamic(const Dynamic&) = delete;
			Dynamic(Dynamic&&) = delete;

			~Dynamic() = default;

			Dynamic& operator=(const Dynamic&) = delete;
			Dynamic& operator=(Dynamic&&) = delete;

			inline IWICBitmap* get_pBitmap() const {
				return m_pBitmap.Get();
			};

			inline ::std::valarray<double> MapPosition(size_t x, size_t y) const {
				return {
					m_blockWidth * x + 1.,
					m_blockHeight * y + 1.
				};
			};
			inline ::std::valarray<double> MapSize(FLOAT x, FLOAT y) const {
				return {
					m_scale.m11 * x,
					m_scale.m22 * y
				};
			};

		protected:
			inline explicit Dynamic(
				const ::std::valarray<double>& scale
			) : m_blockWidth{ ::std::ceil(Specification::Block::Width * ::std::max(static_cast<FLOAT>(scale[0]), ::std::numeric_limits<FLOAT>::epsilon())) }
			  , m_blockHeight{ ::std::ceil(Specification::Block::Height * ::std::max(static_cast<FLOAT>(scale[1]), ::std::numeric_limits<FLOAT>::epsilon())) }
			  , m_scale{ ::D2D1::Matrix3x2F::Scale(static_cast<FLOAT>(scale[0]), static_cast<FLOAT>(scale[1])) }
			  , m_pWicFactory{ nullptr }
			  , m_pBitmap{ nullptr }
			  , m_pDwFactory{ nullptr }
			  , m_pD2dFactory{ nullptr }
			  , m_pRenderTarget{ nullptr } {
				::Windows::ThrowIfFailed(CoInitialize(
					NULL
				), "COM initialization failed");

				::Windows::ThrowIfFailed(CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&m_pWicFactory)
				), "WIC factory creation failed");

				::Windows::ThrowIfFailed(m_pWicFactory->CreateBitmap(
					static_cast<UINT>(m_blockWidth * Specification::Width) + 1U,
					static_cast<UINT>(m_blockHeight * Specification::Height) + 1U,
#ifndef NDEBUG
					GUID_WICPixelFormat32bppPRGBA,
#else
					GUID_WICPixelFormat8bppAlpha,
#endif // !NDEBUG
					WICBitmapCacheOnDemand,
					&m_pBitmap
				), "Bitmap creation failed");

				::Windows::ThrowIfFailed(DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(m_pDwFactory),
					reinterpret_cast<IUnknown**>(m_pDwFactory.ReleaseAndGetAddressOf())
				), "DirectWrite factory creation failed");

				::Windows::ThrowIfFailed(D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED,
#ifndef NDEBUG
					D2D1_FACTORY_OPTIONS{
						D2D1_DEBUG_LEVEL_INFORMATION
					},
#endif // !NDEBUG
					m_pD2dFactory.ReleaseAndGetAddressOf()
				), "Direct2D factory creation failed");

				::Windows::ThrowIfFailed(m_pD2dFactory->CreateWicBitmapRenderTarget(
					m_pBitmap.Get(),
					D2D1_RENDER_TARGET_PROPERTIES{
						D2D1_RENDER_TARGET_TYPE_DEFAULT,
						D2D1_PIXEL_FORMAT{
#ifndef NDEBUG
							DXGI_FORMAT_R8G8B8A8_UNORM,
#else
							DXGI_FORMAT_A8_UNORM,
#endif // !NDEBUG
							D2D1_ALPHA_MODE_PREMULTIPLIED
						},
						0.F, 0.F,
						D2D1_RENDER_TARGET_USAGE_NONE,
						D2D1_FEATURE_LEVEL_DEFAULT
					},
					&m_pRenderTarget
				), "Render target creation failed");

				::Windows::ThrowIfFailed(m_pRenderTarget->CreateSolidColorBrush(
					::D2D1::ColorF(1.F, 1.F, 1.F, 1.F),
					&m_pBrush
				), "Brush creation failed");

				::Windows::ThrowIfFailed(m_pDwFactory->CreateTextFormat(
					Specification::Font::Name,
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					Specification::Font::Size,
					L"", //locale
					&m_pTextFormat
				), "Text format creation failed");

				m_pRenderTarget->BeginDraw();
			};
			template<typename Func, typename... Args,
				::std::enable_if_t<
					::estd::is_invocable_r<void, Func>::value,
				bool> = true>
			inline Dynamic(
				size_t x, size_t y,
				const Func& draw,
				Args&& ...args
			) : Dynamic{ ::std::forward<Args>(args)... } {
				::std::valarray<double> point{ MapPosition(x, y) };
				m_pRenderTarget->SetTransform(
					m_scale
					* ::D2D1::Matrix3x2F::Translation(
						static_cast<FLOAT>(point[0]),
						static_cast<FLOAT>(point[1])));
				draw();
			};

			inline IWICImagingFactory* get_pWicFactory() const {
				return m_pWicFactory.Get();
			};
			inline IDWriteFactory7* get_pDwFactory() const {
				return m_pDwFactory.Get();
			};
			inline ID2D1Factory7* get_pD2dFactory() const {
				return m_pD2dFactory.Get();
			};
			inline ID2D1RenderTarget* get_pRenderTarget() const {
				return m_pRenderTarget.Get();
			};
			inline ID2D1SolidColorBrush* get_pBrush() const {
				return m_pBrush.Get();
			};
			inline IDWriteTextFormat* get_pTextFormat() const {
				return m_pTextFormat.Get();
			};

		private:
			FLOAT m_blockWidth;
			FLOAT m_blockHeight;
			::D2D1::Matrix3x2F m_scale;
			::Microsoft::WRL::ComPtr<IWICImagingFactory> m_pWicFactory;
			::Microsoft::WRL::ComPtr<IWICBitmap> m_pBitmap;
			::Microsoft::WRL::ComPtr<IDWriteFactory7> m_pDwFactory;
			::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
			::Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget;
			::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
			::Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
		};
	}
}