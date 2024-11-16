#include <algorithm>
#include <limits>
#include <utility>

#include "KnobStateChange.h"

using namespace ::Voicemeeter::UI::D2D::Policies;

static constexpr int MIN{ -13500 };
static constexpr int MAX{ 13500 };
static int BREAKPOINTS[]{ 700, 10000, 5 };

bool ::Voicemeeter::UI::D2D::Policies::KnobStateChange::Set(States::Knob& dst, States::Knob& src) const {
	bool result{ false };

	if (dst.gain != src.gain) {
		if (MAX < src.gain) {
			if (MAX != dst.gain) {
				dst.gain = MAX;
				result = true;
			}
		} else if (src.gain < MIN) {
			if (dst.gain != MIN) {
				dst.gain = MIN;
				result = true;
			}
		} else {
			dst.gain = src.gain;
			result = true;
		}
	}
	if (dst.enabled != src.enabled) {
		dst.enabled = src.enabled;
		result = true;
	}
	if (dst.pinned != src.pinned) {
		dst.pinned = src.pinned;
		result = true;
	}
	if (result || dst.enabled || dst.pinned) {
		dst.level = ::std::move(src.level);
	} else if (dst.level.size() != src.level.size()) {
		dst.level = ::std::move(src.level);
		result = true;
	} else {
		int lhs_max{ ::std::numeric_limits<int>::min() };
		int rhs_max{ ::std::numeric_limits<int>::min() };
		for (size_t i{ 0 }; i < src.level.size(); ++i) {
			lhs_max = ::std::max(lhs_max, dst.level[i]);
			rhs_max = ::std::max(rhs_max, src.level[i]);
			dst.level[i] = src.level[i];
		}
		if (lhs_max != rhs_max) {
			if (rhs_max < lhs_max) {
				::std::swap(lhs_max, rhs_max);
			}
			for (int breakpoint : BREAKPOINTS) {
				if (lhs_max < breakpoint && !(rhs_max < breakpoint)) {
					result = true;
					break;
				}
			}
		}
	}

	return result;
}