#pragma once

#include <vector>

namespace Voicemeeter {
	namespace Atlas {
		namespace Generator {
			namespace Field {
				class Encoder {
				public:
					Encoder(
						uint8_t* pMask,
						size_t height, size_t width,
						size_t step
					);
					Encoder() = delete;
					Encoder(const Encoder&) = delete;
					Encoder(Encoder&&) = delete;

					~Encoder() = default;

					Encoder& operator=(const Encoder&) = delete;
					Encoder& operator=(Encoder&&) = delete;

					inline const float* get_pDistance() const {
						return m_cDistance.data();
					};
					inline size_t get_size() const {
						return m_cDistance.size();
					};

				private:
					size_t m_height;
					size_t m_width;
					::std::vector<float> m_cDistance;
				};
			}
		}
	}
}