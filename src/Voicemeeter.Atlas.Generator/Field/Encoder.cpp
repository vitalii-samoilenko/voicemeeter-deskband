#include <algorithm>
#include <cmath>
#include <limits>

#include "Encoder.h"

using namespace ::Voicemeeter::Atlas::Generator::Field;

Encoder::Encoder(
	uint8_t* pMask,
	size_t height, size_t width,
	size_t step
) : m_height{ height / step + 1 }
  , m_width{ width / step + 1 }
  , m_cDistance(m_height * m_width, ::std::numeric_limits<float>::max()) {
	size_t remainder{ width % 4 };
	size_t rowPitch{
		(remainder
			? width - remainder + 4ULL
			: width)
	};

	for (size_t i{ 0 }; i < height; ++i) {
		for (size_t j{ 0 }; j < width; ++j) {
			if (!pMask[i * rowPitch + j]) {
				continue;
			}
			if (pMask[(i - 1) * rowPitch + j]
				&& pMask[(i - 1) * rowPitch + j + 1]
				&& pMask[i * rowPitch + j + 1]
				&& pMask[(i + 1) * rowPitch + j + 1]
				&& pMask[(i + 1) * rowPitch + j]
				&& pMask[(i + 1) * rowPitch + j - 1]
				&& pMask[i * rowPitch + j - 1]
				&& pMask[(i - 1) * rowPitch + j - 1]) {
				continue;
			}
			for (size_t k{ 0 }; k < m_height; ++k) {
				for (size_t l{ 0 }; l < m_width; ++l) {
					m_cDistance[k * m_width + l] = ::std::min(
						m_cDistance[k * m_width + l],
						static_cast<float>(
							(k * step - i) * (k * step - i)
							+ (l * step - j) * (l * step - j))
					);
				}
			}
		}
	}
	float distance{ ::std::numeric_limits<float>::min() };
	for (size_t i{ 0 }; i < m_height - 1; ++i) {
		for (size_t j{ 0 }; j < m_width - 1; ++j) {
			m_cDistance[i * m_width + j] = ::std::sqrt(m_cDistance[i * m_width + j]);
			distance = ::std::max(distance, m_cDistance[i * m_width + j]);
			if (!pMask[(i * step - step / 2) * rowPitch + j * step - step / 2]) {
				m_cDistance[i * m_width + j] = -m_cDistance[i * m_width + j];
			}
		}
	}
	for (size_t i{ 0 }; i < m_height; ++i) {
		for (size_t j{ 0 }; j < m_width; ++j) {
			m_cDistance[i * m_width + j] = (m_cDistance[i * m_width + j] + distance) / (2.F * distance);
		}
	}
}