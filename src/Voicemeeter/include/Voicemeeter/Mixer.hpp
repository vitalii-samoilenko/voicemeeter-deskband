#ifndef VOICEMEETER_MIXER_HPP
#define VOICEMEETER_MIXER_HPP

#include <bitset>
#include <type_traits>
#include <utility>

#include "Voicemeeter/Group.hpp"

namespace Voicemeeter {
	template<
		size_t PISize, typename TPIBus,
		size_t VISize, typename TVIBus,
		size_t POSize, typename TPOBus,
		size_t VOSize, typename TVOBus>
	class Mixer {
	public:
		inline Mixer() = default;
		Mixer(Mixer const &) = delete;
		Mixer(Mixer &&) = delete;

		inline ~Mixer() = default;

		Mixer & operator=(Mixer const &) = delete;
		Mixer & operator=(Mixer &&) = delete;

		template<typename TIBus, typename TOBus>
		inline bool get_Plug(TIBus const &input, TOBus const &output) const {
			size_t i{
				::std::is_same_v<TIBus, TPIBus>
					? (&input - _piGroup.begin())
					: (PISize + &input - _viGroup.begin())
			};
			size_t j{
				::std::is_same_v<TOBus, TPOBus>
					? (&output - _poGroup.begin())
					: (POsize + &output - _voGroup.begin())
			};
			return _plugs[(POSize + VOSize) * i + j];
		};
		template<typename TIBus, typename TOBus>
		inline void set_Plug(TIBus const &input, TOBus const &output, bool value) {
			size_t i{
				::std::is_same_v<TIBus, TPIBus>
					? (&input - _piGroup.begin())
					: (PISize + &input - _viGroup.begin())
			};
			size_t j{
				::std::is_same_v<TOBus, TPOBus>
					? (&output - _poGroup.begin())
					: (POsize + &output - _voGroup.begin())
			};
			_plugs[(POSize + VOSize) * i + j] = value;
		};

		inline Group<PISize, TPIBus> & get_PIGroup() {
			return _piGroup;
		};
		inline Group<VISize, TVIBus> & get_VIGroup() {
			return _viGroup;
		};
		inline Group<POSize, TPOBus> & get_POGroup() {
			return _poGroup;
		};
		inline Group<VOSize, TVOBus> & get_VOGroup() {
			return _voGroup;
		};


	private:
		Group<PISize, TPIBus> _piGroup;
		Group<VISize, TVIBus> _viGroup;
		Group<POSize, TPOBus> _poGroup;
		Group<VOSize, TVOBus> _voGroup;
		::std::bitset<
			(PISize + VISize)
			* (POSize + VOSize)
		> _plugs;	
	};
}

#endif
