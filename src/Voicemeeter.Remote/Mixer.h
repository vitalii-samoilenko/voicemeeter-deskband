#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Environment/ITimer.h"
#include "Voicemeeter/IMixer.h"

#include "Api.h"
#include "Input.h"
#include "Network.h"
#include "Output.h"

template<>
struct ::std::hash<::std::reference_wrapper<::Voicemeeter::Remote::Input>> {
	size_t operator()(const ::std::reference_wrapper<::Voicemeeter::Remote::Input>& rInput) const {
		return reinterpret_cast<size_t>(&(rInput.get()));
	}
};
template<>
struct ::std::hash<::std::reference_wrapper<::Voicemeeter::Remote::Output>> {
	size_t operator()(const ::std::reference_wrapper<::Voicemeeter::Remote::Output>& rOutput) const {
		return reinterpret_cast<size_t>(&(rOutput.get()));
	}
};
template<>
struct ::std::hash<::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>> {
	size_t operator()(const ::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>& key) const {
		return ::std::hash<::std::reference_wrapper<::Voicemeeter::Remote::Input>>{}(key.first)
			^ ::std::hash<::std::reference_wrapper<::Voicemeeter::Remote::Output>>{}(key.second);
	}
};
template<>
struct ::std::equal_to<::std::reference_wrapper<::Voicemeeter::Remote::Input>> {
	bool operator()(const ::std::reference_wrapper<::Voicemeeter::Remote::Input>& lhs, const ::std::reference_wrapper<::Voicemeeter::Remote::Input>& rhs) const {
		return &(lhs.get()) == &(rhs.get());
	}
};
template<>
struct ::std::equal_to<::std::reference_wrapper<::Voicemeeter::Remote::Output>> {
	bool operator()(const ::std::reference_wrapper<::Voicemeeter::Remote::Output>& lhs, const ::std::reference_wrapper<::Voicemeeter::Remote::Output>& rhs) const {
		return &(lhs.get()) == &(rhs.get());
	}
};
template<>
struct ::std::equal_to<::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>> {
	bool operator()(const ::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>& lhs, const ::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>& rhs) const {
		return ::std::equal_to<::std::reference_wrapper<::Voicemeeter::Remote::Input>>{}(lhs.first, rhs.first)
			&& ::std::equal_to<::std::reference_wrapper<::Voicemeeter::Remote::Output>>{}(lhs.second, rhs.second);
	}
};

namespace Voicemeeter {
	namespace Remote {
		namespace {
			using input_reference_hash = ::std::hash<::std::reference_wrapper<::Voicemeeter::Remote::Input>>;
			using input_reference_equal_to = ::std::equal_to<::std::reference_wrapper<::Voicemeeter::Remote::Input>>;
			using output_reference_hash = ::std::hash<::std::reference_wrapper<::Voicemeeter::Remote::Output>>;
			using output_reference_equal_to = ::std::equal_to<::std::reference_wrapper<::Voicemeeter::Remote::Output>>;
			using input_output_reference_hash = ::std::hash<::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>>;
			using input_output_reference_equal = ::std::equal_to<::std::pair<::std::reference_wrapper<::Voicemeeter::Remote::Input>, ::std::reference_wrapper<::Voicemeeter::Remote::Output>>>;
		}

		class Mixer : public IMixer<
			Input,
			::std::vector<::std::unique_ptr<Input>>,
			::std::unordered_set<::std::reference_wrapper<Input>, input_reference_hash, input_reference_equal_to>,
			Output,
			::std::vector<::std::unique_ptr<Output>>,
			::std::unordered_set<::std::reference_wrapper<Output>, output_reference_hash, output_reference_equal_to>> {
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
				m_restart = ::std::chrono::high_resolution_clock::now();
			}

			virtual Type get_Type() const override;
			virtual Network& get_Network() override;
			virtual const ::std::vector<::std::unique_ptr<Input>>& get_Inputs() const override;
			virtual const ::std::vector<::std::unique_ptr<Output>>& get_Outputs() const override;
			virtual const ::std::unordered_set<::std::reference_wrapper<Output>,
				output_reference_hash, output_reference_equal_to>& get_Plugs(const Input& input) const override;
			virtual const ::std::unordered_set<::std::reference_wrapper<Input>,
				input_reference_hash, input_reference_equal_to>& get_Plugs(const Output& output) const override;
			virtual void set_Plug(const Input& input, const Output& output, bool value) override;
			void on_Plug(const Input& input, const Output& output, const ::std::function<void(bool)>& callback);

		private:
			T_VBVMR_INTERFACE m_remote;
			Type m_type;
			Network m_network;
			::std::vector<::std::unique_ptr<Input>> m_cpInput;
			::std::vector< ::std::unique_ptr<Output>> m_cpOutput;
			::std::unordered_map<::std::reference_wrapper<Input>, ::std::unordered_set<::std::reference_wrapper<Output>,
					output_reference_hash, output_reference_equal_to>,
				input_reference_hash, input_reference_equal_to> m_cInputPlugs;
			::std::unordered_map<::std::reference_wrapper<Output>, ::std::unordered_set<::std::reference_wrapper<Input>,
					input_reference_hash, input_reference_equal_to>,
				output_reference_hash, output_reference_equal_to> m_cOutputPlugs;
			::std::unordered_map<::std::pair<::std::reference_wrapper<Input>, ::std::reference_wrapper<Output>>, ::std::vector<::std::function<void(bool)>>,
				input_output_reference_hash, input_output_reference_equal> m_cCallback;
			bool m_dirty;
			::std::chrono::high_resolution_clock::time_point m_restart;
		};
	}
}