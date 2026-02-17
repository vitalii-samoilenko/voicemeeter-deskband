#ifndef VOICEMEETER_CLIENTS___REMOTE_HPP
#define VOICEMEETER_CLIENTS___REMOTE_HPP

#include <stdexcept>

#include "wheel.hpp"

#include "Voicemeeter/Cherry.hpp"
#include "VoicemeeterRemote.h"

namespace Voicemeeter {
	namespace Clients {
		namespace _ {
			namespace Remote {
				enum runtime_t : long {
					Voicemeeter = 1L,
					Banana = 2L,
					Potato = 3L
				};
				template<typename TMixer>
				class bag;
				template<typename TMixer>
				bag<TMixer> Subscribe(TMixer &mixer, T_VBVMR_INTERFACE &remote, runtime_t runtime);
				template<typename TMixer>
				void Update(TMixer &mixer, T_VBVMR_INTERFACE &remote, runtime_t runtime);

				template<>
				class bag<Cherry> final {
				public:
					bag() = delete;
					bag(bag const &) = delete;
					inline bag(bag &&) = default;

					inline ~bag() = default;

					bag & operator=(bag const &) = delete;
					bag & operator=(bag &&) = delete;

				private:
					friend bag<Cherry> Subscribe<Cherry>(Cherry &, T_VBVMR_INTERFACE &, runtime_t);

					Cherry::token _tokenM;
					Cherry::PIStrip::token _tokenP;
					Cherry::VIStrip::token _tokenV;
					Cherry::POStrip::token _tokenA1;
					Cherry::POStrip::token _tokenA2;
					Cherry::VOStrip::token _tokenB1;
					Cherry::VOStrip::token _tokenB2;

					inline explicit bag(Cherry &mixer)
						: _tokenM{ mixer.Subscribe<bag>() }
						, _tokenP{
							mixer.get_Strip<Cherry::Strips::P>()
								.Subscribe<bag>()
						}
						, _tokenV{
							mixer.get_Strip<Cherry::Strips::V>()
								.Subscribe<bag>()
						}
						, _tokenA1{
							mixer.get_Strip<Cherry::Strips::A1>()
								.Subscribe<bag>()
						}
						, _tokenA2{
							mixer.get_Strip<Cherry::Strips::A2>()
								.Subscribe<bag>()
						}
						, _tokenB1{
							mixer.get_Strip<Cherry::Strips::B1>()
								.Subscribe<bag>()
						}
						, _tokenB2{
							mixer.get_Strip<Cherry::Strips::B2>()
								.Subscribe<bag>()
						} {

					};
				};

				static bool g_dirty{ false };
				static char const * g_keys[]{
					"vban.Enable",
					"Strip[0].A1", "Strip[0].A2", "Strip[0].B1", "Strip[0].B2",
					"Strip[2].A1", "Strip[2].A2", "Strip[2].B1", "Strip[2].B2",
					"Strip[3].A1", "Strip[3].A2", "Strip[3].B1", "Strip[3].B2",
					"Strip[5].A1", "Strip[5].A2", "Strip[5].B1", "Strip[5].B2",
					"Strip[0].Gain", "Strip[0].Mute",
					"Strip[2].Gain", "Strip[2].Mute",
					"Strip[3].Gain", "Strip[3].Mute",
					"Strip[5].Gain", "Strip[5].Mute",
					"Bus[0].Gain", "Bus[0].Mute",
					"Bus[1].Gain", "Bus[1].Mute",
					"Bus[3].Gain", "Bus[3].Mute",
					"Bus[5].Gain", "Bus[5].Mute",
					"Bus[4].Gain", "Bus[4].Mute",
					"Bus[6].Gain", "Bus[6].Mute"
				};
				static long g_c_keys[]{
					0L,
					4L, 6L, 10L,
					0L, 8L,
					24L, 40L,
					32L, 48L
				};
				enum prop_t : size_t {
					Gain = 0,
					Mute = 1
				};
				enum level_t : long {
					PreFaderInput = 0L,
					PostFaderInput = 1L,
					PostMuteInput = 2L,
					Output = 3L
				};

				template<typename TMixer>
				char const * ToPlugKey(runtime_t runtime,
					typename TMixer::Strips from, typename TMixer::Strips to);
				template<typename TMixer>
				char const * ToStripKey(runtime_t runtime,
					typename TMixer::Strips target, prop_t property);
				template<typename TMixer>
				long ToChannelKey(runtime_t runtime,
					typename TMixer::Strips target, size_t channel);

				template<>
				inline char const * ToPlugKey<Cherry>(runtime_t runtime,
					Cherry::Strips from, Cherry::Strips to) {
					switch (from) {
					case Cherry::Strips::P:
						return g_keys[1 + to - Cherry::Strips::A1];
					case Cherry::Strips::V:
						return g_keys[5 + 4 * (runtime - Voicemeeter) + to - Cherry::Strips::A1];
					default:
						return nullptr;
					}
				};
				template<>
				inline char const * ToStripKey<Cherry>(runtime_t runtime,
					Cherry::Strips target, prop_t property) {
					switch (target) {
					case Cherry::Strips::P:
						return g_keys[17 + property];
					case Cherry::Strips::V:
						return g_keys[19 + 2 * (runtime - Voicemeeter) + property];
					case Cherry::Strips::A1:
						return g_keys[25 + property];
					case Cherry::Strips::A2:
						return g_keys[27 + property];
					case Cherry::Strips::B1:
						return g_keys[27 + 2 * (runtime - Voicemeeter) + property];
					case Cherry::Strips::B2:
						return g_keys[31 + 2 * (runtime - Voicemeeter) + property];
					default:
						return nullptr;
					}
				};
				template<>
				inline long ToChannelKey<Cherry>(runtime_t runtime,
					Cherry::Strips target, size_t channel) {
					switch (target) {
					case Cherry::Strips::P:
						return g_c_keys[0] + channel;
					case Cherry::Strips::V:
						return g_c_keys[1 + runtime - Voicemeeter] + channel;
					case Cherry::Strips::A1:
						return g_c_keys[4] + channel;
					case Cherry::Strips::A2:
						return g_c_keys[5] + channel;
					case Cherry::Strips::B1:
						return g_c_keys[5 + runtime - Voicemeeter] + channel;
					case Cherry::Strips::B2:
						return g_c_keys[7 + runtime - Voicemeeter] + channel;
					default:
						return -1L;
					}
				};

				template<typename TMixer, typename TMixer::Strips From, typename TMixer::Strips To>
				inline void SubscribePlug(typename TMixer::token &token, TMixer &mixer, T_VBVMR_INTERFACE &client, runtime_t runtime) {
					token.on_plug<From, To>([
						&client,
						key = ToPlugKey<TMixer>(runtime, From, To)
					](bool value)->void {
						if (client.VBVMR_SetParameterFloat(const_cast<char *>(key), static_cast<float>(value))) {
							throw ::std::runtime_error{ key };
						}
						g_dirty = true;
					});
				};
				template<typename TMixer, typename TMixer::Strips Target, typename TToken>
				inline void SubscribeStrip(TToken &token, T_VBVMR_INTERFACE &client, runtime_t runtime) {
					token.on_gain([
						&client,
						key = ToStripKey<TMixer>(runtime, Target, Gain)
					](num_t value)->void {
						if (client.VBVMR_SetParameterFloat(const_cast<char *>(key), static_cast<float>(value) / One)) {
							throw ::std::runtime_error{ key };
						}
						g_dirty = true;
					});
					token.on_mute([
						&client,
						key = ToStripKey<TMixer>(runtime, Target, Mute)
					](bool value)->void {
						if (client.VBVMR_SetParameterFloat(const_cast<char *>(key), static_cast<float>(value))) {
							throw ::std::runtime_error{ key };
						}
						g_dirty = true;
					});
				};

				template<typename TMixer, typename TMixer::Strips From, typename TMixer::Strips To>
				inline void UpdatePlug(TMixer &mixer, T_VBVMR_INTERFACE &client, runtime_t runtime) {
					char const *key{ ToPlugKey<TMixer>(runtime, From, To) };
					float value{ 0.F };
					if (client.VBVMR_GetParameterFloat(const_cast<char *>(key), &value)) {
						throw ::std::runtime_error{ key };
					}
					mixer.set_Plug<bag<TMixer>, From, To>(0.01F < value);
				};
				template<typename TMixer, typename TMixer::Strips Target>
				inline void UpdateStrip(TMixer &mixer, T_VBVMR_INTERFACE &client, runtime_t runtime) {
					char const *key{ ToStripKey<TMixer>(runtime, Target, Gain) };
					float value{ 0.F };
					if (client.VBVMR_GetParameterFloat(const_cast<char *>(key), &value)) {
						throw ::std::runtime_error{ key };
					}
					mixer.get_Strip<Target>()
						.set_Gain<bag<TMixer>>(static_cast<num_t>(value * One));
					key = ToStripKey<TMixer>(runtime, Target, Mute);
					if (client.VBVMR_GetParameterFloat(const_cast<char *>(key), &value)) {
						throw ::std::runtime_error{ key };
					}
					mixer.get_Strip<Target>()
						.set_Mute<bag<TMixer>>(0.01F < value);
				};
				template<typename TMixer, typename TMixer::Strips Target, size_t ...Channels>
				inline void UpdateChannels(TMixer &mixer, T_VBVMR_INTERFACE &client, runtime_t runtime, level_t level) {
					long key{ 0L };
					float value{ 0.F };
					long code{ 0L };
					(((key = ToChannelKey<TMixer>(runtime, Target, Channels))
					,(code = client.VBVMR_GetLevel(level, key, &value))
					,(mixer.get_Strip<Target>()
						.get_Channel<Channels>()
						.set_Level<bag<TMixer>>(code
							? throw ::std::runtime_error{ "level" }
							: static_cast<num_t>(value * One))))
					, ...);
				};

				template<>
				inline bag<Cherry> Subscribe<Cherry>(Cherry &mixer, T_VBVMR_INTERFACE &client, runtime_t runtime) {
					bag<Cherry> tokens{ mixer };
					SubscribePlug<Cherry,
						Cherry::Strips::P, Cherry::Strips::A1>(
						tokens._tokenM, mixer, client, runtime);
					SubscribePlug<Cherry,
						Cherry::Strips::P, Cherry::Strips::B1>(
						tokens._tokenM, mixer, client, runtime);
					SubscribePlug<Cherry,
						Cherry::Strips::V, Cherry::Strips::A1>(
						tokens._tokenM, mixer, client, runtime);
					SubscribePlug<Cherry,
						Cherry::Strips::V, Cherry::Strips::B1>(
						tokens._tokenM, mixer, client, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::P>(
						tokens._tokenP, client, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::V>(
						tokens._tokenV, client, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::A1>(
						tokens._tokenA1, client, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::B1>(
						tokens._tokenB1, client, runtime);
					if (Voicemeeter < runtime) {
						tokens._tokenM.on_vban([
							&client
						](bool value)->void {
							if (client.VBVMR_SetParameterFloat(const_cast<char *>(g_keys[0]), static_cast<float>(value))) {
								throw ::std::runtime_error{ g_keys[0] };
							}
							g_dirty = true;
						});
						SubscribePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::A2>(
							tokens._tokenM, mixer, client, runtime);
						SubscribePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::B2>(
							tokens._tokenM, mixer, client, runtime);
						SubscribePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::A2>(
							tokens._tokenM, mixer, client, runtime);
						SubscribePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::B2>(
							tokens._tokenM, mixer, client, runtime);
						SubscribeStrip<Cherry,
							Cherry::Strips::A2>(
							tokens._tokenA2, client, runtime);
						SubscribeStrip<Cherry,
							Cherry::Strips::B2>(
							tokens._tokenB2, client, runtime);
					}
					return tokens;
				};
				template<>
				inline void Update<Cherry>(Cherry &mixer, T_VBVMR_INTERFACE &client, runtime_t runtime) {
					long temp{ client.VBVMR_IsParametersDirty() };
					if (temp < 0) {
						throw ::std::runtime_error{ "Could not check Voicemeeter" };
					}
					bool dirty{ temp && !g_dirty };
					g_dirty = false;
					if (dirty) {
						UpdatePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::A1>(
							mixer, client, runtime);
						UpdatePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::B1>(
							mixer, client, runtime);
						UpdatePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::A1>(
							mixer, client, runtime);
						UpdatePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::B1>(
							mixer, client, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::P>(
							mixer, client, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::V>(
							mixer, client, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::A1>(
							mixer, client, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::B1>(
							mixer, client, runtime);
					}
					UpdateChannels<Cherry,
						Cherry::Strips::P, 0, 1>(
						mixer, client, runtime, PostFaderInput);
					UpdateChannels<Cherry,
						Cherry::Strips::V, 0, 1, 2, 3, 4, 5, 6, 7>(
						mixer, client, runtime, PostFaderInput);
					UpdateChannels<Cherry,
						Cherry::Strips::A1, 0, 1, 2, 3, 4, 5, 6, 7>(
						mixer, client, runtime, Output);
					UpdateChannels<Cherry,
						Cherry::Strips::B1, 0, 1, 2, 3, 4, 5, 6, 7>(
						mixer, client, runtime, Output);
					if (Voicemeeter < runtime) {
						if (dirty) {
							float value{ 0.F };
							if (client.VBVMR_GetParameterFloat(const_cast<char *>(g_keys[0]), &value)) {
								throw ::std::runtime_error{ g_keys[0] };
							}
							mixer.set_Vban<bag<Cherry>>(0.01F < value);
							UpdatePlug<Cherry,
								Cherry::Strips::P, Cherry::Strips::A2>(
								mixer, client, runtime);
							UpdatePlug<Cherry,
								Cherry::Strips::P, Cherry::Strips::B2>(
								mixer, client, runtime);
							UpdatePlug<Cherry,
								Cherry::Strips::V, Cherry::Strips::A2>(
								mixer, client, runtime);
							UpdatePlug<Cherry,
								Cherry::Strips::V, Cherry::Strips::B2>(
								mixer, client, runtime);
							UpdateStrip<Cherry,
								Cherry::Strips::A2>(
								mixer, client, runtime);
							UpdateStrip<Cherry,
								Cherry::Strips::B2>(
								mixer, client, runtime);
						}
						UpdateChannels<Cherry,
							Cherry::Strips::A2, 0, 1, 2, 3, 4, 5, 6, 7>(
							mixer, client, runtime, Output);
						UpdateChannels<Cherry,
							Cherry::Strips::B2, 0, 1, 2, 3, 4, 5, 6, 7>(
							mixer, client, runtime, Output);
					}
				};
			}
		}
	}
}

#endif
