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
			Range(
			) : m_cpElement{}
			  , m_cLookup{} {

			};
			Range(const Range&) = delete;
			Range(Range&&) = delete;

			~Range() = default;

			Range& operator=(const Range&) = delete;
			Range& operator=(Range&&) = delete;

			template<typename... Args>
			void emplace(Args&& ...args) {
				::std::shared_ptr<T> pElement{
					new T{ ::std::forward<Args>(args)... }
				};
				m_cLookup.insert({
					reinterpret_cast<unsigned long long>(pElement.get()),
					m_cpElement.size()
				});
				m_cpElement.push_back(
					::std::move(pElement)
				);
			};
			void copy(const T& element, const Range<T>& range) {
				unsigned long long addr{
					reinterpret_cast<unsigned long long>(&element)
				};
				m_cLookup.insert({
					addr,
					m_cpElement.size()
				});
				m_cpElement.push_back(
					range.m_cpElement.at(
						range.m_cLookup
							.find(addr)
								->second
					)
				);
			}
			void erase(const T& element) {
				typename ::std::unordered_map<unsigned long long, size_t>::iterator lastLookup{
					m_cLookup.find(reinterpret_cast<unsigned long long>(&*m_cpElement.back()))
				};
				typename ::std::unordered_map<unsigned long long, size_t>::iterator elementLookup{
					m_cLookup.find(reinterpret_cast<unsigned long long>(&element))
				};
				::std::swap(m_cpElement.back(), m_cpElement.at(elementLookup->second));
				lastLookup->second = elementLookup->second;
				m_cpElement.pop_back();
				m_cLookup.erase(elementLookup);
			}
			RangeIterator<T> find(const T& element) {
				typename ::std::unordered_map<unsigned long long, size_t>::iterator lookup{
					m_cLookup.find(reinterpret_cast<unsigned long long>(&element))
				};
				return RangeIterator<T>{
					(lookup == m_cLookup.end()
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
			::std::unordered_map<unsigned long long, size_t> m_cLookup;
		};
	}
}