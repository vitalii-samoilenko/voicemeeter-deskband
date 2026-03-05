#ifndef WUI_ADAPTERS_ICOMPONENT_HPP
#define WUI_ADAPTERS_ICOMPONENT_HPP

#include "math.hpp"

#include "WUI/Focus.hpp"

namespace WUI {
	namespace Adapters {
		class IVisible {
		public:
			IVisible(IVisible const &) = delete;
			IVisible(IVisible &&) = delete;

			IVisible & operator=(IVisible const &) = delete;
			IVisible & operator=(IVisible &&) = delete;

			virtual vec_t const & get_Position() const = 0;
			virtual vec_t const & get_Size() const = 0;

			virtual void Redraw(vec_t const &point, vec_t const &vertex) = 0;

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

			virtual void Move(vec_t const &point) = 0;

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

			virtual void Resize(vec_t const &vertex) = 0;

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

			virtual vec_t const & get_BaseSize() const = 0;

			virtual void Rescale(vec_t const &vertex) = 0;

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

			virtual bool MouseLDown(vec_t const &point) = 0;
			virtual bool MouseLDouble(vec_t const &point) = 0;
			virtual bool MouseLUp(vec_t const &point) = 0;
			virtual bool MouseMDown(vec_t const &point) = 0;
			virtual bool MouseMDouble(vec_t const &point) = 0;
			virtual bool MouseRDown(vec_t const &point) = 0;
			virtual bool MouseRDouble(vec_t const &point) = 0;
			virtual bool MouseWheel(vec_t const &point, num_t delta) = 0;
			virtual bool MouseMove(vec_t const &point) = 0;

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

			virtual ~IComponent() {};

			IComponent & operator=(IComponent const &) = delete;
			IComponent & operator=(IComponent &&) = delete;

		protected:
			inline IComponent() = default;
		};

		template<typename TComponent>
		class Component final : public IComponent {
		public:
			inline explicit Component(TComponent *that)
				: that{ that } {

			};
			Component() = delete;
			Component(Component const &) = delete;
			Component(Component &&) = delete;

			inline ~Component() = default;

			Component & operator=(Component const &) = delete;
			Component & operator=(Component &&) = delete;

			virtual vec_t const & get_Position() const override {
				return that->get_Position();
			};
			virtual vec_t const & get_Size() const override {
				return that->get_Size();
			};
			virtual vec_t const & get_BaseSize() const override {
				return that->get_BaseSize();
			};

			virtual void set_Focus(Focus value) override {
				that->set_Focus(value);
			};

			virtual void Redraw(vec_t const &point, vec_t const &vertex) override {
				that->Redraw(point, vertex);
			};
			virtual void Move(vec_t const &point) override {
				that->Move(point);
			};
			virtual void Rescale(vec_t const &vertex) override {
				that->Rescale(vertex);
			};
			virtual bool MouseLDown(vec_t const &point) override {
				return that->MouseLDown(point);
			};
			virtual bool MouseLDouble(vec_t const &point) override {
				return that->MouseLDouble(point);
			};
			virtual bool MouseLUp(vec_t const &point) override {
				return that->MouseLUp(point);
			};
			virtual bool MouseMDown(vec_t const &point) override {
				return that->MouseMDown(point);
			};
			virtual bool MouseMDouble(vec_t const &point) override {
				return that->MouseMDouble(point);
			};
			virtual bool MouseRDown(vec_t const &point) override {
				return that->MouseRDown(point);
			};
			virtual bool MouseRDouble(vec_t const &point) override {
				return that->MouseRDouble(point);
			};
			virtual bool MouseWheel(vec_t const &point, num_t delta) override {
				return that->MouseWheel(point, delta);
			};
			virtual bool MouseMove(vec_t const &point) override {
				return that->MouseMove(point);
			};

		private:
			TComponent *that;
		};
	}
}

#endif
