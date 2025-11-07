#ifndef VOICEMEETER_UI_ADAPTERS_ICOMPONENT_HPP
#define VOICEMEETER_UI_ADAPTERS_ICOMPONENT_HPP

#include <memory>
#include <utility>
#include <valarray>

#include "Voicemeeter/UI/Focus.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			class IVisible {
			public:
				IVisible(IVisible const &) = delete;
				IVisible(IVisible &&) = delete;

				IVisible & operator=(IVisible const &) = delete;
				IVisible & operator=(IVisible &&) = delete;

				virtual ::std::valarray<int> const & get_Position() const = 0;
				virtual ::std::valarray<int> const & get_Size() const = 0;

				virtual void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) = 0;

			protected:
				inline IVisible() = default;

				inline ~IVisible() = default;
			};

			class IMovable {
			public:
				IMovable(IMovable const &) = delete;
				IMovable(IMovable &&) = delete;

				IMovable & operator=(IMovable const &) = delete;
				IMovable & operator=(IMovable &&) = delete;

				virtual void Move(::std::valarray<int> const &point) = 0;

			protected:
				inline IMovable() = default;

				inline ~IMovable() = default;
			};

			class IResizable {
			public:
				IResizable(IResizable const &) = delete;
				IResizable(IResizable &&) = delete;

				IResizable & operator=(IResizable const &) = delete;
				IResizable & operator=(IResizable &&) = delete;

				virtual void Resize(::std::valarray<int> const &vertex) = 0;

			protected:
				inline IResizable() = default;

				inline ~IResizable() = default;
			};

			class IScalable {
			public:
				IScalable(IScalable const &) = delete;
				IScalabel(IScalable &&) = delete;

				IScalable & operator=(IScalable const &) = delete;
				IScalable & operator=(IScalable &&) = delete;

				virtual ::std::valarray<int> const & get_BaseSize() const = 0;

				virtual void Rescale(::std::valarray<int> const &vertex) = 0;

			protected:
				inline IScalable() = default;

				inline ~IScalable() = default;
			};

			class IInteractive {
			public:
				IInteractive(IInteractive const &) = delete;
				IInteractive(IInteractive &&) = delete;

				IInteractive & operator=(IInteractive const &) = delete;
				IInteractive & operator=(IInteractive &&) = delete;

				virtual void Focus(Focus mode) = 0;
				virtual bool MouseLDown(::std::valarray<int> const &point) = 0;
				virtual bool MouseLDouble(::std::valarray<int> const &point) = 0;
				virtual bool MouseLUp(::std::valarray<int> const &point) = 0;
				virtual bool MouseMDown(::std::valarray<int> const &point) = 0;
				virtual bool MouseMDouble(::std::valarray<int> const &point) = 0;
				virtual bool MouseRDown(::std::valarray<int> const &point) = 0;
				virtual bool MouseRDouble(::std::valarray<int> const &point) = 0;
				virtual bool MouseWheel(::std::valarray<int> const &point, int delta) = 0;
				virtual bool MouseMove(::std::valarray<int> const &point) = 0;

			protected:
				inline IInteractive() = default;

				inline ~IInteractive() = default;
			};

			class IComponent :
				public IVisible,
				public IMovable,
				public IScalable,
				public IInteractive {
			public:
				IComponent(IComponent const &) = delete;
				IComponent(IComponent &&) = delete;

				virtual ~IComponent() = 0;

				IComponent & operator=(IComponent const &) = delete;
				IComponent & operator=(IComponent &&) = delete;

				virtual void * get_Target() const = 0;

			protected:
				inline IComponent() = default;
			};

			template<typename TComponent>
			class Component final : public IComponent {
			public:
				inline explicit Component(TComponent &target)
					: _target{ &target }
					, _type{ Ownership::None } {

				};
				inline explicit Component(::std::unique_ptr<TComponent> &target)
					: _target{ target.get() }
					, _type{ Ownership::Exclusive }
					, _eTarget{ ::std::move(target) } {

				};
				inline explicit Component(::std::shared_ptr<TComponent> const &target)
					: _target{ target.get() }
					, _type{ Ownership::Shared }
					, _sTarget{ target } {

				};
				Component() = delete;
				Component(Component const &) = delete;
				Component(Component &&) = delete;

				inline ~Component() {
					switch (_type) {
					case Ownership::Exclusive:
						_eTarget = nullptr;
						break;
					case Ownership::Shared:
						_sTarget = nullptr;
						break;
					};
				};

				Component & operator=(Component const &) = delete;
				Component & operator=(Component &&) = delete;

				virtual void * get_Target() override {
					return reinterpret_cast<void *>(_target);
				};

				virtual ::std::valarray<int> const & get_Position() const override {
					return _target->get_Position();
				};
				virtual ::std::valarray<int> const & get_Size() const override {
					return _target->get_Size();
				};
				virtual ::std::valarray<int> const & get_BaseSize() const override {
					return _target->get_BaseSize();
				};

				virtual void Redraw(::std::valarray<int> const &point, ::std::valarray<int> const &vertex) override {
					_target->Redraw(point, vertex);
				};
				virtual void Move(::std::valarray<int> const &point) override {
					_target->Move(point);
				};
				virtual void Rescale(::std::valarray<int> const &vertex) override {
					_target->Rescale(vertex);
				};
				virtual void Focus(Focus mode) override {
					_target->Focus(mode);
				};
				virtual bool MouseLDown(::std::valarray<int> const &point) override {
					return _target->MouseLDown(point);
				};
				virtual bool MouseLDouble(::std::valarray<int> const &point) override {
					return _target->MouseLDouble(point);
				};
				virtual bool MouseLUp(::std::valarray<int> const &point) override {
					return _target->MouseLUp(point);
				};
				virtual bool MouseMDown(::std::valarray<int> const &point) override {
					return _target->MouseMDown(point);
				};
				virtual bool MouseMDouble(::std::valarray<int> const &point) override {
					return _target->MouseMDouble(point);
				};
				virtual bool MouseRDown(::std::valarray<int> const &point) override {
					return _target->MouseRDown(point);
				};
				virtual bool MouseRDouble(::std::valarray<int> const &point) override {
					return _target->MouseRDouble(point);
				};
				virtual bool MouseWheel(::std::valarray<int> const &point, int delta) override {
					return _target->MouseWheel(point, delta);
				};
				virtual bool MouseMove(::std::valarray<int> const &point) override {
					return _target->MouseMove(point);
				};

			private:
				enum class Ownership {
					None = 0,
					Exclusive = 1,
					Shared = 2
				};

				TComponent *_target;
				Ownership _type;
				union {
					::std::unique_ptr<TComponent> _eTarget;
					::std::shared_ptr<TComponent> _sTarget;
				};
			};
		}
	}
}

#endif
