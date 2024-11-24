#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					namespace Circular {
						template<
							typename TState,
							TState Min,
							TState Max,
							TState Delta>
						struct Next {
							inline bool operator()(TState& dst) const {
								if (Max - Delta < dst) {
									dst = Min;
								} else {
									dst += Delta;
								}
								return true;
							}
						};
						template<
							typename TState,
							TState Min,
							TState Max,
							TState Delta>
						struct Previous {
							inline bool operator()(TState& dst) const {
								if (dst < Min + Delta) {
									dst = Max;
								} else {
									dst -= Delta;
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