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
		static_assert(
			::std::is_move_constructible_v<TPInput>,
			"TPInput must be move constructible");
		static_assert(
			::std::is_move_constructible_v<TVInput>,
			"TVInput must be move constructible");
		static_assert(
			::std::is_move_constructible_v<TPOutput>,
			"TPOutput must be move constructible");
		static_assert(
			::std::is_move_constructible_v<TVOutput>,
			"TVOutput must be move constructible");

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
				(::std::is_same_v<TPIStrip, TInput> || ::std::is_same_v<TVIStrip, TInput>)
				&& (::std::is_same_v<TPOStrip, TOutput> || ::std::is_same_v<TVOStrip, TOutput>),
				bool> = true>
		inline bool get_Plug(TInput& input, TOutput& output) const {
			constexpr size_t InputOffset{
				(::std::is_same_v<TPIStrip, TInput>
					? 0
					: Specification::Input::Physical::Width)
			};
			constexpr size_t OutputOffset{
				(::std::is_same_v<TPOStrip, TOutput>
					? 0
					: Specification::Output::Physical::Width)
			};
			size_t iInput{
				(::std::is_same_v<TPIStrip, TInput>
					? reinterpret_cast<unsigned long long>(&input) - reinterpret_cast<unsigned long long>(&(*m_physicalInput.begin()))
					: reinterpret_cast<unsigned long long>(&input) - reinterpret_cast<unsigned long long>(&(*m_virtualInput.begin()))) / sizeof(TInput)
			};
			size_t iOutput{
				(::std::is_same_v<TPOStrip, TOutput>
					? reinterpret_cast<unsigned long long>(&output) - reinterpret_cast<unsigned long long>(&(*m_physicalOutput.begin()))
					: reinterpret_cast<unsigned long long>(&output) - reinterpret_cast<unsigned long long>(&(*m_virtualOutput.begin()))) / sizeof(TOutput)
			};
			return m_cPlug[(InputOffset + iInput)
				* (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)
				+ (OutputOffset + iOutput)];
		};
		template<typename TInput, typename TOutput,
			::std::enable_if_t<
				(::std::is_same_v<TPIStrip, TInput> || ::std::is_same_v<TVIStrip, TInput>)
				&& (::std::is_same_v<TPOStrip, TOutput> || ::std::is_same_v<TVOStrip, TOutput>),
				bool> = true>
		inline void set_Plug(TInput& input, TOutput& output, bool value) {
			constexpr size_t InputOffset{
				(::std::is_same_v<TPIStrip, TInput>
					? 0
					: Specification::Input::Physical::Width)
			};
			constexpr size_t OutputOffset{
				(::std::is_same_v<TPOStrip, TOutput>
					? 0
					: Specification::Output::Physical::Width)
			};
			size_t iInput{
				(::std::is_same_v<TPIStrip, TInput>
					? reinterpret_cast<unsigned long long>(&input) - reinterpret_cast<unsigned long long>(&(*m_physicalInput.begin()))
					: reinterpret_cast<unsigned long long>(&input) - reinterpret_cast<unsigned long long>(&(*m_virtualInput.begin()))) / sizeof(TInput)
			};
			size_t iOutput{
				(::std::is_same_v<TPOStrip, TOutput>
					? reinterpret_cast<unsigned long long>(&output) - reinterpret_cast<unsigned long long>(&(*m_physicalOutput.begin()))
					: reinterpret_cast<unsigned long long>(&output) - reinterpret_cast<unsigned long long>(&(*m_virtualOutput.begin()))) / sizeof(TOutput)
			};
			m_cPlug[(InputOffset + iInput)
				* (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)
				+ (OutputOffset + iOutput)] = value;
		};

		inline TPInput& get_PhysicalInput() {
			return m_physicalInput;
		};
		inline TVInput& get_VirtualInput() {
			return m_virtualInput;
		};
		inline TPOutput& get_PhysicalOutput() {
			return m_physicalOutput;
		};
		inline TVOutput& get_VirtualOutput() {
			return m_virtualOutput;
		};

	private:
		mutable TPInput m_physicalInput;
		mutable TVInput m_virtualInput;
		mutable TPOutput m_physicalOutput;
		mutable TVOutput m_virtualOutput;
		::std::bitset<(Specification::Input::Physical::Width + Specification::Input::Virtual::Width) * (Specification::Output::Physical::Width + Specification::Output::Virtual::Width)> m_cPlug;
	};
}