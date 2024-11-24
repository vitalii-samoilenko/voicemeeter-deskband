#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					namespace Range {
						template<
							typename TState,
							TState Max,
							TState Delta>
						struct Next {
							inline bool operator()(TState& dst) const {
								if (Max == dst) {
									return false;
								}
								if (Max - Delta < dst) {
									dst = Max;
								} else {
									dst += Delta;
								}
								return true;
							}
						};
						template<
							typename TState,
							TState Min,
							TState Delta>
						struct Previous {
							inline bool operator()(TState& dst) const {
								if (Min == dst) {
									return false;
								}
								if (dst < Min + Delta) {
									dst = Min;
								} else {
									dst -= Delta;
								}
								return true;
							}
						};
						template<
							typename TState,
							TState Min,
							TState Max>
						struct Set {
							inline bool operator()(TState& dst, TState& src) const {
								if (dst == src) {
									return false;
								}
								if (src < Min) {
									if (dst == Min) {
										return false;
									}
									dst = Min;
								} else if (Max < src) {
									if (dst == Max) {
										return false;
									}
									dst = Max;
								} else {
									dst = src;
								}
								return true;
							}
						};
					}
				}
			}
		}
	}
}