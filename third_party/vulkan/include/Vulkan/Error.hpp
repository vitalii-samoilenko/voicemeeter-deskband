#ifndef VULKAN_ERROR_HPP
#define VULKAN_ERROR_HPP

#include <stdexcept>

#include "vulkan/vulkan.h"

namespace Vulkan {
	class Error : public ::std::runtime_error {
	public:
		inline Error(VkResult code_arg, char const *what_arg) noexcept
			: runtime_error{ what_arg }
			, _code{ code_arg } {

		};

		inline VkResult code() const noexcept {
			return _code;
		};

	private:
		VkResult _code;
	};
}

#endif
