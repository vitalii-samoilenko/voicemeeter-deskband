#pragma once

#include <array>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Atlas {
				public:
					struct Image {
						size_t Width;
						size_t Height;
						const void* pData;
					};
					enum Image_Id : size_t {
						out_a_act = 0,
						out_b_act = 1,
						out_a_inact = 2,
						out_b_inact = 3
					};
					static constexpr size_t MIPMAP_COUNT{ 2 };

					inline Image get_Image(Image_Id id, size_t mipmap) const {
						return m_ppImage[mipmap][id];
					}

					Atlas();
					Atlas(const Atlas&) = delete;
					Atlas(Atlas&&) = delete;

					~Atlas() = default;

					Atlas& operator=(const Atlas&) = delete;
					Atlas& operator=(Atlas&&) = delete;

				private:
					std::array<std::array<Image, out_b_inact + 1>, MIPMAP_COUNT> m_ppImage;
				};
			}
		}
	}
}