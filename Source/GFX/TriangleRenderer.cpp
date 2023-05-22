#include "TriangleRenderer.h"
#include "Util\Time.h"
#include "Renderer.h"
#include "State.h"

static const std::vector<gfx::VertexPosColorUv> vertices = {
	{ {-0.5f, 0.f, -0.5f}, {0.f, 0.f, 1.f}, {0.f, 0.f} },
	{ {-0.5f, 0.f, 0.5f}, {1.f, 0.f, 0.f}, {0.f, 1.f} },
	{ {0.5f, 0.f, 0.5f}, {0.f, 0.f, 1.f}, {1.f, 1.f} },
	{ {0.5f, 0.f, 0.5f}, {0.f, 0.f, 1.f}, {1.f, 1.f} },
	{ {0.5f, 0.f, -0.5f}, {0.f, 1.f, 0.f}, {1.f, 0.f} },
	{ {-0.5f, 0.f, -0.5f}, {0.f, 0.f, 1.f}, {0.f, 0.f} }
};

struct UBO {
	mat4 model;
	mat4 view;
	mat4 proj;
};

namespace gfx {

	void TriangleRenderer::Init(PipelineCache* cache, VkRenderPass renderPass) {
		pool = DescriptorPool::Builder()
			.SetMaxSets(vk::MAX_FRAMES_IN_FLIGHT)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				vk::MAX_FRAMES_IN_FLIGHT)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				vk::MAX_FRAMES_IN_FLIGHT)
			.Create();

		layout = DescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				VK_SHADER_STAGE_VERTEX_BIT)
			.AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				VK_SHADER_STAGE_FRAGMENT_BIT)
			.Create();

		Pipeline::LayoutSettings layoutSettings;
		layoutSettings.sets.push_back(*layout);

		Shader vertShader{ "Shaders\\simple.vert.spv", VK_SHADER_STAGE_VERTEX_BIT };
		Shader fragShader{ "Shaders\\simple.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT };

		auto settings = GraphicsPipeline::DefaultSettings<VertexPosColorUv>(vertShader, fragShader, renderPass);

		pipeline = std::make_unique<GraphicsPipeline>(layoutSettings, settings, cache);

		vbuffer = std::make_unique<Buffer>(
			sizeof(VertexPosColorUv),
			vertices.size(),
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			vk::QueueFamilies::Graphics
			);
		vbuffer->Map();
		vbuffer->Write((void*)vertices.data());
		vbuffer->UnMap();

		texture = Texture::Load(
			"Resources\\statue.jpg",
			VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vk::QueueFamilies::Graphics
		);

		uniforms.resize(vk::MAX_FRAMES_IN_FLIGHT);
		sets.resize(vk::MAX_FRAMES_IN_FLIGHT);
		for (u32 i = 0; i < vk::MAX_FRAMES_IN_FLIGHT; i++) {
			uniforms[i] = std::make_unique<Buffer>(
				sizeof(UBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				vk::QueueFamilies::Graphics
				);
			uniforms[i]->Map();

			VULKAN_CHECK(
				DescriptorBuilder(*pool, *layout)
				.WriteBuffer(0, uniforms[i]->DescriptorInfo())
				.WriteImage(1, texture->DescriptorInfo())
				.Build(sets[i]),
				"Failed to write descriptor sets!"
			);
		}
	}

	void TriangleRenderer::Render(VkCommandBuffer commandBuffer, uint32_t frameIndex) {
		pipeline->Bind(commandBuffer);

		UBO ubo{};
		ubo.model = math::Rotate(mat4::Identity, vec3::Up, (f32)global.time->CurrentTime() / 1000.f * math::Pi<f32>());
		ubo.view = math::LookAt(vec3{ -1.f, 1.f, -1.f }, vec3{ 0.f });
		ubo.proj = math::Persp(math::Radians(45.f), global.renderer->Aspect(), 0.1f, 10.f);
		uniforms[frameIndex]->Write((void*)&ubo);
		uniforms[frameIndex]->Flush();

		vkCmdBindDescriptorSets(
			commandBuffer,
			pipeline->BindPoint(), pipeline->GetLayout(),
			0,
			1, &sets[frameIndex],
			0, nullptr
		);

		VkBuffer buffers[] = { *vbuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		vkCmdDraw(commandBuffer, vbuffer->InstanceCount(), 1, 0, 0);
	}
}