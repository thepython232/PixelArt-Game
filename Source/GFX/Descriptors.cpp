#include "Descriptors.h"
#include "Platform\Platform.h"
#include "State.h"

namespace gfx {

	DescriptorPool::Builder& DescriptorPool::Builder::SetMaxSets(uint32_t maxSets) {
		this->maxSets = maxSets;

		return *this;
	}

	DescriptorPool::Builder& DescriptorPool::Builder::AddPoolSize(
		VkDescriptorType type, uint32_t count) {
		VkDescriptorPoolSize poolSize{};
		poolSize.type = type;
		poolSize.descriptorCount = count;
		poolSizes.push_back(poolSize);

		return *this;
	}

	std::unique_ptr<DescriptorPool> DescriptorPool::Builder::Create() {
		return std::make_unique<DescriptorPool>(maxSets, poolSizes);
	}

	DescriptorPool::DescriptorPool(uint32_t maxSets,
		const std::vector<VkDescriptorPoolSize>& poolSizes) {
		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.maxSets = maxSets;
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		createInfo.pPoolSizes = poolSizes.data();

		VULKAN_CHECK(
			vkCreateDescriptorPool(*global.platform->device,
				&createInfo, nullptr, &pool),
			"Failed to create descriptor pool!"
		);
	}

	DescriptorPool::~DescriptorPool() {
		vkDestroyDescriptorPool(*global.platform->device,
			pool, nullptr);
	}

	VkResult DescriptorPool::AllocateDescriptor(VkDescriptorSetLayout layout,
		VkDescriptorSet& set) {
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		return vkAllocateDescriptorSets(*global.platform->device,
			&allocInfo, &set);
	}

	DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::AddBinding(
		uint32_t binding, VkDescriptorType type, VkShaderStageFlags stages) {
		VkDescriptorSetLayoutBinding setBinding{};
		setBinding.binding = binding;
		setBinding.stageFlags = stages;
		setBinding.descriptorType = type;
		setBinding.descriptorCount = 1;
		bindings[binding] = setBinding;

		return *this;
	}

	std::unique_ptr<DescriptorSetLayout> DescriptorSetLayout::Builder::Create() {
		return std::make_unique<DescriptorSetLayout>(bindings);
	}

	DescriptorSetLayout::DescriptorSetLayout(const BindingMap& bindings)
		: bindings(bindings) {
		std::vector<VkDescriptorSetLayoutBinding> bindingList;
		for (const auto& kv : bindings) {
			bindingList.push_back(kv.second);
		}

		VkDescriptorSetLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = static_cast<uint32_t>(bindingList.size());
		createInfo.pBindings = bindingList.data();

		VULKAN_CHECK(
			vkCreateDescriptorSetLayout(*global.platform->device,
				&createInfo, nullptr, &layout),
			"Failed to create descriptor set layout!"
		);
	}

	DescriptorSetLayout::~DescriptorSetLayout() {
		vkDestroyDescriptorSetLayout(*global.platform->device,
			layout, nullptr);
	}

	DescriptorBuilder::DescriptorBuilder(DescriptorPool& pool,
		const DescriptorSetLayout& layout) : pool(pool), layout(layout) {

	}

	DescriptorBuilder& DescriptorBuilder::WriteBuffer(uint32_t binding,
		const VkDescriptorBufferInfo& bufferInfo, uint32_t index) {
		ASSERT(layout.bindings.contains(binding),
			"Descriptor layout doesn't have that binding!");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount = 1;
		write.descriptorType = layout.bindings
			.find(binding)->second.descriptorType;
		write.dstArrayElement = index;
		write.dstBinding = binding;
		
		bufferInfos.push_back(bufferInfo);
		write.pBufferInfo = &bufferInfos.back();

		writes.push_back(write);

		return *this;
	}

	DescriptorBuilder& DescriptorBuilder::WriteImage(uint32_t binding,
		const VkDescriptorImageInfo& imageInfo, uint32_t index) {
		ASSERT(layout.bindings.contains(binding),
			"Descriptor layout doesn't have that binding!");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount = 1;
		write.descriptorType = layout.bindings
			.find(binding)->second.descriptorType;
		write.dstArrayElement = index;
		write.dstBinding = binding;

		imageInfos.push_back(imageInfo);
		write.pImageInfo = &imageInfos.back();

		writes.push_back(write);

		return *this;
	}

	void DescriptorBuilder::Overwrite(VkDescriptorSet& set) {
		for (auto& write : writes) {
			write.dstSet = set;
		}

		vkUpdateDescriptorSets(*global.platform->device,
			static_cast<uint32_t>(writes.size()),
			writes.data(),
			0, nullptr);
	}

	VkResult DescriptorBuilder::Build(VkDescriptorSet& set) {
		VkResult result = pool.AllocateDescriptor(layout, set);
		if (result == VK_SUCCESS) {
			Overwrite(set);
		}

		return result;
	}
}