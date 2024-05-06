#pragma once

#include "panel.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			enum class orientation { right };

			template<orientation O>
			class StackPanel : public Panel {
				static std::once_flag PnlStackClassGuard;

			protected:
				virtual LRESULT OnSize() override;

			public:
				explicit StackPanel(
					Window& rWndParent
				);
				StackPanel() = delete;
				StackPanel(const StackPanel&) = delete;
				StackPanel(StackPanel&&) = delete;

				~StackPanel() = default;

				StackPanel& operator=(const StackPanel&) = delete;
				StackPanel& operator=(StackPanel&&) = delete;
			};
		}
	}
}