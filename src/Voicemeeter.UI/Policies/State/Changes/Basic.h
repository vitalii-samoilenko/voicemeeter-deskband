#pragma once

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					namespace Basic {
						template<
							typename TState,
							TState Value>
						struct Default {
							inline bool operator()(TState& dst) const {
								if (dst == Value) {
									return false;
								}
								dst = Value;
								return true;
							}
						};
					}
				}
			}
		}
	}
}