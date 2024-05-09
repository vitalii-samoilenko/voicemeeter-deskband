#pragma once

#include <utility>
#include <vector>

#include "control.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class Panel : public Window {
			protected:
				using ratio_type = std::pair<unsigned int, unsigned int>;
				using pack_type = std::pair<ratio_type, std::unique_ptr<Control>>;

				enum ratio_member : size_t { width = 0, height = 1 };
				enum pack_member : size_t { ratio = 0, pControl = 1 };

				Panel(
					std::once_flag& rWndClassGuard,
					LPCWSTR lpszClassName,
					Window& rWndParent
				);

			private:
				Window& m_rWndParent;
				std::vector<pack_type> m_cPack;

			public:
				inline const std::vector<pack_type>& get_rcPack() const noexcept {
					return m_cPack;
				};

				Panel() = delete;
				Panel(const Panel&) = delete;
				Panel(Panel&&) = delete;

				~Panel() = default;

				Panel& operator=(const Panel&) = delete;
				Panel& operator=(Panel&&) = delete;

				virtual void Initialize() override;
				virtual void Show(int nCmdShow) override;

				template<typename T, typename ...Args>
				T* MakeControl(ratio_type r, Args... args) {
					m_cPack.emplace_back(r, std::make_unique(new T{ *this, std::forward<Args>(args)... }));

					return reinterpret_cast<T*>(std::get<pControl>(m_cPack.back()).get());
				}
			};
		}
	}
}