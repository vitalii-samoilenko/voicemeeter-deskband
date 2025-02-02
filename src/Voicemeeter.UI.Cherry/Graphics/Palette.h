#pragma once

#include <bitset>
#include <chrono>
#include <memory>
#include <unordered_set>
#include <utility>

#include "Voicemeeter.Atlas/Cherry.h"

namespace Voicemeeter {
	namespace UI {
		namespace Cherry {
			namespace Graphics {
				template<typename TInstrumentation>
				class Palette;

				struct Theme final {
					enum RGBA : size_t {
						red = 0,
						green = 1,
						blue = 2,
						alpha = 3
					};
					
					::std::valarray<double> Background;
					::std::valarray<double> Warning;
					::std::valarray<double> Danger;
					::std::valarray<double> DarkGlass;
					::std::valarray<double> LightGlass;
					::std::valarray<double> PrimaryActive;
					::std::valarray<double> SecondaryActive;
					::std::valarray<double> Inactive;
					::std::valarray<double> Indicator;
					::std::valarray<double> EqualizerLow;
					::std::valarray<double> EqualizerMedium;
					::std::valarray<double> EqualizerHigh;

					inline static Theme Dark() {
						return {
							{ 0., 0., 0., 0. },
							{ 231 / 255., 255 / 255., 17 / 255., 1. },
							{ 248 / 255., 99 / 255., 77 / 255., 1. },
							{ 0.75, 0.75, 0.75, 0.4 },
							{ 1., 1., 1., 0.4 },
							{ 112 / 255., 195 / 255., 153 / 255., 1. },
							{ 104 / 255., 230 / 255., 250 / 255., 1. },
							{ 95 / 255., 120 / 255., 137 / 255., 1. },
							{ 255 / 255., 255 / 255., 255 / 255., 1. },
							{ 139 / 255., 179 / 255., 187 / 255., 1. },
							{ 30 / 255., 254 / 255., 91 / 255., 1. },
							{ 255 / 255., 0 / 255., 0 / 255., 1. }
						};
					};
					inline static Theme Light() {
						return {
							{ 0., 0., 0., 0. },
							{ 215 / 255., 215 / 255., 215 / 255., 1. },
							{ 248 / 255., 99 / 255., 77 / 255., 1. },
							{ 0.75, 0.75, 0.75, 0.4 },
							{ 1., 1., 1., 0.4 },
							{ 112 / 255., 195 / 255., 153 / 255., 1. },
							{ 104 / 255., 230 / 255., 250 / 255., 1. },
							{ 95 / 255., 120 / 255., 137 / 255., 1. },
							{ 215 / 255., 215 / 255., 215 / 255., 1. },
							{ 139 / 255., 179 / 255., 187 / 255., 1. },
							{ 30 / 255., 254 / 255., 91 / 255., 1. },
							{ 255 / 255., 0 / 255., 0 / 255., 1. }
						};
					};
				};

				template<typename TInstrumentation>
				class Atlas final {
					using Palette = Palette<TInstrumentation>;

				public:
					using Specification = ::Voicemeeter::Atlas::CherrySpecification;

					inline Atlas(
						Palette& palette,
						bool horizontal
					) : m_palette{ palette }
					  , m_horizontal{ horizontal }
					  , m_pAtlas{ nullptr } {

					};
					Atlas() = delete;
					Atlas(const Atlas&) = delete;
					Atlas(Atlas&&) = delete;

					~Atlas() = default;

					Atlas& operator=(Atlas&&) = delete;
					Atlas& operator=(const Atlas&) = delete;

					inline ::std::valarray<double> MapPosition(size_t x, size_t y) const {
						return m_pAtlas->MapPosition(x, y);
					};
					inline ::std::valarray<double> MapSize(FLOAT x, FLOAT y) const {
						return m_pAtlas->MapSize(x, y);
					};
					void Rescale(const ::std::valarray<double>& scale);
					void Fill(
						const ::std::valarray<double>& point,
						const ::std::valarray<double>& vertex,
						const ::std::valarray<double>& maskPoint,
						const ::std::valarray<double>& color,
						bool blend);

				private:
					Palette& m_palette;
					bool m_horizontal;
					::std::unique_ptr<::Voicemeeter::Atlas::Cherry> m_pAtlas;
				};

				template<typename TInstrumentation>
				class Bundle {
					using Palette = Palette<TInstrumentation>;

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

					inline ::std::bitset<sizeof(size_t)>& get_Changed() {
						return m_changed;
					};

					inline Bundle(
						Palette& palette,
						const ::std::valarray<double>& baseVertex
					) : m_palette{ palette }
					  , m_changed{}
					  , m_point{ 0., 0. }
					  , m_vertex{ baseVertex }
					  , m_baseVertex{ baseVertex } {

					};

				private:
					Palette& m_palette;
					::std::bitset<sizeof(size_t)> m_changed;
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

				template<typename TInstrumentation>
				class Queue final {
					using Bundle = Bundle<TInstrumentation>;

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

				template<typename TInstrumentation>
				class Palette final {
					using Atlas = Atlas<TInstrumentation>;
					using Queue = Queue<TInstrumentation>;

				public:
					template<typename... Args>
					inline Palette(
						const Theme& theme,
						bool horizontal,
						Args ...args
					) : m_theme{ theme }
					  , m_atlas{ *this, horizontal }
					  , m_timer{}
					  , m_queue{}
					  , m_instrumentation{ ::std::forward<Args>(args)... } {

					}
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
					inline TInstrumentation& get_Instrumentation() {
						return m_instrumentation;
					};

					static inline void Blend(::std::valarray<double>& dst, const ::std::valarray<double>& src, double alpha) {
						using RGBA = Theme::RGBA;

						dst[RGBA::red] = dst[RGBA::red] * (1. - alpha) + src[RGBA::red] * alpha;
						dst[RGBA::green] = dst[RGBA::green] * (1. - alpha) + src[RGBA::green] * alpha;
						dst[RGBA::blue] = dst[RGBA::blue] * (1. - alpha) + src[RGBA::blue] * alpha;
					};

				private:
					const Theme m_theme;
					Atlas m_atlas;
					Timer m_timer;
					Queue m_queue;
					TInstrumentation m_instrumentation;
				};
			}
		}
	}
}