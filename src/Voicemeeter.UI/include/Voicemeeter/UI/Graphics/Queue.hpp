#ifndef VOICEMEETER_UI_GRAPHICS_QUEUE_HPP
#define VOICEMEETER_UI_GRAPHICS_QUEUE_HPP

#include <memory>
#include <vector>

#include "Voicemeeter/UI/Adapters/IBundle.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			class Queue final {
			public:
				inline Queue()
					: _items{} {

				};
				Queue(Queue const &) = delete;
				inline Queue(Queue &&) = default;

				inline ~Queue() = default;

				Queue & operator=(Queue const &) = delete;
				Queue & operator=(Queue &&) = delete;

				class slot final {
				public:
					slot() = delete;
					slot(slot const &) = delete;
					inline slot(slot &&other)
						: _items{ other._items }
						, _i{ other._i }
						, _itemId{ other._itemId } {
						other._i = 0;
						other._itemId = nullptr;
					};

					inline ~slot() = default;

					slot & operator=(slot const &) = delete;
					slot & operator=(slot &&) = delete;

					template<typename TBundle>
					inline void overwrite(TBundle &target) {
						if (_itemId == &typeid(TBundle)) {
							return;
						} else if (_itemId == nullptr) {
							_i = _items.size();
							_items.emplace_back(nullptr);
						}
						_items[_i] = ::std::make_unique<
							Adapters::Bundle<TBundle>>(
							target);
						_itemId = &typeid(TBundle);
					};

				private:
					friend class Queue;

					::std::vector<::std::unique_ptr<Adapters::IBundle>> &_items;
					size_t _i;
					void const *_itemId;

					inline explicit slot(Queue &target)
						: _items{ target._items }
						, _i{ 0 }
						, _itemId{ nullptr } {

					};
				};

				inline bool empty() const {
					return _items.empty();
				};

				inline auto begin() {
					return _items.begin();
				};
				inline auto end() {
					return _items.end();
				};

				inline slot reserve() {
					return slot{ *this };
				};

			private:
				friend class slot;

				::std::vector<::std::unique_ptr<Adapters::IBundle>> _items;
			};
		}
	}
}

#endif
