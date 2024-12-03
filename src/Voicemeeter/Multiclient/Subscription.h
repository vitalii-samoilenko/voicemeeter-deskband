#pragma once

#include <array>
#include <functional>

namespace Voicemeeter {
	namespace Multiclient {
		template<typename Specification>
		class Subscription {
		public:
			inline Subscription(
			) : m_cLevelCallback{}
			  , m_cGainCallback{}
			  , m_cMuteCallback{}
			  , m_cPlugCallback{}
			  , m_vbanCallback{ [](bool)->void {} } {
				for (::std::function<void(double)>& callback : m_cLevelCallback) {
					callback = [](double)->void {};
				}
				for (::std::function<void(double)>& callback : m_cGainCallback) {
					callback = [](double)->void {};
				}
				for (::std::function<void(bool)>& callback : m_cMuteCallback) {
					callback = [](bool)->void {};
				}
				for (::std::function<void(bool)>& callback : m_cPlugCallback) {
					callback = [](bool)->void {};
				}
			};
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
				return m_vbanCallback;
			};
			inline void on_Vban(const ::std::function<void(bool)>& callback) {
				m_vbanCallback = callback;
			};
			inline const ::std::function<void(bool)>& on_Plug(size_t inputId, size_t outputId) const {
				return m_cPlugCallback[(Specification::Output::Physical::Width + Specification::Output::Virtual::Width) * inputId
					+ outputId - (Specification::Input::Physical::Width + Specification::Input::Virtual::Width)];
			};
			inline void on_Plug(size_t inputId, size_t outputId, const ::std::function<void(bool)>& callback) {
				m_cPlugCallback[(Specification::Output::Physical::Width + Specification::Output::Virtual::Width) * inputId
					+ outputId - (Specification::Input::Physical::Width + Specification::Input::Virtual::Width)] = callback;
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
			::std::function<void(bool)> m_vbanCallback;
		};
	}
}