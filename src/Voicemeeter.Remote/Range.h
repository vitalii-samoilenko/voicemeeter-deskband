#pragma once

#include <iterator>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Voicemeeter/IRange.h"

namespace Voicemeeter {
	namespace Remote {
		template<typename T>
		class RangeIterator {
		public:
			explicit RangeIterator(
				typename ::std::vector<::std::shared_ptr<T>>::const_iterator current
			) : m_current{ current } {

			};
			RangeIterator() = delete;
			RangeIterator(const RangeIterator&) = default;
			RangeIterator(RangeIterator&&) = default;

			~RangeIterator() = default;

			RangeIterator& operator=(const RangeIterator&) = default;
			RangeIterator& operator=(RangeIterator&&) = default;

			RangeIterator& operator++() {
				++m_current;

				return *this;
			};
			T& operator*() {
				return **m_current;
			}
			bool operator==(const RangeIterator& rhs) {
				return m_current == rhs.m_current;
			};
			bool operator!=(const RangeIterator& rhs) {
				return !(*this == rhs);
			}

		private:
			typename ::std::vector<::std::shared_ptr<T>>::const_iterator m_current;
		};

		template<typename T>
		class Range : public IRange<T, RangeIterator<T>> {
		public:
			using iterator_type = RangeIterator<T>;

			Range(
			) : m_cpElement{}
			  , m_lElement{} {

			};
			Range(const Range&) = delete;
			Range(Range&&) = delete;

			~Range() = default;

			Range& operator=(const Range&) = delete;
			Range& operator=(Range&&) = delete;

			template<typename... Args>
			void emplace(Args&& ...args) {
				m_cpElement.emplace_back(
					::std::move(::std::shared_ptr<T>{
						new T{ ::std::forward<Args>(args)... }
					})
				);
				m_lElement.emplace(
					reinterpret_cast<void*>(m_cpElement.back()
						.get()),
					m_cpElement.size() - 1
				);
			};
			void copy(const T& element, const Range<T>& range) {
				void* pElement{
					reinterpret_cast<void*>(
						const_cast<T*>(&element))
				};
				m_cpElement.push_back(
					range.m_cpElement.at(
						range.m_lElement
							.find(pElement)
								->second
					)
				);
				m_lElement.emplace(
					pElement,
					m_cpElement.size() - 1
				);
			}
			void erase(const T& element) {
				typename ::std::unordered_map<void*, size_t>::iterator elementLookup{
					m_lElement.find(reinterpret_cast<void*>(
						const_cast<T*>(&element)))
				};
				::std::swap(m_cpElement.back(), m_cpElement.at(elementLookup->second));
				m_lElement[reinterpret_cast<void*>(m_cpElement[elementLookup->second]
					.get())] = elementLookup->second;
				m_cpElement.pop_back();
				m_lElement.erase(elementLookup);
			}
			RangeIterator<T> find(const T& element) {
				typename ::std::unordered_map<void*, size_t>::iterator lookup{
					m_lElement.find(reinterpret_cast<void*>(
						const_cast<T*>(&element)))
				};
				return RangeIterator<T>{
					(lookup == m_lElement.end()
						? m_cpElement.end()
						: m_cpElement.begin() + lookup->second)
				};
			}

			virtual RangeIterator<T> begin() const {
				return RangeIterator<T>{ m_cpElement.begin() };
			};
			virtual RangeIterator<T> end() const {
				return RangeIterator<T>{ m_cpElement.end() };
			};

		private:
			::std::vector<::std::shared_ptr<T>> m_cpElement;
			::std::unordered_map<void*, size_t> m_lElement;
		};
	}
}