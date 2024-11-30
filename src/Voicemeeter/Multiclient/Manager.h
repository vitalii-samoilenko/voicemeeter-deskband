#pragma once

#include <functional>
#include <typeinfo>
#include <unordered_map>

#include "Subscription.h"

namespace Voicemeeter {
	namespace Multiclient {
		template<typename Specification>
		class Manager {
		public:
			inline Manager() = default;
			Manager(const Manager&) = delete;
			Manager(Manager&&) = delete;

			inline ~Manager() = default;

			Manager& operator=(const Manager&) = delete;
			Manager& operator=(Manager&&) = delete;

			template<typename TClient>
			inline Subscription<Specification>& get_Subscription() const {
				return m_cSubscription[reinterpret_cast<void*>(&
					const_cast<type_info&>(typeid(TClient)))];
			};

			inline auto begin() const {
				return m_cSubscription.begin();
			};
			inline auto end() const {
				return m_cSubscription.end();
			};

		private:
			mutable ::std::unordered_map<void*, Subscription<Specification>> m_cSubscription;
		};
	}
}