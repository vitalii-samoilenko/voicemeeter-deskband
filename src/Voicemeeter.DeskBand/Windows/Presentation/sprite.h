#pragma once

#include <array>
#include <vector>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Sprite {
				public:
					using BYTE = unsigned char;

					struct Region {
						size_t Width;
						size_t Height;
						BYTE* pData;
						size_t RowPitch;
						size_t SlicePitch;
					};
					enum Sprite_element : size_t {
						out_a_act = 0,
						out_b_act = 1,
						out_a_inact = 2,
						out_b_inact = 3
					};
					static constexpr size_t MIPMAP_COUNT{ 2 };

					using buffer_type = std::vector<BYTE>;
					using manifest_type = std::array<std::vector<Region>, MIPMAP_COUNT>;

					Region get_Region(Sprite_element element, size_t mipmap) const;

					Sprite();
					Sprite(const Sprite&) = delete;
					Sprite(Sprite&&) = delete;

					~Sprite() = default;

					Sprite& operator=(const Sprite&) = delete;
					Sprite& operator=(Sprite&&) = delete;

				private:
					buffer_type m_buffer;
					manifest_type m_manifest;
				};
			}
		}
	}
}