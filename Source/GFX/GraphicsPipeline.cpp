#include "GraphicsPipeline.h"
#include "Platform\Platform.h"
#include "State.h"

namespace gfx {

	GraphicsPipeline::GraphicsPipeline(
		const LayoutSettings& layoutSettings,
		const Settings& settings,
		PipelineCache* cache)
		: Pipeline(layoutSettings, cache) {
		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.layout = layout;

		createInfo.renderPass = settings.renderPass;
		createInfo.subpass = settings.subpass;

		std::vector<VkPipelineShaderStageCreateInfo> shaders;
		for (const auto& shader : settings.shaders) {
			shaders.push_back(shader->Create());
		}

		createInfo.stageCount = static_cast<uint32_t>(shaders.size());
		createInfo.pStages = shaders.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = settings.inputAssembly.topology;
		inputAssembly.primitiveRestartEnable =
			settings.inputAssembly.restartEnable;
		createInfo.pInputAssemblyState = &inputAssembly;

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexBindingDescriptionCount =
			static_cast<uint32_t>(settings.vertexInput.bindings.size());
		vertexInput.pVertexBindingDescriptions =
			settings.vertexInput.bindings.data();
		vertexInput.vertexAttributeDescriptionCount =
			static_cast<uint32_t>(settings.vertexInput.attributes.size());
		vertexInput.pVertexAttributeDescriptions =
			settings.vertexInput.attributes.data();
		createInfo.pVertexInputState = &vertexInput;

		VkPipelineViewportStateCreateInfo viewport{};
		viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		if (std::find(
			settings.dynamicStates.begin(),
			settings.dynamicStates.end(),
			VK_DYNAMIC_STATE_VIEWPORT)
			!= settings.dynamicStates.end()) {
			viewport.viewportCount = 1;
		}
		else {
			viewport.viewportCount =
				static_cast<uint32_t>(settings.viewport.viewports.size());
			viewport.pViewports = settings.viewport.viewports.data();
		}
		if (std::find(
			settings.dynamicStates.begin(),
			settings.dynamicStates.end(),
			VK_DYNAMIC_STATE_SCISSOR)
			!= settings.dynamicStates.end()) {
			viewport.scissorCount = 1;
		}
		else {
			viewport.scissorCount =
				static_cast<uint32_t>(settings.viewport.scissors.size());
			viewport.pScissors = settings.viewport.scissors.data();
		}
		createInfo.pViewportState = &viewport;

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount =
			static_cast<uint32_t>(settings.dynamicStates.size());
		dynamicState.pDynamicStates = settings.dynamicStates.data();
		createInfo.pDynamicState = &dynamicState;

		VkPipelineRasterizationStateCreateInfo rasterization{};
		rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterization.cullMode = settings.rasterization.cullMode;
		rasterization.depthBiasEnable = settings.rasterization.depthBiasEnable;
		rasterization.depthBiasClamp = settings.rasterization.depthBiasClamp;
		rasterization.depthBiasConstantFactor =
			settings.rasterization.depthBiasConstantFactor;
		rasterization.depthBiasSlopeFactor =
			settings.rasterization.depthBiasSlopeFactor;
		rasterization.frontFace = settings.rasterization.frontFace;
		rasterization.polygonMode = settings.rasterization.polygonMode;
		rasterization.lineWidth = settings.rasterization.lineWidth;
		rasterization.rasterizerDiscardEnable = settings.rasterization.discard;
		createInfo.pRasterizationState = &rasterization;

		VkPipelineMultisampleStateCreateInfo multisample{};
		multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisample.rasterizationSamples = settings.multisampling.rasterSamples;
		if (settings.multisampling.sampleMask) {
			multisample.pSampleMask = &settings.multisampling.sampleMask.value();
		}
		multisample.sampleShadingEnable = settings.multisampling.sampleShading;
		multisample.minSampleShading = settings.multisampling.minSampleShading;
		multisample.alphaToOneEnable = settings.multisampling.alphaToOne;
		multisample.alphaToCoverageEnable =
			settings.multisampling.alphaToCoverage;
		createInfo.pMultisampleState = &multisample;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = settings.blending.enableLogicOp;
		colorBlending.logicOp = settings.blending.logicOp;
		colorBlending.attachmentCount =
			static_cast<uint32_t>(settings.blending.attachments.size());
		colorBlending.pAttachments = settings.blending.attachments.data();
		colorBlending.blendConstants[0] = settings.blending.constants[0];
		colorBlending.blendConstants[1] = settings.blending.constants[1];
		colorBlending.blendConstants[2] = settings.blending.constants[2];
		colorBlending.blendConstants[3] = settings.blending.constants[3];
		createInfo.pColorBlendState = &colorBlending;

		VkPipelineDepthStencilStateCreateInfo depth{};
		depth.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth.depthCompareOp = settings.depth.compareOp;
		depth.depthTestEnable = settings.depth.test;
		depth.depthWriteEnable = settings.depth.write;
		depth.depthBoundsTestEnable = settings.depth.bounds;
		depth.minDepthBounds = settings.depth.minBounds;
		depth.maxDepthBounds = settings.depth.maxBounds;
		depth.stencilTestEnable = settings.depth.testStencil;
		depth.front = settings.depth.stencilFront;
		depth.back = settings.depth.stencilBack;
		if (settings.depth.enabled) {
			createInfo.pDepthStencilState = &depth;
		}
		
		VkPipelineTessellationStateCreateInfo tesselation{};
		tesselation.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		tesselation.patchControlPoints = settings.tesselation.controlPoints;
		if (settings.tesselation.enabled) {
			createInfo.pTessellationState = &tesselation;
		}

		VULKAN_CHECK(vkCreateGraphicsPipelines(
			*global.platform->device,
			cache ? cache->GetCache() : VK_NULL_HANDLE,
			1, &createInfo,
			nullptr,
			&pipeline),
			"Failed to create graphics pipeline!");
	}

	GraphicsPipeline::Settings GraphicsPipeline::DefaultSettings(
		const Shader& vertShader,
		const Shader& fragShader,
		VkRenderPass renderPass,
		uint32_t attachmentCount
	) {
		Settings settings{};
		settings.renderPass = renderPass;
		settings.shaders.push_back(&vertShader);
		settings.shaders.push_back(&fragShader);

		for (uint32_t i = 0; i < attachmentCount; i++) {
			settings.blending.attachments.push_back(
				VkPipelineColorBlendAttachmentState{
					VK_FALSE,
					VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
					VK_BLEND_FACTOR_SRC_ALPHA,
					VK_BLEND_OP_ADD,
					VK_BLEND_FACTOR_ONE,
					VK_BLEND_FACTOR_ZERO,
					VK_BLEND_OP_ADD,
					VK_COLOR_COMPONENT_R_BIT
					| VK_COLOR_COMPONENT_G_BIT
					| VK_COLOR_COMPONENT_B_BIT
					| VK_COLOR_COMPONENT_A_BIT
				}
			);
		}

		return settings;
	}
}