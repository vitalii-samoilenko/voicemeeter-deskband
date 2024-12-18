#pragma once

#include <cmath>
#include <string>
#include <utility>

#include "Voicemeeter.UI/States/Knob.h"

namespace Voicemeeter {
	namespace UI {
		namespace D2D {
			namespace Policies {
				namespace State {
					namespace Changes {
						namespace Knob {
							static constexpr int DEFAULT{ 9000 };
							static constexpr int MIN{ -13500 };
							static constexpr int MAX{ 13500 };
							static constexpr int DELTA{ 1 };
							static constexpr int BREAKPOINTS[]{ 700, 10000, 5 };

							inline static ::std::wstring ToLabel(size_t id) {
								switch (id) {
								case 0:
									return L"P";
								case 1:
									return L"V";
								case 2:
									return L"A1";
								case 3:
									return L"A2";
								case 4:
									return L"B1";
								case 5:
									return L"B2";
								default:
									return L"";
								}
							};
							inline static ::std::wstring ToLabel(int gain) {
								return ::std::to_wstring(::std::abs(
									static_cast<int>(
										::std::floor((gain - DEFAULT) / ((MAX - MIN) / 72.)))));
							};

							struct Set {
								inline bool operator()(States::Knob& dst, States::Knob& src) const {
									bool result{ false };
									if (dst.id != src.id) {
										dst.id = src.id;
										result = true;
									}
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
									if (dst.toggle != src.toggle) {
										dst.toggle = src.toggle;
										result = true;
									}
									if (dst.hold != src.hold) {
										dst.hold = src.hold;
										result = true;
									}
									if (result || dst.toggle || dst.hold) {
										dst.level = ::std::move(src.level);
									} else if (dst.level.size() != src.level.size()) {
										dst.level = ::std::move(src.level);
										result = true;
									} else {
										int lhs_max{ dst.level.max() };
										int rhs_max{ src.level.max() };
										dst.level = ::std::move(src.level);
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
							};
						}
					}
				}
			}
		}
	}
}