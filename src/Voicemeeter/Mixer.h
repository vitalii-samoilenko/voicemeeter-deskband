#pragma once

#include <bitset>
#include <type_traits>
#include <utility>

#include "Bus.h"

namespace Voicemeeter {
	template<typename Specification,
		typename TPILine, typename TPIStrip, typename TPInput,
		typename TVILine, typename TVIStrip, typename TVInput,
		typename TPOLine, typename TPOStrip, typename TPOutput,
		typename TVOLine, typename TVOStrip, typename TVOutput>
	class Mixer {
		static_assert(
			::std::is_base_of_v<Bus<typename Specification::Input::Physical, TPILine, TPIStrip>, TPInput>,
			"TPInput must be derived from Bus");
		static_assert(
			::std::is_base_of_v<Bus<typename Specification::Input::Virtual, TVILine, TVIStrip>, TVInput>,
			"TVInput must be derived from Bus");
		static_assert(
			::std::is_base_of_v<Bus<typename Specification::Output::Physical, TPOLine, TPOStrip>, TPOutput>,
			"TPOutput must be derived from Bus");
		static_assert(
			::std::is_base_of_v<Bus<typename Specification::Output::Virtual, TVOLine, TVOStrip>, TVOutput>,
			"TVOutput must be derived from Bus");
		//static_assert(
		//	::std::is_move_constructible_v<TPInput>,
		//	"TPInput must be move constructible");
		//static_assert(
		//	::std::is_move_constructible_v<TVInput>,
		//	"TVInput must be move constructible");
		//static_assert(
		//	::std::is_move_constructible_v<TPOutput>,
		//	"TPOutput must be move constructible");
		//static_assert(
		//	::std::is_move_constructible_v<TVOutput>,
		//	"TVOutput must be move constructible");

	public:
		inline Mixer(
			TPInput&& physicalInput,
			TVInput&& virtualInput,
			TPOutput&& physicalOutput,
			TVOutput&& virtualOutput
		) : m_physicalInput{ ::std::move(physicalInput) }
		  , m_virtualInput{ ::std::move(virtualInput) }
		  , m_physicalOutput{ ::std::move(physicalOutput) }
		  , m_virtualOutput{ ::std::move(virtualOutput) }
		  , m_cPlug{} {

		};
		Mixer() = delete;
		Mixer(const Mixer&) = delete;
		Mixer(Mixer&&) = delete;

		inline ~Mixer() = default;

		Mixer& operator=(const Mixer&) = delete;
		Mixer& operator=(Mixer&&) = delete;

		template<typename TInput, typename TOutput,
			::std::enable_if_t<
				(::std::is_same_v<TPInput, TInput> || ::std::is_same_v<TVInput, TInput>)
				&& (::std::is_same_v<TPOutput, TOutput> || ::std::is_same_v<TVOutput, TOutput>),
				bool> = true>
		inline bool get_Plug(decltype(TInput::begin()) input, decltype(TOutput::begin()) output) const {
			constexpr size_t InputOffset{
				(::std::is_same_v<TPInput, TInput>
					? 0
					: Specification::Input::Physical::Width)
			};
			constexpr size_t OutputOffset{
				(::std::is_same_v<TPOutput, TOutput>
					? 0
					: Specification::Output::Physical::Width)
			};
			const TInput& m_input{
				(::std::is_same_v<TPInput, TInput>
					? m_physicalInput
					: m_virtualInput)
			};
			const TOutput& m_output{
				(::std::is_same_v<TPOutput, TOutput>
					? m_physicalOutput
					: m_virtualOutput)
			};
			return m_cPlug[(InputOffset + m_input.begin() - input)
				* (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)
				+ (OutputOffset + m_output.begin() - output)];
		};
		template<typename TInput, typename TOutput,
			::std::enable_if_t<
					(::std::is_same_v<TPInput, TInput> || ::std::is_same_v<TVInput, TInput>)
					&& (::std::is_same_v<TPOutput, TOutput> || ::std::is_same_v<TVOutput, TOutput>),
					bool> = true>
		inline void set_Plug(decltype(TInput::begin()) input, decltype(TOutput::begin()) output, bool value) {
			constexpr size_t InputOffset{
				(::std::is_same_v<TPInput, TInput>
					? 0
					: Specification::Input::Physical::Width)
			};
			constexpr size_t OutputOffset{
				(::std::is_same_v<TPOutput, TOutput>
					? 0
					: Specification::Output::Physical::Width)
			};
			const TInput& m_input{
				(::std::is_same_v<TPInput, TInput>
					? m_physicalInput
					: m_virtualInput)
			};
			const TOutput& m_output{
				(::std::is_same_v<TPOutput, TOutput>
					? m_physicalOutput
					: m_virtualOutput)
			};
			m_cPlug[(InputOffset + m_input.begin() - input)
				* (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)
				+ (OutputOffset + m_output.begin() - output)] = value;
		};

		const TPInput& get_PhysicalInput() const {
			return m_physicalInput;
		};
		const TVInput& get_VirtualInput() const {
			return m_virtualInput;
		};
		const TPOutput& get_PhysicalOutput() const {
			return m_physicalOutput;
		};
		const TVOutput& get_VirtualOutput() const {
			return m_virtualOutput;
		};

	private:
		TPInput m_physicalInput;
		TVInput m_virtualInput;
		TPOutput m_physicalOutput;
		TVOutput m_virtualOutput;
		::std::bitset<(Specification::Input::Physical::Width + Specification::Input::Virtual::Width) * (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)> m_cPlug;
	};
}