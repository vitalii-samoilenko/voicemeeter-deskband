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
					inline slot() = default;
					slot(slot const &) = delete;
					inline slot(slot &&) = default;

					inline ~slot() = default;

					slot & operator=(slot const &) = delete;
					slot & operator=(slot &&) = delete;

					template<typename TBundle>
					inline void overwrite(TBundle &target) const {
						if (_itemId != &target
							|| _itemTypeId == &typeid(TBundle)) {
							return;
						}
						_queue->_items[_pos] = ::std::make_unique<
							Adapters::Bundle<TBundle>>(
							target);
						_itemId = &target;
						_itemTypeId = const_cast<void *>(
							&typeid(TBundle));
					};

				private:
					Queue *_queue;
					size_t _pos;
					void *_itemId;
					void *_itemTypeId;

					friend Queue;

					inline slot(
						Queue &queue,
						size_t pos)
						: _target{ &target }
						, _pos{ pos }
						, _itemId{ nullptr }
						, _itemTypeId{ nullptr } {

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

				template<typename TBundle>
				inline slot push(TBundle &item) {
					slot target{ *this, _items.size() };
					_items.emplace_back(nullptr);
					target.overwrite(item);
					return target;
				};

			private:
				::std::vector<::std::unique_ptr<Adapters::IBundle>> _items;

				friend slot;
			};
		}
	}
}

#endif
