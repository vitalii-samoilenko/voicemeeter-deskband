#ifndef WINDOWS_TIMER_HPP
#define WINDOWS_TIMER_HPP

#include <chrono>
#include <memory>
#include <utility>

#include "Windows/API.h"

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
		inline void Set(::std::chrono::milliseconds duration, TTick &target) {
			SetTimer(_hWnd, get_Id(), static_cast<UINT>(duration.count()), NULL);
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
			KillTimer(_hWnd, get_Id());
			_target = nullptr;
			targetId = nullptr;
		};

		inline void Elapse() {
			_target->operator()();
		};

	private:
		class ITick {
		public:
			ITick(ITick const &) = delete;
			ITick(ITick &&) = delete;

			virtual ~ITick() = 0;

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
				: _target{ &target }
				, _type{ Ownership::None } {

			};
			inline explicit Tick(::std::unique_ptr<TTick> &&target)
				: _target{ target.get() }
				, _type{ Ownership::Exclusive }
				, _eTarget{ ::std::move(target) } {

			};
			inline explicit Tick(::std::shared_ptr<TBundle> const &target)
				: _target{ target.get() }
				, _type{ Ownership::Shared }
				, _sTarget{ target } {

			};
			Tick() = delete;
			Tick(Tick const &) = delete;
			Tick(Tick &&) = delete;

			inline ~Tick() {
				switch(_type) {
				case Ownership::Exclusive:
					_eTarget = nullptr;
					break;
				case Ownership::Shared:
					_sTarget = nullptr;
					break;
				};
			};

			Tick & operator=(Tick const &) = delete;
			Tick & operator=(Tick &&) = delete;

			virtual void operator()() override {
				_target->operator()();
			};

		private:
			enum class Ownership {
				None = 0,
				Exclusive = 1,
				Shared = 2
			};

			TTick *_target;
			Ownership _type;
			union {
				::std::unique_ptr<TBundle> _eTarget;
				::std::shared_ptr<TBundle> _sTarget;
			};
		};

		HWND _hWnd;
		::std::function<bool()> _callback;
	};

	::std::unique_ptr<ITick> _target;
	void *_targetId;
}

#endif
