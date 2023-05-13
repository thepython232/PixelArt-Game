#include "Pipeline.h"
#include "Platform\Platform.h"
#include "State.h"
#include "Util\File.h"

namespace gfx {

	PipelineCache::PipelineCache(const std::string& filepath) : filename(filepath) {
		VkPipelineCacheCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		auto data = util::ReadFileBinary(filepath).UnwrapOr({});

		if (util::FileExists(filepath)) {
			createInfo.initialDataSize = data.size();
			createInfo.pInitialData = static_cast<const void*>(data.data());
		}

		VULKAN_CHECK(vkCreatePipelineCache(*global.platform->device, &createInfo, nullptr, &cache), "Failed to create pipeline cache!");
	}

	PipelineCache::~PipelineCache() {
		Flush();

		vkDestroyPipelineCache(*global.platform->device, cache, nullptr);
	}

	void PipelineCache::Flush() const {
		size_t size;
		vkGetPipelineCacheData(*global.platform->device, cache, &size, nullptr);
		std::vector<u8> data(size);
		vkGetPipelineCacheData(*global.platform->device, cache, &size, static_cast<void*>(data.data()));

		if (!util::FileExists(filename)) {
			util::CreateFile(filename);
		}

		util::WriteFileBinary(filename, data);
	}

	Shader::Shader(const std::string& filename, VkShaderStageFlagBits stage)
		: stage(stage) {
		if (!util::FileExists(filename)) {
			ERROR(-1, util::Logger::GFX, "Failed to open shader $!", filename);
		}

		std::vector<u8> code = util::ReadFileBinary(filename).Unwrap();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VULKAN_CHECK(vkCreateShaderModule(*global.platform->device, &createInfo, nullptr, &shader),
			"Failed to create shader module!");
	}

	Shader::~Shader() {
		vkDestroyShaderModule(*global.platform->device, shader, nullptr);
	}

	VkPipelineShaderStageCreateInfo Shader::Create() const {
		VkPipelineShaderStageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.stage = stage;
		createInfo.pName = "main";
		createInfo.module = shader;
		
		return createInfo;
	}

	Pipeline::Pipeline(const LayoutSettings& layoutSettings, PipelineCache* cache) : cache(cache) {
		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.setLayoutCount = static_cast<uint32_t>(layoutSettings.sets.size());
		createInfo.pSetLayouts = layoutSettings.sets.data();
		createInfo.pushConstantRangeCount = static_cast<uint32_t>(layoutSettings.ranges.size());
		createInfo.pPushConstantRanges = layoutSettings.ranges.data();

		VULKAN_CHECK(vkCreatePipelineLayout(*global.platform->device, &createInfo, nullptr, &layout),
			"Failed to create pipeline layout!");
	}

	Pipeline::~Pipeline() {
		vkDestroyPipeline(*global.platform->device, pipeline, nullptr);
		vkDestroyPipelineLayout(*global.platform->device, layout, nullptr);
	}

	void Pipeline::Bind(VkCommandBuffer commandBuffer) {
		vkCmdBindPipeline(commandBuffer, BindPoint(), pipeline);
	}
}