#ifndef VOICEMEETER_UI_ADAPTERS_ICOMPONENT_HPP
#define VOICEMEETER_UI_ADAPTERS_ICOMPONENT_HPP

#include "wheel.hpp"

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

				virtual vector_t const & get_Position() const = 0;
				virtual vector_t const & get_Size() const = 0;

				virtual void Redraw(vector_t const &point, vector_t const &vertex) = 0;

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

				virtual void Move(vector_t const &point) = 0;

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

				virtual void Resize(vector_t const &vertex) = 0;

			protected:
				inline IResizable() = default;

				inline ~IResizable() = default;
			};

			class IScalable {
			public:
				IScalable(IScalable const &) = delete;
				IScalable(IScalable &&) = delete;

				IScalable & operator=(IScalable const &) = delete;
				IScalable & operator=(IScalable &&) = delete;

				virtual vector_t const & get_BaseSize() const = 0;

				virtual void Rescale(vector_t const &vertex) = 0;

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

				virtual void set_Focus(Focus value) = 0;

				virtual bool MouseLDown(vector_t const &point) = 0;
				virtual bool MouseLDouble(vector_t const &point) = 0;
				virtual bool MouseLUp(vector_t const &point) = 0;
				virtual bool MouseMDown(vector_t const &point) = 0;
				virtual bool MouseMDouble(vector_t const &point) = 0;
				virtual bool MouseRDown(vector_t const &point) = 0;
				virtual bool MouseRDouble(vector_t const &point) = 0;
				virtual bool MouseWheel(vector_t const &point, num_t delta) = 0;
				virtual bool MouseMove(vector_t const &point) = 0;

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

			protected:
				inline IComponent() = default;
			};

			template<typename TComponent>
			class Component final : public IComponent {
			public:
				inline explicit Component(TComponent &target)
					: _target{ target } {

				};
				Component() = delete;
				Component(Component const &) = delete;
				Component(Component &&) = delete;

				inline ~Component() = default;

				Component & operator=(Component const &) = delete;
				Component & operator=(Component &&) = delete;

				virtual vector_t const & get_Position() const override {
					return _target.get_Position();
				};
				virtual vector_t const & get_Size() const override {
					return _target.get_Size();
				};
				virtual vector_t const & get_BaseSize() const override {
					return _target.get_BaseSize();
				};

				virtual void set_Focus(Focus value) override {
					_target.set_Focus(value);
				};

				virtual void Redraw(vector_t const &point, vector_t const &vertex) override {
					_target.Redraw(point, vertex);
				};
				virtual void Move(vector_t const &point) override {
					_target.Move(point);
				};
				virtual void Rescale(vector_t const &vertex) override {
					_target.Rescale(vertex);
				};
				virtual bool MouseLDown(vector_t const &point) override {
					return _target.MouseLDown(point);
				};
				virtual bool MouseLDouble(vector_t const &point) override {
					return _target.MouseLDouble(point);
				};
				virtual bool MouseLUp(vector_t const &point) override {
					return _target.MouseLUp(point);
				};
				virtual bool MouseMDown(vector_t const &point) override {
					return _target.MouseMDown(point);
				};
				virtual bool MouseMDouble(vector_t const &point) override {
					return _target.MouseMDouble(point);
				};
				virtual bool MouseRDown(vector_t const &point) override {
					return _target.MouseRDown(point);
				};
				virtual bool MouseRDouble(vector_t const &point) override {
					return _target.MouseRDouble(point);
				};
				virtual bool MouseWheel(vector_t const &point, num_t delta) override {
					return _target.MouseWheel(point, delta);
				};
				virtual bool MouseMove(vector_t const &point) override {
					return _target.MouseMove(point);
				};

			private:
				TComponent &_target;
			};
		}
	}
}

#endif
