#pragma once

#include <chrono>
#include <functional>
#include <unordered_map>
#include <vector>

#include "Environment/ITimer.h"
#include "Voicemeeter/IMixer.h"

#include "Api.h"
#include "Input.h"
#include "Network.h"
#include "Output.h"

template<>
struct ::std::iterator_traits<::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Input>> {
	typedef ::Voicemeeter::Remote::Input value_type;
};
template<>
struct ::std::iterator_traits<::Voicemeeter::Remote::RangeIterator<::Voicemeeter::Remote::Output>> {
	typedef ::Voicemeeter::Remote::Output value_type;
};
template<>
struct ::std::hash<::std::pair<unsigned long long, unsigned long long>> {
	size_t operator()(const ::std::pair<unsigned long long, unsigned long long>& key) const {
		return static_cast<size_t>(key.first ^ key.second);
	}
};

namespace Voicemeeter {
	namespace Remote {
		class Mixer : public IMixer<
			Input, RangeIterator<Input>,
			Output, RangeIterator<Output>> {
		public:
			explicit Mixer(
				::Environment::ITimer& timer
			);
			Mixer() = delete;
			Mixer(const Mixer&) = delete;
			Mixer(Mixer&&) = delete;

			~Mixer();

			Mixer& operator=(const Mixer&) = delete;
			Mixer& operator=(Mixer&&) = delete;

			inline const T_VBVMR_INTERFACE& get_Remote() const {
				return m_remote;
			}
			inline void set_Dirty() {
				m_dirty = true;
			};
			inline void set_Restart() {
				m_restart = ::std::chrono::system_clock::now();
			}

			virtual Network& get_Network() override;
			virtual const Range<Input>& get_Inputs() const override;
			virtual const Range<Output>& get_Outputs() const override;
			virtual const Range<Output>& get_Plugs(const Input& input) const override;
			virtual const Range<Input>& get_Plugs(const Output& output) const override;
			virtual void set_Plug(const Input& input, const Output& output, bool value) override;
			void on_Plug(const Input& input, const Output& output, const ::std::function<void(bool)>& callback);

		private:
			::Environment::ITimer& m_timer;
			T_VBVMR_INTERFACE m_remote;
			Network m_network;
			Range<Input> m_cInput;
			Range<Output> m_cOutput;
			::std::unordered_map<unsigned long long, Range<Output>> m_cInputPlugs;
			::std::unordered_map<unsigned long long, Range<Input>> m_cOutputPlugs;
			::std::unordered_map<::std::pair<unsigned long long, unsigned long long>, ::std::vector<::std::function<void(bool)>>> m_cCallback;
			bool m_dirty;
			::std::chrono::time_point<::std::chrono::system_clock> m_restart;
		};
	}
}