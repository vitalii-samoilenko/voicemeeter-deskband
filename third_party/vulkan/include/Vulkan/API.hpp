#ifndef VULKAN_API_HPP
#define VULKAN_API_HPP

#include "Vulkan/Error.hpp"

namespace Vulkan {
	inline VkResult ThrowIfFailed(
		VkResult code,
		char const *what) {
		if (code != VK_SUCCESS) {
			throw Error{ code, what };
		}
		return code;
	};
}

#endif
