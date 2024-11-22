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
struct ::std::hash<::std::pair<void*, void*>> {
	size_t operator()(const ::std::pair<void*, void*>& key) const {
		return static_cast<size_t>(
			reinterpret_cast<unsigned long long>(key.first)
			^ reinterpret_cast<unsigned long long>(key.second));
	}
};

namespace Voicemeeter {
	namespace Remote {
		class Mixer : public IMixer<
			Input, RangeIterator<Input>,
			Output, RangeIterator<Output>> {
		public:
			explicit Mixer(
				::Environment::ITimer& envTimer
			);
			Mixer() = delete;
			Mixer(const Mixer&) = delete;
			Mixer(Mixer&&) = delete;

			~Mixer();

			Mixer& operator=(const Mixer&) = delete;
			Mixer& operator=(Mixer&&) = delete;

			float get_Parameter(const ::std::string& name) const;
			float get_Level(long type, long channel) const;
			void set_Parameter(const ::std::string& name, float value);
			inline void set_Dirty() {
				m_dirty = true;
			};
			inline void set_Restart() {
				m_restart = ::std::chrono::system_clock::now();
			}

			virtual Type get_Type() const override;
			virtual Network& get_Network() override;
			virtual const Range<Input>& get_Inputs() const override;
			virtual const Range<Output>& get_Outputs() const override;
			virtual const Range<Output>& get_Plugs(const Input& input) const override;
			virtual const Range<Input>& get_Plugs(const Output& output) const override;
			virtual void set_Plug(const Input& input, const Output& output, bool value) override;
			void on_Plug(const Input& input, const Output& output, const ::std::function<void(bool)>& callback);

		private:
			T_VBVMR_INTERFACE m_remote;
			Type m_type;
			Network m_network;
			Range<Input> m_cInput;
			Range<Output> m_cOutput;
			::std::unordered_map<void*, Range<Output>> m_cInputPlugs;
			::std::unordered_map<void*, Range<Input>> m_cOutputPlugs;
			::std::unordered_map<::std::pair<void*, void*>, ::std::vector<::std::function<void(bool)>>> m_cCallback;
			bool m_dirty;
			::std::chrono::time_point<::std::chrono::system_clock> m_restart;
		};
	}
}