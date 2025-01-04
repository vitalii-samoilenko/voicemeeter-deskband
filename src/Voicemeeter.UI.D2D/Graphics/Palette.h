#pragma once

#include <bitset>
#include <chrono>
#include <memory>
#include <unordered_set>

#include <d2d1_3.h>
#include <dcomp.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "Voicemeeter.Atlas/Cherry.h"
#include "Voicemeeter.UI/Direction.h"
#include "Windows/Wrappers.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Graphics {
				class Palette;

				struct Theme final {
					::D2D1::ColorF Background;
					::D2D1::ColorF Warning;
					::D2D1::ColorF Danger;
					::D2D1::ColorF DarkGlass;
					::D2D1::ColorF LightGlass;
					::D2D1::ColorF PrimaryActive;
					::D2D1::ColorF SecondaryActive;
					::D2D1::ColorF Inactive;
					::D2D1::ColorF Indicator;
					::D2D1::ColorF EqualizerLow;
					::D2D1::ColorF EqualizerMedium;
					::D2D1::ColorF EqualizerHigh;

					inline static Theme Dark() {
						return {
							{ 0.F, 0.F, 0.F, 0.F },
							{ 231 / 255.F, 255 / 255.F, 17 / 255.F },
							{ 248 / 255.F, 99 / 255.F, 77 / 255.F },
							{ 0.75F, 0.75F, 0.75F, 0.4F },
							{ 1.F, 1.F, 1.F, 0.4F },
							{ 112 / 255.F, 195 / 255.F, 153 / 255.F },
							{ 104 / 255.F, 230 / 255.F, 250 / 255.F },
							{ 95 / 255.F, 120 / 255.F, 137 / 255.F },
							{ 1., 1., 1. },
							{ 139 / 255.F, 179 / 255.F, 187 / 255.F },
							{ 30 / 255.F, 254 / 255.F, 91 / 255.F },
							{ 1.F, 0.F, 0.F }
						};
					};
					inline static Theme Light() {
						return {
							{ 0.F, 0.F, 0.F, 0.F },
							{ 215 / 255.F, 215 / 255.F, 215 / 255.F },
							{ 248 / 255.F, 99 / 255.F, 77 / 255.F },
							{ 0.75F, 0.75F, 0.75F, 0.4F },
							{ 1.F, 1.F, 1.F, 0.4F },
							{ 112 / 255.F, 195 / 255.F, 153 / 255.F },
							{ 104 / 255.F, 230 / 255.F, 250 / 255.F },
							{ 95 / 255.F, 120 / 255.F, 137 / 255.F },
							{ 215 / 255.F, 215 / 255.F, 215 / 255.F },
							{ 139 / 255.F, 179 / 255.F, 187 / 255.F },
							{ 30 / 255.F, 254 / 255.F, 91 / 255.F },
							{ 1.F, 0.F, 0.F }
						};
					};
				};

				class Atlas final {
				public:
					using Specification = ::Voicemeeter::Atlas::CherrySpecification;

					inline Atlas(
						Palette& palette,
						Direction direction
					) : m_palette{ palette }
					  , m_horizontal{ direction == Direction::Right }
					  , m_pAtlas{ nullptr }
					  , m_pBitmap{ nullptr } {

					};
					Atlas() = delete;
					Atlas(const Atlas&) = delete;
					Atlas(Atlas&&) = delete;

					~Atlas() = default;

					Atlas& operator=(Atlas&&) = delete;
					Atlas& operator=(const Atlas&) = delete;

					inline static D2D1_RECT_F RectF(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) {
						return ::D2D1::RectF(
							static_cast<FLOAT>(point[0]),
							static_cast<FLOAT>(point[1]),
							static_cast<FLOAT>(point[0] + vertex[0] + AAEPS),
							static_cast<FLOAT>(point[1] + vertex[1] + AAEPS));
					};

					inline ID2D1Bitmap1* get_pBitmap() const {
						return m_pBitmap.Get();
					};

					inline ::std::valarray<double> MapPosition(size_t x, size_t y) const {
						return m_pAtlas->MapPosition(x, y);
					};
					inline ::std::valarray<double> MapSize(FLOAT x, FLOAT y) const {
						return m_pAtlas->MapSize(x, y);
					};
					void Rescale(const ::std::valarray<double>& scale);

				private:
					static constexpr double AAEPS{ 0.5 - ::std::numeric_limits<double>::epsilon() };

					Palette& m_palette;
					bool m_horizontal;
					::std::unique_ptr<::Voicemeeter::Atlas::Cherry> m_pAtlas;
					::Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pBitmap;
				};

				class Bundle {
				public:
					Bundle() = delete;
					Bundle(const Bundle&) = delete;
					Bundle(Bundle&&) = delete;

					~Bundle() = default;

					Bundle& operator=(const Bundle&) = delete;
					Bundle& operator=(Bundle&&) = delete;

					inline Palette& get_Palette() {
						return m_palette;
					};
					inline const ::std::valarray<double>& get_Position() const {
						return m_point;
					};
					inline void set_Position(const ::std::valarray<double>& value) {
						m_point = value;
						m_changed.set(position);
					};
					inline const ::std::valarray<double>& get_Size() const {
						return m_vertex;
					};
					inline void set_Size(const ::std::valarray<double>& value) {
						m_vertex = value;
						m_changed.set(size);
					};
					inline const ::std::valarray<double>& get_BaseSize() const {
						return m_baseVertex;
					};

					virtual void Execute() = 0;

				protected:
					enum flags : size_t {
						position = 0,
						size = 1
					};

					::std::bitset<sizeof(size_t)> m_changed;

					inline Bundle(
						Palette& palette,
						const ::std::valarray<double>& baseVertex
					) : m_changed{}
					  , m_palette{ palette }
					  , m_point{ 0., 0. }
					  , m_vertex{ baseVertex }
					  , m_baseVertex{ baseVertex } {

					};

				private:
					Palette& m_palette;
					::std::valarray<double> m_point;
					::std::valarray<double> m_vertex;
					const ::std::valarray<double> m_baseVertex;
				};

				class Timer final {
				public:
					Timer() = default;
					Timer(const Timer&) = delete;
					Timer(Timer&&) = delete;

					~Timer() = default;

					Timer& operator=(const Timer&) = delete;
					Timer& operator=(Timer&&) = delete;

					inline ::std::chrono::nanoseconds get_Elapsed() const {
						return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(m_now - m_past);
					};

					inline void Tick() {
						m_past = m_now;
						m_now = ::std::chrono::high_resolution_clock::now();
					};

				private:
					::std::chrono::high_resolution_clock::time_point m_now;
					::std::chrono::high_resolution_clock::time_point m_past;
				};

				class Queue final {
				public:
					Queue() = default;
					Queue(const Queue&) = delete;
					Queue(Queue&&) = default;

					~Queue() = default;

					Queue& operator=(const Queue&) = delete;
					Queue& operator=(Queue&&) = delete;
					inline Queue& operator+=(Bundle& bundle) {
						m_queue.insert(&bundle);
						return *this;
					};

					inline bool Empty() const {
						return m_queue.empty();
					};

					inline auto begin() const {
						return m_queue.begin();
					};
					inline auto end() const {
						return m_queue.end();
					};

				private:
					::std::unordered_set<Bundle*> m_queue;
				};

				class Palette final {
				public:
					Palette(
						HWND hWnd,
						const Theme& theme,
						Direction direction
					);
					Palette() = delete;
					Palette(const Palette&) = delete;
					Palette(Palette&&) = delete;

					~Palette() = default;

					Palette& operator=(const Palette&) = delete;
					Palette& operator=(Palette&&) = delete;

					inline const Theme& get_Theme() const {
						return m_theme;
					};
					inline Atlas& get_Atlas() {
						return m_atlas;
					};
					inline Timer& get_Timer() {
						return m_timer;
					};
					inline Queue& get_Queue() {
						return m_queue;
					};
					inline ID2D1Factory7* get_pD2dFactory() const {
						return m_pD2dFactory.Get();
					};
					inline ID2D1DeviceContext5* get_pDeviceContext() const {
						return m_pDeviceContext.Get();
					};
					inline IDXGISwapChain1* get_pSwapChain() const {
						return m_pSwapChain.Get();
					};
					inline ID2D1SolidColorBrush* get_pBrush() const {
						return m_pBrush.Get();
					};

					static inline void Blend(::D2D1::ColorF& dst, const ::D2D1::ColorF& src, FLOAT alpha) {
						dst.r = dst.r * (1.F - alpha) + src.r * alpha;
						dst.g = dst.g * (1.F - alpha) + src.g * alpha;
						dst.b = dst.b * (1.F - alpha) + src.b * alpha;
					};

				private:
					const Theme m_theme;
					Atlas m_atlas;
					Timer m_timer;
					Queue m_queue;
					::Microsoft::WRL::ComPtr<ID2D1Factory7> m_pD2dFactory;
					::Microsoft::WRL::ComPtr<ID2D1DeviceContext5> m_pDeviceContext;
					::Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pSwapChain;
					::Microsoft::WRL::ComPtr<IDCompositionTarget> m_pCompositionTarget;
					::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
				};
			}
		}
	}
}