#pragma once

#include "statecontrol.h"

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			class CircularStateControl : public StateControl {
				LONG m_mod;

			protected:
				virtual LRESULT OnLButtonDown(LONG x, LONG y) override;

			public:
				CircularStateControl(
					Panel& rPnlParent,
					LONG mod
				);
				CircularStateControl() = delete;
				CircularStateControl(const CircularStateControl&) = delete;
				CircularStateControl(CircularStateControl&&) = delete;

				~CircularStateControl() = default;

				CircularStateControl& operator=(const CircularStateControl&) = delete;
				CircularStateControl& operator=(CircularStateControl&&) = delete;
			};
		}
	}
}