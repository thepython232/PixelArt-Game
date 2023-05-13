#pragma once

#include "Platform\Device.h"

namespace gfx {

	class PipelineCache {
	public:
		PipelineCache(const std::string& filepath);
		~PipelineCache();

		VkPipelineCache GetCache() const { return cache; }
		void Flush() const; //Flush out to the file

	private:
		std::string filename;
		VkPipelineCache cache;
	};

	class Shader {
	public:
		Shader(const std::string& filename, VkShaderStageFlagBits stage);
		~Shader();

		VkPipelineShaderStageCreateInfo Create() const;

	private:
		VkShaderModule shader;
		VkShaderStageFlagBits stage;
	};

	class Pipeline {
	public:
		struct LayoutSettings {
			std::vector<VkDescriptorSetLayout> sets;
			std::vector<VkPushConstantRange> ranges;
		};

		Pipeline(const LayoutSettings& layoutSettings, PipelineCache* cache = nullptr);
		~Pipeline();

		void Bind(VkCommandBuffer commandBuffer);
		VkPipelineLayout GetLayout() const { return layout; }
		VkPipeline GetPipeline() const { return pipeline; }

		virtual VkPipelineBindPoint BindPoint() const = 0;

	protected:
		VkPipeline pipeline;

		VkPipelineLayout layout;
		PipelineCache* cache;
	};
}