#pragma once

#include <utility>

#include "../../../States/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					namespace Knob {
						template<
							int Value>
						struct Default {
							inline bool operator()(States::Knob& dst) const {
								if (dst.gain == Value) {
									return false;
								}
								dst.gain = Value;
								return true;
							}
						};
						template<
							int Max,
							int Delta>
						struct Next {
							inline bool operator()(States::Knob& dst) const {
								if (Max == dst.gain) {
									return false;
								}
								if (Max - Delta < dst.gain) {
									dst.gain = Max;
								} else {
									dst.gain += Delta;
								}
								dst.hold = true;
								return true;
							}
						};
						template<
							int Min,
							int Delta>
						struct Previous {
							inline bool operator()(States::Knob& dst) const {
								if (dst.gain == Min) {
									return false;
								}
								if (dst.gain < Min + Delta) {
									dst.gain = Min;
								} else {
									dst.gain -= Delta;
								}
								dst.hold = true;
								return true;
							}
						};
						template<
							int Min,
							int Max>
						struct Set {
							inline bool operator()(States::Knob& dst, States::Knob& src) const {
								bool result{ false };
								if (dst.id != src.id) {
									dst.id = src.id;
									result = true;
								}
								if (dst.gain != src.gain) {
									if (Max < src.gain) {
										if (Max != dst.gain) {
											dst.gain = Max;
											result = true;
										}
									} else if (src.gain < Min) {
										if (dst.gain != Min) {
											dst.gain = Min;
											result = true;
										}
									} else {
										dst.gain = src.gain;
										result = true;
									}
								}
								if (dst.level.size() != src.level.size()
									|| (dst.level != src.level).max()) {
									dst.level = ::std::move(src.level);
									result = true;
								}
								if (dst.toggle != src.toggle) {
									dst.toggle = src.toggle;
									result = true;
								}
								if (dst.hold != src.hold) {
									dst.hold = src.hold;
									result = true;
								}
								return result;
							}
						};
					}
				}
			}
		}
	}
}