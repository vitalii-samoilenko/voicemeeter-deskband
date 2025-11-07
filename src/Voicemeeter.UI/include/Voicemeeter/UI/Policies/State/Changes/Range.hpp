#ifndef VOICEMEETER_UI_POLICIES_STATE_CHANGES_RANGE_HPP
#define VOICEMEETER_UI_POLICIES_STATE_CHANGES_RANGE_HPP

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace State {
				namespace Changes {
					template<
						typename TState,
						TState Default,
						TState Min,
						TState Max,
						TState Delta>
					struct Range {
						inline bool SetDefault(TState &dst) const {
							dst = Default;
						};
						inline bool SetNext(TState &dst) const {
							if (Max - Delta < dst) {
								dst = Max;
							} else {
								dst += Delta;
							}
						};
						inline bool SetPrevious(TState &dst) const {
							if (dst < Min + Delta) {
								dst = Min;
							} else {
								dst -= Delta;
							}
						};
						inline bool Set(TState &dst, TState &src) const {
							if (src < Min) {
								dst = Min;
							} else if (Max < src) {
								dst = Max;
							} else {
								dst = src;
							}
						};
					};
				}
			}
		}
	}
}

#endif
