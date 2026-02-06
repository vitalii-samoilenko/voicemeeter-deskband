#ifndef WINDOWS_TIMER_HPP
#define WINDOWS_TIMER_HPP

#include <memory>
#include <utility>

#include "wheel.hpp"

#include "Windows/API.hpp"

namespace Windows {
	class Timer final {
	public:
		inline explicit Timer(HWND hWnd)
			: _hWnd{ hWnd }
			, _target{ nullptr }
			, _targetId{ nullptr } {

		};
		Timer(Timer const &) = delete;
		Timer(Timer &&) = delete;

		inline ~Timer() = default;

		Timer & operator=(Timer const &) = delete;
		Timer & operator=(Timer &&) = delete;

		inline UINT_PTR get_Id() const {
			return reinterpret_cast<UINT_PTR>(this);
		};

		template<typename TTick>
		inline void Set(num_t duration, TTick &target) {
			::Windows::SetTimer(_hWnd, get_Id(),
				static_cast<UINT>(duration), NULL);
			_target = ::std::make_unique<
				Tick<TTick>>(
				target);
			_targetId = &target;
		};
		template<typename TTick>
		inline void Unset(TTick &target) {
			if (_targetId != &target) {
				return;
			}
			::Windows::KillTimer(_hWnd, get_Id());
			_target = nullptr;
			_targetId = nullptr;
		};

		inline void Elapse() {
			_target->operator()();
		};

	private:
		class ITick {
		public:
			ITick(ITick const &) = delete;
			ITick(ITick &&) = delete;

			virtual ~ITick() {};

			ITick & operator=(ITick const &) = delete;
			ITick & operator=(ITick &&) = delete;

			virtual void operator()() = 0;

		protected:
			inline ITick() = default;
		};

		template<typename TTick>
		class Tick final : public ITick {
		public:
			inline explicit Tick(TTick &target)
				: _target{ target } {

			};
			Tick() = delete;
			Tick(Tick const &) = delete;
			Tick(Tick &&) = delete;

			inline ~Tick() = default;

			Tick & operator=(Tick const &) = delete;
			Tick & operator=(Tick &&) = delete;

			virtual void operator()() override {
				_target();
			};

		private:
			TTick &_target;
		};

		HWND _hWnd;
		::std::unique_ptr<ITick> _target;
		void const *_targetId;
	};
}

#endif
