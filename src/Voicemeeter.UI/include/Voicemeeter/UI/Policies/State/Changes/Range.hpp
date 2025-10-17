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
							if (Default == dst) {
								return false;
							}
							dst = Default;
							return true;
						};
						inline bool SetNext(TState &dst) const {
							if (Max == dst) {
								return false;
							}
							if (Max - Delta < dst) {
								dst = Max;
							} else {
								dst += Delta;
							}
							return true;
						};
						inline bool SetPrevious(TState &dst) const {
							if (Min == dst) {
								return false;
							}
							if (dst < Min + Delta) {
								dst = Min;
							} else {
								dst -= Delta;
							}
							return true;
						};
						inline bool Set(TState &dst, TState &src) const {
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
						};
					};
				}
			}
		}
	}
}

#endif
