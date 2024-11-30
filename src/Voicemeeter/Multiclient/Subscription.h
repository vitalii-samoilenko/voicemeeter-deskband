#pragma once

#include <array>
#include <functional>

namespace Voicemeeter {
	namespace Multiclient {
		template<typename Specification>
		class Subscription {
		public:
			inline Subscription() = default;
			Subscription(const Subscription&) = delete;
			Subscription(Subscription&&) = delete;

			inline ~Subscription() = default;

			Subscription& operator=(const Subscription&) = delete;
			Subscription& operator=(Subscription&&) = delete;

			inline const ::std::function<void(double)>& on_Level(size_t id) const {
				return m_cLevelCallback[id];
			};
			inline void on_Level(size_t id, const ::std::function<void(double)>& callback) {
				m_cLevelCallback[id] = callback;
			};
			inline const ::std::function<void(double)>& on_Gain(size_t id) const {
				return m_cGainCallback[id];
			};
			inline void on_Gain(size_t id, const ::std::function<void(double)>& callback) {
				m_cGainCallback[id] = callback;
			};
			inline const ::std::function<void(bool)>& on_Mute(size_t id) const {
				return m_cMuteCallback[id];
			};
			inline void on_Mute(size_t id, const ::std::function<void(bool)>& callback) {
				m_cMuteCallback[id] = callback;
			};
			inline const ::std::function<void(bool)>& on_Vban() const {
				return m_VbanCallback;
			};
			inline void on_Vban(const ::std::function<void(bool)>& callback) {
				m_VbanCallback = callback;
			};
			inline const ::std::function<void(bool)>& on_Plug(size_t inputId, size_t outputId) const {
				return m_cPlugCallback[(Specification::Input::Physical::Width + Specification::Input::Virtual::Width) * inputId
					+ outputId];
			};
			inline void on_Plug(size_t inputId, size_t outputId, const ::std::function<void(bool)>& callback) {
				m_cPlugCallback[(Specification::Input::Physical::Width + Specification::Input::Virtual::Width) * inputId
					+ outputId] = callback;
			};

		private:
			::std::array<::std::function<void(double)>,
				(Specification::Input::Physical::Width * Specification::Input::Physical::Strip::Width)
				+ (Specification::Input::Virtual::Width * Specification::Input::Virtual::Strip::Width)
				+ (Specification::Output::Physical::Width * Specification::Output::Physical::Strip::Width)
				+ (Specification::Output::Virtual::Width * Specification::Output::Virtual::Strip::Width)> m_cLevelCallback;
			::std::array<::std::function<void(double)>,
				Specification::Input::Physical::Width
				+ Specification::Input::Virtual::Width
				+ Specification::Output::Physical::Width
				+ Specification::Output::Virtual::Width> m_cGainCallback;
			::std::array<::std::function<void(bool)>,
				Specification::Input::Physical::Width
				+ Specification::Input::Virtual::Width
				+ Specification::Output::Physical::Width
				+ Specification::Output::Virtual::Width> m_cMuteCallback;
			::std::array<::std::function<void(bool)>,
				(Specification::Input::Physical::Width + Specification::Input::Virtual::Width)
				* (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)> m_cPlugCallback;
			::std::function<void(bool)> m_VbanCallback;
		};
	}
}