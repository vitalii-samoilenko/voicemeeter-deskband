#pragma once

#include <array>
#include <vector>

namespace Voicemeeter {
	namespace DeskBand {
		namespace Windows {
			namespace Presentation {
				class Sprite {
				public:
					struct Region {
						size_t Offset;
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

					Region get_Region(Sprite_element element, size_t mipmap) const;

					Sprite() = default;
					Sprite(const Sprite&) = delete;
					Sprite(Sprite&&) = delete;

					~Sprite() = default;

					Sprite& operator=(const Sprite&) = delete;
					Sprite& operator=(Sprite&&) = delete;

					std::vector<unsigned char> LoadSprite();

				private:
					std::array<std::vector<Region>, MIPMAP_COUNT> m_manifest;
				};
			}
		}
	}
}