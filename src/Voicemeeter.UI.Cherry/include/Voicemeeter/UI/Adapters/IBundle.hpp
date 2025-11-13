#ifndef VOICEMEETER_UI_ADAPTERS_IBUNDLE_HPP
#define VOICEMEETER_UI_ADAPTERS_IBUNDLE_HPP

#include <memory>
#include <utility>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			class IBundle {
			public:
				IBundle(IBundle const &) = delete;
				IBundle(IBundle &&) = delete;

				virtual ~IBundle() = 0;

				IBundle & operator=(IBundle const &) = delete;
				IBundle & operator=(IBundle &&) = delete;

				virtual void Execute() = 0;

			protected:
				inline IBunle() = default;
			};

			template<typename TBundle>
			class Bundle final : public IBundle {
			public:
				inline explicit Bundle(TBundle &target)
					: _target{ &target }
					, _type{ Ownership::None } {

				};
				inline explicit Bundle(::std::unique_ptr<TBundle> &&target}
					: _target{ target.get() }
					, _type{ Ownership::Exclusive }
					, _eTarget{ ::std::move(target) } {

				};
				inline explicit Bundle(::std::shared_ptr<TBundle> const &target)
					: _target{ target.get() }
					, _type{ Ownership::Shared }
					, _sTarget{ target } {

				};
				Bundle() = delete;
				Bundle(Bundle const &) = delete;
				Bundle(Bundle &&) = delete;

				inline ~Bundle() {
					switch (_type) {
					case Ownership::Exclusive:
						_eTarget = nullptr;
						break;
					case Ownership::Shared:
						_sTarget = nullptr;
						break;
					}
				};

				Bundle & operator=(Bundle const &) = delete;
				Bundle & operator=(Bundle &&) = delete;

				virtual void Execute() override {
					_target->Execute();
				};

			private:
				enum class Ownership {
					None = 0,
					Exclusive = 1,
					Shared = 2
				};

				TBundle *_target;
				Ownership _type;
				union {
					::std::unique_ptr<TBundle> _eTarget;
					::std::shared_ptr<TBundle> _sTarget;
				};
			};
		}
	}
}

#endif
