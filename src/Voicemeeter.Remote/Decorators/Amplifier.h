#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "Voicemeeter/Decorators/IAmplifier.h"

#include "../Bus.h"

using namespace Voicemeeter::Decorators;

namespace Voicemeeter {
	namespace Remote {
		namespace Decorators {
			template<typename TBus>
			class Amplifier : public IAmplifier, public TBus {
				static_assert(
					::std::is_base_of_v<Bus, TBus>,
					"TBus must be derived from Bus");
			public:
				template<typename... Args>
				Amplifier(
					Mixer& mixer,
					const ::std::string& key,
					Args&& ...args
				) : TBus{ ::std::forward<Args>(args)... }
				  , m_mixer{ mixer }
				  , m_key{ key }
				  , m_gain{}
				  , m_mute{}
				  , m_cGainCallback{}
				  , m_cMuteCallback{} {

				}
				Amplifier() = delete;
				Amplifier(const Amplifier&) = delete;
				Amplifier(Amplifier&&) = delete;

				~Amplifier() = default;

				Amplifier& operator=(const Amplifier&) = delete;
				Amplifier& operator=(Amplifier&&) = delete;

				inline const ::std::string& get_Key() const {
					return m_key;
				}

				virtual double get_Gain() const override {
					return m_gain;
				};
				virtual void set_Gain(double value) override {
					m_mixer.set_Dirty();
					m_gain = value;
					m_mixer.set_Parameter(const_cast<char*>((m_key + ".Gain").c_str()), static_cast<float>(m_gain));
				};
				void on_Gain(const ::std::function<void(double)>& callback) {
					m_cGainCallback.push_back(callback);
				};
				virtual bool get_Mute() const override {
					return m_mute;
				};
				virtual void set_Mute(bool value) override {
					m_mixer.set_Dirty();
					m_mute = value;
					m_mixer.set_Parameter(const_cast<char*>((m_key + ".Mute").c_str()), m_mute);
				};
				void on_Mute(const ::std::function<void(bool)>& callback) {
					m_cMuteCallback.push_back(callback);
				};

				void Update(bool dirty) override {
					TBus::Update(dirty);

					if (!dirty) {
						return;
					}

					float value{ m_mixer.get_Parameter(m_key + ".Gain") };
					if (!(::std::abs(m_gain - value) < 0.01)) {
						m_gain = value;
						for (const ::std::function<void(double)>& callback : m_cGainCallback) {
							callback(m_gain);
						}
					}
					value = m_mixer.get_Parameter(m_key + ".Mute");
					if (!(::std::abs(m_mute - value) < 0.01)) {
						m_mute = !(value < 0.01);
						for (const ::std::function<void(double)>& callback : m_cMuteCallback) {
							callback(m_mute);
						}
					}
				};

			private:
				Mixer& m_mixer;
				::std::string m_key;
				double m_gain;
				bool m_mute;
				::std::vector<::std::function<void(double)>> m_cGainCallback;
				::std::vector<::std::function<void(bool)>> m_cMuteCallback;
			};
		}
	}
}