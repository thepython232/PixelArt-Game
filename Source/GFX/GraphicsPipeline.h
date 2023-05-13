#pragma once

#include "Pipeline.h"
#include "Vertex.h"

namespace gfx {

	class GraphicsPipeline : public Pipeline {
	public:
		struct Settings {
			std::vector<const Shader*> shaders;

			VkRenderPass renderPass;
			uint32_t subpass = 0;

			//Input assembly
			struct {
				VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				VkBool32 restartEnable = VK_FALSE;
			} inputAssembly;

			//Vertex input
			struct {
				std::vector<VkVertexInputBindingDescription> bindings;
				std::vector<VkVertexInputAttributeDescription> attributes;
			} vertexInput;

			//Viewport
			struct {
				std::vector<VkViewport> viewports;
				std::vector<VkRect2D> scissors;
			} viewport;

			//Dynamic States
			std::vector<VkDynamicState> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			//Rasterization
			struct {
				VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE; //TODO
				VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
				VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
				float lineWidth = 1.f;
				VkBool32 discard = VK_FALSE;
				VkBool32 depthBiasEnable = VK_FALSE;
				float depthBiasClamp = 0.f;
				float depthBiasConstantFactor = 0.f;
				float depthBiasSlopeFactor = 0.f;
			} rasterization;

			//Multisample
			struct {
				VkSampleCountFlagBits rasterSamples = VK_SAMPLE_COUNT_1_BIT;
				std::optional<VkSampleMask> sampleMask = std::nullopt;
				VkBool32 sampleShading = VK_FALSE;
				float minSampleShading = 0.f;
				VkBool32 alphaToOne = VK_FALSE;
				VkBool32 alphaToCoverage = VK_FALSE;
			} multisampling;

			//Blending
			struct {
				VkBool32 enableLogicOp = VK_FALSE;
				VkLogicOp logicOp;
				std::vector<VkPipelineColorBlendAttachmentState> attachments;
				float constants[4] = { 1.f, 1.f, 1.f, 1.f };
			} blending;

			//Depth
			struct {
				bool enabled = false;
				VkCompareOp compareOp = VK_COMPARE_OP_LESS;
				VkBool32 test = VK_TRUE;
				VkBool32 write = VK_TRUE;
				VkBool32 bounds = VK_FALSE;
				float minBounds = 0.f;
				float maxBounds = 1.f;
				VkBool32 testStencil = VK_FALSE;
				VkStencilOpState stencilFront;
				VkStencilOpState stencilBack;
			} depth;

			//Tesselation
			struct {
				bool enabled = false;
				uint32_t controlPoints = 0;
			} tesselation;
		};

		GraphicsPipeline(
			const LayoutSettings& layoutSettings,
			const Settings& settings,
			PipelineCache* cache = nullptr
		);

		VkPipelineBindPoint BindPoint() const { return VK_PIPELINE_BIND_POINT_GRAPHICS; }

		template<IsVertex V>
		static Settings DefaultSettings(
			const Shader& vertShader,
			const Shader& fragShader,
			VkRenderPass renderPass,
			uint32_t attachmentCount = 1
		) {
			Settings settings = DefaultSettings(vertShader, fragShader, renderPass, attachmentCount);
			settings.vertexInput.bindings.push_back(V::Binding);
			settings.vertexInput.attributes = V::Attributes;

			return settings;
		}

		static Settings DefaultSettings(
			const Shader& vertShader,
			const Shader& fragShader,
			VkRenderPass renderPass,
			uint32_t attachmentCount = 1
		);
	};
}