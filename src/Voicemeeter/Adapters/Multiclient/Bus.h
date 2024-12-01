#pragma once

#include <type_traits>

#include "../../Bus.h"
#include "../../Multiclient/Manager.h"
#include "Strip.h"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			template<typename Specification, typename BusSpecification, typename TLine, typename TStrip>
			class Bus : private ::Voicemeeter::Bus<BusSpecification, TLine, TStrip> {
				using Base = ::Voicemeeter::Bus<BusSpecification, TLine, TStrip>;

				static_assert(
					::std::is_base_of_v<Strip<Specification, typename BusSpecification::Strip, TLine>, TStrip>,
					"TStrip must be derived from Strip");

			public:
				using Base::Base;

				Bus() = delete;
				Bus(const Bus&) = delete;
				inline Bus(Bus&&) = default;

				inline ~Bus() = default;

				Bus& operator=(const Bus&) = delete;
				Bus& operator=(Bus&&) = delete;

				using Base::begin;
				using Base::end;
			};
		}
	}
}