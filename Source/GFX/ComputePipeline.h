#pragma once

#include "Pipeline.h"

namespace gfx {

	class ComputePipeline : public Pipeline {
	public:
		ComputePipeline(const LayoutSettings& layoutSettings,
			const Shader& shader, PipelineCache* cache);

		static VkPipelineBindPoint BindPoint() { return VK_PIPELINE_BIND_POINT_COMPUTE; }
	};
}