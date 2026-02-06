#ifndef VOICEMEETER_UI_ADAPTERS_IBUNDLE_HPP
#define VOICEMEETER_UI_ADAPTERS_IBUNDLE_HPP

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			class IBundle {
			public:
				IBundle(IBundle const &) = delete;
				IBundle(IBundle &&) = delete;

				virtual ~IBundle() {};

				IBundle & operator=(IBundle const &) = delete;
				IBundle & operator=(IBundle &&) = delete;

				virtual void operator()() = 0;

			protected:
				inline IBundle() = default;
			};

			template<typename TBundle>
			class Bundle final : public IBundle {
			public:
				inline explicit Bundle(TBundle &target)
					: _target{ target } {

				};
				Bundle() = delete;
				Bundle(Bundle const &) = delete;
				Bundle(Bundle &&) = delete;

				inline ~Bundle() = default;

				Bundle & operator=(Bundle const &) = delete;
				Bundle & operator=(Bundle &&) = delete;

				virtual void operator()() override {
					_target();
				};

			private:
				TBundle &_target;
			};
		}
	}
}

#endif
