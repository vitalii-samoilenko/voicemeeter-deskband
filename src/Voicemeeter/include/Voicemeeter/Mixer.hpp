#ifndef VOICEMEETER_MIXER_HPP
#define VOICEMEETER_MIXER_HPP

#include <bitset>
#include <type_traits>
#include <utility>

#include "Voicemeeter/Group.hpp"

namespace Voicemeeter {
	template<
		size_t PISize, typename TPIStrip,
		size_t VISize, typename TVIStrip,
		size_t POSize, typename TPOStrip,
		size_t VOSize, typename TVOStrip>
	class Mixer {
	public:
		template<typename... Args>
		inline explicit Mixer(Args ...&&args)
			: _piGroup{ ::std::forward<Args>(args)... }
			, _viGroup{ ::std::forward<Args>(args)... }
			, _poGroup{ ::std::forward<Args>(args)... }
			, _voGroup{ ::std::forward<Args>(args)... } {

		};
		Mixer() = delete;
		Mixer(Mixer const &) = delete;
		Mixer(Mixer &&) = delete;

		inline ~Mixer() = default;

		Mixer & operator=(Mixer const &) = delete;
		Mixer & operator=(Mixer &&) = delete;

		template<typename TIStrip, typename TOStrip>
		inline bool get_Plug(TIStrip const &input, TOStrip const &output) const {
			size_t i{
				::std::is_same_v<TIStrip, TPIStrip>
					? (&input - _piGroup.begin())
					: (PISize + &input - _viGroup.begin())
			};
			size_t j{
				::std::is_same_v<TOStrip, TPOStrip>
					? (&output - _poGroup.begin())
					: (POsize + &output - _voGroup.begin())
			};
			return _plugs[(POSize + VOSize) * i + j];
		};
		template<typename TIStrip, typename TOStrip>
		inline void set_Plug(TIStrip const &input, TOStrip const &output, bool value) {
			size_t i{
				::std::is_same_v<TIStrip, TPIStrip>
					? (&input - _piGroup.begin())
					: (PISize + &input - _viGroup.begin())
			};
			size_t j{
				::std::is_same_v<TOStrip, TPOStrip>
					? (&output - _poGroup.begin())
					: (POsize + &output - _voGroup.begin())
			};
			_plugs[(POSize + VOSize) * i + j] = value;
		};

		inline Group<PISize, TPIStrip> & get_PIGroup() {
			return _piGroup;
		};
		inline Group<VISize, TVIStrip> & get_VIGroup() {
			return _viGroup;
		};
		inline Group<POSize, TPOStrip> & get_POGroup() {
			return _poGroup;
		};
		inline Group<VOSize, TVOStrip> & get_VOGroup() {
			return _voGroup;
		};


	private:
		Group<PISize, TPIStrip> _piGroup;
		Group<VISize, TVIStrip> _viGroup;
		Group<POSize, TPOStrip> _poGroup;
		Group<VOSize, TVOStrip> _voGroup;
		::std::bitset<
			(PISize + VISize)
			* (POSize + VOSize)
		> _plugs;	
	};
}

#endif
