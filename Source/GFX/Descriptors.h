#pragma once

#include "Platform\Device.h"

namespace gfx {

	class DescriptorPool {
	public:
		struct Builder {
			Builder& SetMaxSets(uint32_t maxSets);
			Builder& AddPoolSize(VkDescriptorType type, uint32_t count);
			std::unique_ptr<DescriptorPool> Create();

		private:
			uint32_t maxSets = 0;
			std::vector<VkDescriptorPoolSize> poolSizes;
		};

		DescriptorPool(uint32_t maxSets,
			const std::vector<VkDescriptorPoolSize>& poolSizes);
		~DescriptorPool();

		inline operator VkDescriptorPool() const { return pool; }
		inline VkDescriptorPool GetPool() const { return pool; }

		VkResult AllocateDescriptor(VkDescriptorSetLayout layout,
			VkDescriptorSet& set);

	private:
		VkDescriptorPool pool;
	};

	class DescriptorSetLayout {
	public:
		using BindingMap = std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>;

		struct Builder {
			Builder& AddBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags stages);
			std::unique_ptr<DescriptorSetLayout> Create();

		private:
			BindingMap bindings;
		};

		DescriptorSetLayout(const BindingMap& bindings);
		~DescriptorSetLayout();

		inline operator VkDescriptorSetLayout() const { return layout; }
		inline VkDescriptorSetLayout GetLayout() const { return layout; }

	private:
		VkDescriptorSetLayout layout;
		BindingMap bindings;

		friend class DescriptorBuilder;
	};

	class DescriptorBuilder {
	public:
		DescriptorBuilder(DescriptorPool& pool,
			const DescriptorSetLayout& layout);

		DescriptorBuilder& WriteBuffer(uint32_t binding,
			const VkDescriptorBufferInfo& bufferInfo, uint32_t index = 0);
		DescriptorBuilder& WriteImage(uint32_t binding,
			const VkDescriptorImageInfo& bufferInfo, uint32_t index = 0);

		void Overwrite(VkDescriptorSet& set);
		VkResult Build(VkDescriptorSet& set);

	private:
		std::list<VkDescriptorBufferInfo> bufferInfos;
		std::list<VkDescriptorImageInfo> imageInfos;

		std::vector<VkWriteDescriptorSet> writes;

		DescriptorPool& pool;
		const DescriptorSetLayout& layout;
	};
}