#ifndef VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_VBAN_HPP
#define VOICEMEETER_UI_POLICIES_CONTROL_UPDATES_VBAN_HPP

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			namespace Control {
				namespace Updates {
					template<typename TToolkit, typename TVban>
					class Vban {
					public:
						inline explicit Vban(TToolkit &toolkit)
							: _toolkit{ toolkit } {

						};
						Vban() = delete;
						Vban(Vban const &) = delete;
						inline Vban(Vban &&) = default;

						inline ~Vban() = default;

						Vban & operator=(Vban const &) = delete;
						Vban & operator=(Vban &&) = delete;

						inline void operator()(
							TVban &control, typename TVban::state_t state) const {
							glyph.set_FrameColor(
								state
									? _toolkit.get_Theme()
										.Information
									: _toolkit.get_Theme()
										.Inactive
							);
						};

					private:
						TToolkit &_toolkit;
					};
				}
			}
		}
	}
}

#endif
