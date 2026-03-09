#ifndef WUI_GRAPHICS_SURFACE_HPP
#define WUI_GRAPHICS_SURFACE_HPP

#include <array>
#include <vector>

#include "math.hpp"
#include "memory.hpp"

#include "Vulkan.hpp"

#include "windows.h"

namespace WUI {
	namespace Graphics {
		class Sufrace final {
		public:
			inline explicit Surface(HWND hWnd)
				: _hWnd{ hWnd } {
				bool failed{ true };
				auto guard = ::wstd::make_guard([
					&failed,
					this
				]()->void {
					if (!failed) {
						return;
					}
					::vkFreeCommandBuffers(_device, _slots_commandPool,
						static_cast<uint32_t>(_slots_commandBuffers.size()), &_slots_commandBuffers[0]);
					::vkDestroyCommandPool(_device, _commandPool, NULL);
					::vkDestroyDevice(_device, NULL);
					::vkDestroyInstance(_instance, NULL);
				});
				{
					void const *next{ NULL };
					::std::vector<char const *> enabledLayerNames{};
					::std::vector<char const *> enabledExtensionNames{
						"VK_KHR_get_physical_device_properties2"
					};
#ifndef NDEBUG
					enabledLayerNames.push_back("VK_LAYER_KHRONOS_validation");
					enabledExtensionNames.push_back("VK_EXT_layer_settings");
					VkBool32 enabled{ VK_TRUE };
					::std::vector<VkLayerSettingsEXT> settings{
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"legacy_detection", VK_LAYER_SETTING_TYPE_BOOL32_EXT,
							1U, &enabled
						},
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"validate_sync", VK_LAYER_SETTING_TYPE_BOOL32_EXT,
							1U, &enabled
						},
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"printf_enable", VK_LAYER_SETTING_TYPE_BOOL32_EXT,
							1U, &enabled
						},
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"gpuav_enable", VK_LAYER_SETTING_TYPE_BOOL32_EXT,
							1U, &enabled
						},
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"validate_best_practices", VK_LAYER_SETTING_TYPE_BOOL32_EXT,
							1U, &enabled
						},
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"validate_best_practices_nvidia", VK_LAYER_SETTING_TYPE_BOOL32_EXT,
							1U, &enabled
						},
						VkLayerSettingsEXT{
							"VK_LAYER_KHRONOS_validation",
							"report_flags", VK_LAYER_SETTING_TYPE_STRING_EXT,
							1U, "warn,perf,error"
						},
					};
					VkLayerSettingsCreateInfoEXT layerSettingsCreateInfo{
						VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT, next,
						static_cast<uint32_t>(settings.size()), &settings[0]
					};
					next = &layerSettingsCreateInfo;
#endif
					VkApplicationInfo applicationInfo{
						VK_STRUCTURE_TYPE_APPLICATION_INFO, NULL,
						"WUI", VK_MAKE_VERSION(0, 9, 0),
						NULL, 0U,
						VK_API_VERSION_1_0
					};
					VkInstaceCreateInfo createInfo{
						VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, next,
						0U,
						&applicationInfo,
						static_cast<uint32_t>(enabledLayerNames.size()), &enabledLayerNames[0],
						static_cast<uint32_t>(enabledExtensionNames.size()), &enabledExtensionNames[0]
					};
					::Vulkan::ThrowIfFailed(::vkCreateInstance(
						&createInfo, NULL,
						&_instance
					), "Instance creation failed");
				}
				{
					uint32_t physicalDeviceCount;
					::Vulkan::ThrowIfFailed(::vkEnumeratePhysicalDevices(
						_instante,
						&physicalDeviceCount, NULL
					), "Failed to count devices");
					::std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
					::Vulkan::ThrowIfFailed(::vkEnumeratePhysicalDevices(
						_instance,
						&physicalDeviceCount, &physicalDevices[0]
					), "Failed to enumerate devices");
					_physicalDevice = physicalDevices[0];
					for (size_t i{ 0 }, p{ 0 }; i < physicalDevices.size(); ++i) {
						VkPhysicalDevice const &physicalDevice{ physicalDevices[i] };
						VkPhysicalDeviceProperties2KHR properties;
						::Vulkan::ThrowIfFailed(::vkGetPhysicalDeviceProperties2KHR(
							physicalDevice,
							&properties
						), "Failed to get device properties");
						VkPhysicalDeviceProperties const &coreProperties{ properties.properties };
						size_t t{ 0 };
						switch (coreProperties.deviceType) {
						case VK_PHYSICAL_DEVICE_TYPE_CPU:
							t = 1;
							break;
						case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
							t = 2;
							break;
						case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
							t = 3;
							break;
						};
						if (p < t) {
							_physicalDevice = physicalDevice;
							p = t;
						}
					}
				}
				{
					uint32_t queueFamilyPropertyCount;
					::Vulkan::ThrowIfFailed(::vkGetPhysicalDeviceQueueFamilyProperties(
						_physicalDevice,
						&queueFamilyPropertyCount, NULL
					), "Failed to count device queue families");
					::std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
					::Vulkan::ThrowIfFailed(::vkGetPhysicalDeviceQueueFamilyProperties(
						_physicalDevice,
						&queueFamilyPropertyCount, queueFamilyProperties.data()
					), "Failed to get device queue families");
					for (size_t i{ 0 }; i < queueFamilyProperties.size(); ++i) {
						VkQueueFamilyProperties const &queueFamilyProperty{ queueFamilyProperties[i] };
						if (!(queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
							continue;
						}
						_queueFamilyIndex = static_cast<uint32_t>(i);
						break;
					}
				}
				{
					// TODO: Global high priority? 1.4
					::std::vector<float> queuePriorities{ 1.F };
					::std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{
						VkDeviceQueueCreateInfo{
							VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, NULL,
							0U,
							_queueFamilyIndex,
							static_cast<uint32_t>(queuePriorities.size()), &queuePriorities[0]
						}
					};
					::std::vector<char const *> enabledExtensionNames{
						"VK_KHR_synchronization2"
					};
					VkDeviceCreateInfo createInfo{
						VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, NULL,
						0U,
						static_cast<uint32_t>(queueCreateInfos.size()), &queueCreateInfos[0],
						0U, NULL,
						static_cast<uint32_t>(enabledExtensionNames.size()), &enabledExtensionNames[0],
						NULL
					};
					::Vulkan::ThrowIfFailed(::vkCreateDevice(
						_physicalDevice,
						&createInfo, NULL,
						&_device
					), "Device creation failed");
					::Vulkan::ThrowIfFailed(::vkGetDeviceQueue(
						_device,
						_queueFamilyIndex, 0U,
						&_queue
					), "Failed to get device queue");
				}
				{
					VkCommandPoolCreateInfo createInfo{
						VK_STRUCTURE_COMMAND_POOL_CREATE_INFO, NULL,
						VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
						_queueFamilyIndex
					};
					::Vulkan::ThrowIfFailed(::vkCreateCommandPool(
						_device,
						&createInfo, NULL,
						&_slots_commandPool
					), "Command pool creation failed");
					VkCommandBufferAllocateInfo allocateInfo{
						VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, NULL,
						_slots_commandPool,
						VK_COMMAND_BUFFER_LEVEL_PRIMARY,
						static_cast<uint32_t>(SlotsSize)
					};
					::Vulkan::ThrowIfFailed(::vkAllocateCommandBuffers(
						_device,
						allocateInfo,
						&_slots_commandBuffers[0]
					), "Failed to allocate command buffers");
				}
				failed = false;
			};
			Surface() = delete;
			Surface(Surface const &) = delete;
			Surface(Surface &&) = delete;

			inline ~Surface() {
				::vkFreeCommandBuffers(_device, _slots_commandPool,
					static_cast<uint32_t>(_slots_commandBuffers.size()), &_slots_commandBuffers[0]);
				::vkDestroyCommandPool(_device, _slots_commandPool, NULL);
				::vkDestroyDevice(_device, NULL);
				::vkDestroyInstance(_instance, NULL);
			};

			Surface & operator=(Surface const &) = delete;
			Surface & operator=(Surface &&) = delete;

			inline void Prepare(vec_t const &point, vec_t const &vertex) {
				size_t slot{ inc_slots_Current() };
				VkCommandBufferBeginInfo beginInfo{
					VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, NULL,
					VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
					NULL
				};
				::Vulkan::ThrowIfFailed(::vkBeginCommandBuffer(
					get_slots_commandBuffer(slot),
					&beginInfo
				), "Command buffer reset failed");
				::Vulkan::ThrowIfFailed(::vkEndCommandBuffer(
					get_slots_commandBuffer(slot)
				), "Command buffer close failed");
				::std::vector<VkCommandBufferSubmitInfo> commandBufferInfos{
					VkCommandBufferSubmitInfo{
						VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO, NULL,
						get_slots_commandBuffer(slot),
						0U
					}
				};
				::std::vector<VkSubmitInfo2> submits{
					VkSubmitInfo2{
						VK_STRUCTURE_TYPE_SUBMIT_INFO_2, NULL,
						0U,
						0U, NULL,
						static_cast<uint32_t>(commandBufferInfos.size()), &commandBufferInfos[0],
						0U, NULL
					}
				};
				::Vulkan::ThrowIfFailed(::vkQueueSubmit2KHR(
					_queue,
					static_cast<uint32_t>(submits.size()), &submits[0],
					NULL
				), "Queue submit failed");
			};
			inline void Commit() {
			};

			static constexpr size_t BuffersSize{ 2 };
			static constexpr size_t SlotsSize{ 3 };

			// slots
			inline size_t get_slots_Current() const {
				return _slots_current;
			};
			inline size_t inc_slots_Current() {
				return (_slots_current = (_slots_current + 1) % SlotsSize);
			};
			inline VkCommandBuffer get_slots_CommandBuffer(size_t slot) const {
				return _slots_commandBuffers[slot];
			}
			// -----

		private:
			HWND _hWnd;
			VkInstance _instance;
			VkPhysicalDevice _physicalDevice;
			uint32_t _queueFamilyIndex;
			VkDevice _device;
			VkQueue _queue;
			// slots
			size_t _slots_current;
			VkCommandPool _slots_commandPool;
			::std::array<VkCommandBuffer, SlotsSize> _slots_commandBuffers;
			// -----
		};
	}
}

#endif
