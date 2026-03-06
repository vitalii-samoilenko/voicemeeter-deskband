#ifndef VOICEMEETER_CLIENTS___VBVMR_HPP
#define VOICEMEETER_CLIENTS___VBVMR_HPP

#include <stdexcept>
#include <wchar.h>

#include "math.hpp"

#include "Windows/API.hpp"

#include "Voicemeeter/Cherry.hpp"

#include "VoicemeeterRemote.h"

namespace Voicemeeter {
	namespace Clients {
		namespace _ {
			namespace VBVMR {
				enum runtime_t : long {
					Voicemeeter = 1L,
					Banana = 2L,
					Potato = 3L
				};
				template<typename TMixer>
				class bag;
				template<typename TMixer>
				void Subscribe(bag<TMixer> &tokens, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime);
				template<typename TMixer>
				void Update(TMixer &mixer, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime);

				template<
					typename TTimer,
					typename TMixer>
				class Proxy final {
				public:
					inline Proxy(
						TTimer &timer,
						TMixer &mixer)
						: _mixer{ mixer }
						, _tokens{ mixer }
						, _hVoicemeeterRemote{ NULL }
						, _voicemeeterRemote{}
						, _runtime{ Voicemeeter }
						, _remoteTick{ this, timer } {
						{
							WCHAR path[MAX_PATH];
							DWORD size{ sizeof(path) };
							if (ERROR_SUCCESS != ::Windows::RegGetValueW(
									HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\VB:Voicemeeter {17359A74-1236-5467})", L"UninstallString",
									RRF_RT_REG_SZ, NULL, &value, &size)) {
								throw ::std::runtime_error{ "Voicemeeter is not installed" };
							}
							size_t i{ wcslen(path) };
							for (; 0 < i; --i) {
								if (path[i] == L'\\') {
									break;
								}
							}
							if (0 != wcscpy_s(&path[i], MAX_PATH - i,
									sizeof(void *) == 8
										? L"VoicemeeterRemote64.dll"
										: L"VoicemeeterRemote.dll")) {
								throw ::std::runtime_error{ "Path is too long" };
							}
							_hVoicemeeterRemote = ::Windows::LoadLibraryW(path);
						}
						{
							_voicemeeterRemote.VBVMR_Login = (T_VBVMR_Login)::Windows::GetProcAddress(_hVoicemeeterRemote, "VBVMR_Login");
							_voicemeeterRemote.VBVMR_Logout = (T_VBVMR_Logout)::Windows::GetProcAddress(_hVoicemeeterRemote, "VBVMR_Logout");
							_voicemeeterRemote.VBVMR_GetVoicemeeterType = (T_VBVMR_GetVoicemeeterType)::Windows::GetProcAddress(_hVoicemeeterRemote,"VBVMR_GetVoicemeeterType");
							_voicemeeterRemote.VBVMR_IsParametersDirty = (T_VBVMR_IsParametersDirty)::Windows::GetProcAddress(_hVoicemeeterRemote,"VBVMR_IsParametersDirty");
							_voicemeeterRemote.VBVMR_GetParameterFloat = (T_VBVMR_GetParameterFloat)::Windows::GetProcAddress(_hVoicemeeterRemote,"VBVMR_GetParameterFloat");
							_voicemeeterRemote.VBVMR_GetLevel = (T_VBVMR_GetLevel)::Windows::GetProcAddress(_hVoicemeeterRemote,"VBVMR_GetLevel");
							_voicemeeterRemote.VBVMR_SetParameterFloat = (T_VBVMR_SetParameterFloat)::Windows::GetProcAddress(_hVoicemeeterRemote,"VBVMR_SetParameterFloat");
						}
						long login{ _voicemeeterRemote.VBVMR_Login() };
						if (login < 0) {
							throw ::std::runtime_error{ "Could not connect to Voicemeeter" };
						} else if (login) {
							_voicemeeterRemote.VBVMR_Logout();
							::FreeLibrary(_hVoicemeeterRemote);
							throw ::std::runtime_error{ "Voicemeeter is not started" };
						}
						if (_voicemeeterRemote.VBVMR_GetVoicemeeterType(
							reinterpret_cast<long *>(&_runtime))) {
							_voicemeeterRemote.VBVMR_Logout();
							::FreeLibrary(_hVoicemeeterRemote);
							throw ::std::runtime_error{ "Could not get Voicemeeter type" };
						}
						Subscribe(_tokens, _voicemeeterRemote, _runtime);
						_remoteTick.Set();
					};
					Proxy() = delete;
					Proxy(Proxy const &) = delete;
					Proxy(Proxy &&) = delete;

					inline ~Proxy() {
						_voicemeeterRemote.VBVMR_Logout();
						::FreeLibrary(_hVoicemeeterRemote);
					};

					Proxy & operator=(Proxy const &) = delete;
					Proxy & operator=(Proxy &&) = delete;

					inline Type get_Type() const {
						return _runtime;
					};

				private:
					class UpdateTick final {
					public:
						inline UpdateTick(
							Remote *that,
							TTimer &timer)
							: that{ that }
							, _timer{ timer } {

						};
						UpdateTick() = delete;
						UpdateTick(ProxyTick const &) = delete;
						UpdateTick(ProxyTick &&) = delete;

						inline ~UpdateTick() {
							Unset();
						};

						UpdateTick & operator=(ProxyTick const &) = delete;
						UpdateTick & operator=(ProxyTick &&) = delete;

						inline void operator()() const {
							Update(that->_mixer, that->_voicemeeterRemote, that->_runtime);
						};

						inline void Set() {
							_timer.Set(100, *this);
						};
						inline void Unset() {
							_timer.Unset(*this);
						};

					private:
						Remote *that;
						TTimer &_timer;
					};

					friend class UpdateTick;

					TMixer &_mixer;
					bag<TMixer> _tokens;
					HMODULE _hVoicemeeterRemote;
					T_VBVMR_INTERFACE _voicemeeterRemote;
					runtime_t _runtime;
					UpdateTick _updateTick;
				};

				template<>
				class bag<Cherry> final {
				public:
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
					bag() = delete;
					bag(bag const &) = delete;
					inline bag(bag &&) = default;

					inline ~bag() = default;

					bag & operator=(bag const &) = delete;
					bag & operator=(bag &&) = delete;

				private:
					Cherry::token _tokenM;
					Cherry::PIStrip::token _tokenP;
					Cherry::VIStrip::token _tokenV;
					Cherry::POStrip::token _tokenA1;
					Cherry::POStrip::token _tokenA2;
					Cherry::VOStrip::token _tokenB1;
					Cherry::VOStrip::token _tokenB2;
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
				inline void SubscribePlug(typename TMixer::token &token, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime) {
					token.on_plug<From, To>([
						&voicemeeterRemote,
						key = ToPlugKey<TMixer>(runtime, From, To)
					](bool value)->void {
						if (voicemeeterRemote.VBVMR_SetParameterFloat(const_cast<char *>(key), static_cast<float>(value))) {
							throw ::std::runtime_error{ key };
						}
						g_dirty = true;
					});
				};
				template<typename TMixer, typename TMixer::Strips Target, typename TToken>
				inline void SubscribeStrip(TToken &token, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime) {
					token.on_gain([
						&voicemeeterRemote,
						key = ToStripKey<TMixer>(runtime, Target, Gain)
					](num_t value)->void {
						if (voicemeeterRemote.VBVMR_SetParameterFloat(const_cast<char *>(key), static_cast<float>(value) / One)) {
							throw ::std::runtime_error{ key };
						}
						g_dirty = true;
					});
					token.on_mute([
						&voicemeeterRemote,
						key = ToStripKey<TMixer>(runtime, Target, Mute)
					](bool value)->void {
						if (voicemeeterRemote.VBVMR_SetParameterFloat(const_cast<char *>(key), static_cast<float>(value))) {
							throw ::std::runtime_error{ key };
						}
						g_dirty = true;
					});
				};

				template<typename TMixer, typename TMixer::Strips From, typename TMixer::Strips To>
				inline void UpdatePlug(TMixer &mixer, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime) {
					char const *key{ ToPlugKey<TMixer>(runtime, From, To) };
					float value{ 0.F };
					if (voicemeeterRemote.VBVMR_GetParameterFloat(const_cast<char *>(key), &value)) {
						throw ::std::runtime_error{ key };
					}
					mixer.set_Plug<bag<TMixer>, From, To>(0.01F < value);
				};
				template<typename TMixer, typename TMixer::Strips Target>
				inline void UpdateStrip(TMixer &mixer, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime) {
					char const *key{ ToStripKey<TMixer>(runtime, Target, Gain) };
					float value{ 0.F };
					if (voicemeeterRemote.VBVMR_GetParameterFloat(const_cast<char *>(key), &value)) {
						throw ::std::runtime_error{ key };
					}
					mixer.get_Strip<Target>()
						.set_Gain<bag<TMixer>>(static_cast<num_t>(value * One));
					key = ToStripKey<TMixer>(runtime, Target, Mute);
					if (voicemeeterRemote.VBVMR_GetParameterFloat(const_cast<char *>(key), &value)) {
						throw ::std::runtime_error{ key };
					}
					mixer.get_Strip<Target>()
						.set_Mute<bag<TMixer>>(0.01F < value);
				};
				template<typename TMixer, typename TMixer::Strips Target, size_t ...Channels>
				inline void UpdateChannels(TMixer &mixer, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime, level_t level) {
					long key{ 0L };
					float value{ 0.F };
					long code{ 0L };
					(((key = ToChannelKey<TMixer>(runtime, Target, Channels))
					,(code = voicemeeterRemote.VBVMR_GetLevel(level, key, &value))
					,(mixer.get_Strip<Target>()
						.get_Channel<Channels>()
						.set_Level<bag<TMixer>>(code
							? throw ::std::runtime_error{ "level" }
							: static_cast<num_t>(value * push(10000)))))
					, ...);
				};

				template<>
				inline void Subscribe<Cherry>(bag<Cherry> &tokens, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime) {
					SubscribePlug<Cherry,
						Cherry::Strips::P, Cherry::Strips::A1>(
						tokens._tokenM, voicemeeterRemote, runtime);
					SubscribePlug<Cherry,
						Cherry::Strips::P, Cherry::Strips::B1>(
						tokens._tokenM, voicemeeterRemote, runtime);
					SubscribePlug<Cherry,
						Cherry::Strips::V, Cherry::Strips::A1>(
						tokens._tokenM, voicemeeterRemote, runtime);
					SubscribePlug<Cherry,
						Cherry::Strips::V, Cherry::Strips::B1>(
						tokens._tokenM, voicemeeterRemote, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::P>(
						tokens._tokenP, voicemeeterRemote, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::V>(
						tokens._tokenV, voicemeeterRemote, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::A1>(
						tokens._tokenA1, voicemeeterRemote, runtime);
					SubscribeStrip<Cherry,
						Cherry::Strips::B1>(
						tokens._tokenB1, voicemeeterRemote, runtime);
					if (Voicemeeter < runtime) {
						tokens._tokenM.on_vban([
							&voicemeeterRemote
						](bool value)->void {
							if (voicemeeterRemote.VBVMR_SetParameterFloat(const_cast<char *>(g_keys[0]), static_cast<float>(value))) {
								throw ::std::runtime_error{ g_keys[0] };
							}
							g_dirty = true;
						});
						SubscribePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::A2>(
							tokens._tokenM, voicemeeterRemote, runtime);
						SubscribePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::B2>(
							tokens._tokenM, voicemeeterRemote, runtime);
						SubscribePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::A2>(
							tokens._tokenM, voicemeeterRemote, runtime);
						SubscribePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::B2>(
							tokens._tokenM, voicemeeterRemote, runtime);
						SubscribeStrip<Cherry,
							Cherry::Strips::A2>(
							tokens._tokenA2, voicemeeterRemote, runtime);
						SubscribeStrip<Cherry,
							Cherry::Strips::B2>(
							tokens._tokenB2, voicemeeterRemote, runtime);
					}
				};
				template<>
				inline void Update<Cherry>(Cherry &mixer, T_VBVMR_INTERFACE &voicemeeterRemote, runtime_t runtime) {
					long temp{ voicemeeterRemote.VBVMR_IsParametersDirty() };
					if (temp < 0) {
						throw ::std::runtime_error{ "Could not check Voicemeeter" };
					}
					bool dirty{ temp && !g_dirty };
					g_dirty = false;
					if (dirty) {
						UpdatePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::A1>(
							mixer, voicemeeterRemote, runtime);
						UpdatePlug<Cherry,
							Cherry::Strips::P, Cherry::Strips::B1>(
							mixer, voicemeeterRemote, runtime);
						UpdatePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::A1>(
							mixer, voicemeeterRemote, runtime);
						UpdatePlug<Cherry,
							Cherry::Strips::V, Cherry::Strips::B1>(
							mixer, voicemeeterRemote, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::P>(
							mixer, voicemeeterRemote, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::V>(
							mixer, voicemeeterRemote, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::A1>(
							mixer, voicemeeterRemote, runtime);
						UpdateStrip<Cherry,
							Cherry::Strips::B1>(
							mixer, voicemeeterRemote, runtime);
					}
					UpdateChannels<Cherry,
						Cherry::Strips::P, 0, 1>(
						mixer, voicemeeterRemote, runtime, PostFaderInput);
					UpdateChannels<Cherry,
						Cherry::Strips::V, 0, 1, 2, 3, 4, 5, 6, 7>(
						mixer, voicemeeterRemote, runtime, PostFaderInput);
					UpdateChannels<Cherry,
						Cherry::Strips::A1, 0, 1, 2, 3, 4, 5, 6, 7>(
						mixer, voicemeeterRemote, runtime, Output);
					UpdateChannels<Cherry,
						Cherry::Strips::B1, 0, 1, 2, 3, 4, 5, 6, 7>(
						mixer, voicemeeterRemote, runtime, Output);
					if (Voicemeeter < runtime) {
						if (dirty) {
							float value{ 0.F };
							if (voicemeeterRemote.VBVMR_GetParameterFloat(const_cast<char *>(g_keys[0]), &value)) {
								throw ::std::runtime_error{ g_keys[0] };
							}
							mixer.set_Vban<bag<Cherry>>(0.01F < value);
							UpdatePlug<Cherry,
								Cherry::Strips::P, Cherry::Strips::A2>(
								mixer, voicemeeterRemote, runtime);
							UpdatePlug<Cherry,
								Cherry::Strips::P, Cherry::Strips::B2>(
								mixer, voicemeeterRemote, runtime);
							UpdatePlug<Cherry,
								Cherry::Strips::V, Cherry::Strips::A2>(
								mixer, voicemeeterRemote, runtime);
							UpdatePlug<Cherry,
								Cherry::Strips::V, Cherry::Strips::B2>(
								mixer, voicemeeterRemote, runtime);
							UpdateStrip<Cherry,
								Cherry::Strips::A2>(
								mixer, voicemeeterRemote, runtime);
							UpdateStrip<Cherry,
								Cherry::Strips::B2>(
								mixer, voicemeeterRemote, runtime);
						}
						UpdateChannels<Cherry,
							Cherry::Strips::A2, 0, 1, 2, 3, 4, 5, 6, 7>(
							mixer, voicemeeterRemote, runtime, Output);
						UpdateChannels<Cherry,
							Cherry::Strips::B2, 0, 1, 2, 3, 4, 5, 6, 7>(
							mixer, voicemeeterRemote, runtime, Output);
					}
				};
			}
		}
	}
}

#endif
