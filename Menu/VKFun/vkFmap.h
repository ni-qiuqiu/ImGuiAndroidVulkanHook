#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_android.h>

#include <unordered_map>
#include <string>

//TODO: mabe have error or dismatch def

#define VK_RESULT_CALL(funcname, ...) \
    do { \
        return ((PFN_##funcname)vkFunctionMap[#funcname])(##__VA_ARGS__); \
    } while (0)

#define VK_VOID_CALL(funcname, ...) \
    do { \
        ((PFN_##funcname)vkFunctionMap[#funcname])(##__VA_ARGS__); \
    } while (0)





std::unordered_map<std::string, PFN_vkVoidFunction> vkFunctionMap;


VkResult m_vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance){
    VK_RESULT_CALL(vkCreateInstance, pCreateInfo, pAllocator, pInstance);
}


void m_vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyInstance, instance, pAllocator);
}

VkResult m_vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices){
    VK_RESULT_CALL(vkEnumeratePhysicalDevices, instance, pPhysicalDeviceCount, pPhysicalDevices);
}

void m_vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures){
    VK_VOID_CALL(vkGetPhysicalDeviceFeatures, physicalDevice, pFeatures);
}

void m_vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties){
    VK_VOID_CALL(vkGetPhysicalDeviceFormatProperties, physicalDevice, format, pFormatProperties);
}

VkResult m_vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties){
    VK_RESULT_CALL(vkGetPhysicalDeviceImageFormatProperties, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

void m_vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties){
    VK_VOID_CALL(vkGetPhysicalDeviceProperties, physicalDevice, pProperties);
}

void m_vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties){
    VK_VOID_CALL(vkGetPhysicalDeviceQueueFamilyProperties, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void m_vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties){
    VK_VOID_CALL(vkGetPhysicalDeviceMemoryProperties, physicalDevice, pMemoryProperties);
}


VkResult m_vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice){
    VK_RESULT_CALL(vkCreateDevice, physicalDevice, pCreateInfo, pAllocator, pDevice);
}

void m_vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyDevice, device, pAllocator);
}

VkResult m_vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties){
    VK_RESULT_CALL(vkEnumerateInstanceExtensionProperties, pLayerName, pPropertyCount, pProperties);
}

VkResult m_vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties){
    VK_RESULT_CALL(vkEnumerateDeviceExtensionProperties, physicalDevice, pLayerName, pPropertyCount, pProperties);
}

VkResult m_vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties){
    VK_RESULT_CALL(vkEnumerateInstanceLayerProperties, pPropertyCount, pProperties);
}

VkResult m_vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties){
    VK_RESULT_CALL(vkEnumerateDeviceLayerProperties, physicalDevice, pPropertyCount, pProperties);
}

void m_vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue){
    VK_VOID_CALL(vkGetDeviceQueue, device, queueFamilyIndex, queueIndex, pQueue);
}

VkResult m_vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence){
    VK_RESULT_CALL(vkQueueSubmit, queue, submitCount, pSubmits, fence);
}

VkResult m_vkQueueWaitIdle(VkQueue queue){
    VK_RESULT_CALL(vkQueueWaitIdle, queue);
}

VkResult m_vkDeviceWaitIdle(VkDevice device){
    VK_RESULT_CALL(vkDeviceWaitIdle, device);
}

VkResult m_vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory){
    VK_RESULT_CALL(vkAllocateMemory, device, pAllocateInfo, pAllocator, pMemory);
}

void m_vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator){   
    VK_VOID_CALL(vkFreeMemory, device, memory, pAllocator);
}

VkResult m_vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData){
    VK_RESULT_CALL(vkMapMemory, device, memory, offset, size, flags, ppData);
}

void m_vkUnmapMemory(VkDevice device, VkDeviceMemory memory){
    VK_VOID_CALL(vkUnmapMemory, device, memory);
}

VkResult m_vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges){     
    VK_RESULT_CALL(vkFlushMappedMemoryRanges, device, memoryRangeCount, pMemoryRanges);
}

VkResult m_vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges){
    VK_RESULT_CALL(vkInvalidateMappedMemoryRanges, device, memoryRangeCount, pMemoryRanges);
}

void m_vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes){
    VK_VOID_CALL(vkGetDeviceMemoryCommitment, device, memory, pCommittedMemoryInBytes);
}

VkResult m_vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset){
    VK_RESULT_CALL(vkBindBufferMemory, device, buffer, memory, memoryOffset);
}

VkResult m_vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset){
    VK_RESULT_CALL(vkBindImageMemory, device, image, memory, memoryOffset);
}

void m_vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements){
    VK_VOID_CALL(vkGetBufferMemoryRequirements, device, buffer, pMemoryRequirements);
}

void m_vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements){ 
    VK_VOID_CALL(vkGetImageMemoryRequirements, device, image, pMemoryRequirements);
}

void m_vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements){
    VK_VOID_CALL(vkGetImageSparseMemoryRequirements, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void m_vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties){
    VK_VOID_CALL(vkGetPhysicalDeviceSparseImageFormatProperties, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}

VkResult m_vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence){
    VK_RESULT_CALL(vkQueueBindSparse, queue, bindInfoCount, pBindInfo, fence);
}

VkResult m_vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence){
    VK_RESULT_CALL(vkCreateFence, device, pCreateInfo, pAllocator, pFence);
}

void m_vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyFence, device, fence, pAllocator);
}

VkResult m_vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences){
    VK_RESULT_CALL(vkResetFences, device, fenceCount, pFences);
}

VkResult m_vkGetFenceStatus(VkDevice device, VkFence fence){
    VK_RESULT_CALL(vkGetFenceStatus, device, fence);
}

VkResult m_vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout){
    VK_RESULT_CALL(vkWaitForFences, device, fenceCount, pFences, waitAll, timeout);
}

VkResult m_vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore){
    VK_RESULT_CALL(vkCreateSemaphore, device, pCreateInfo, pAllocator, pSemaphore);
}

void m_vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroySemaphore, device, semaphore, pAllocator);
}

VkResult m_vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent){
    VK_RESULT_CALL(vkCreateEvent, device, pCreateInfo, pAllocator, pEvent);
}

void m_vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyEvent, device, event, pAllocator);
}

VkResult m_vkGetEventStatus(VkDevice device, VkEvent event){
    VK_RESULT_CALL(vkGetEventStatus, device, event);
}

VkResult m_vkSetEvent(VkDevice device, VkEvent event){
    VK_RESULT_CALL(vkSetEvent, device, event);
}

VkResult m_vkResetEvent(VkDevice device, VkEvent event){
    VK_RESULT_CALL(vkResetEvent, device, event);
}

VkResult m_vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool){
    VK_RESULT_CALL(vkCreateQueryPool, device, pCreateInfo, pAllocator, pQueryPool);
}

void m_vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyQueryPool, device, queryPool, pAllocator);    
}

VkResult m_vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags){
    VK_RESULT_CALL(vkGetQueryPoolResults, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

VkResult m_vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer){
    VK_RESULT_CALL(vkCreateBuffer, device, pCreateInfo, pAllocator, pBuffer);
}

void m_vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyBuffer, device, buffer, pAllocator);
}

VkResult m_vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView){
    VK_RESULT_CALL(vkCreateBufferView, device, pCreateInfo, pAllocator, pView);
}

void m_vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyBufferView, device, bufferView, pAllocator);
}

VkResult m_vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage){
    VK_RESULT_CALL(vkCreateImage, device, pCreateInfo, pAllocator, pImage);
}

void m_vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyImage, device, image, pAllocator);
}

void m_vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout){
    VK_VOID_CALL(vkGetImageSubresourceLayout, device, image, pSubresource, pLayout);
}

VkResult m_vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView){
    VK_RESULT_CALL(vkCreateImageView, device, pCreateInfo, pAllocator, pView);
}

void m_vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyImageView, device, imageView, pAllocator);
}

VkResult m_vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule){
    VK_RESULT_CALL(vkCreateShaderModule, device, pCreateInfo, pAllocator, pShaderModule);
}

void m_vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyShaderModule, device, shaderModule, pAllocator);
}

VkResult m_vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache){
    VK_RESULT_CALL(vkCreatePipelineCache, device, pCreateInfo, pAllocator, pPipelineCache);
}

void m_vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyPipelineCache, device, pipelineCache, pAllocator);
}

VkResult m_vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout){
    VK_RESULT_CALL(vkCreatePipelineLayout, device, pCreateInfo, pAllocator, pPipelineLayout);
}

void m_vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyPipelineLayout, device, pipelineLayout, pAllocator);
}

VkResult m_vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler){
    VK_RESULT_CALL(vkCreateSampler, device, pCreateInfo, pAllocator, pSampler);
}

void m_vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroySampler, device, sampler, pAllocator);
}

VkResult m_vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout){
    VK_RESULT_CALL(vkCreateDescriptorSetLayout, device, pCreateInfo, pAllocator, pSetLayout);
}

void m_vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyDescriptorSetLayout, device, descriptorSetLayout, pAllocator);
}

VkResult m_vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool){
    VK_RESULT_CALL(vkCreateDescriptorPool, device, pCreateInfo, pAllocator, pDescriptorPool);
}

void m_vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyDescriptorPool, device, descriptorPool, pAllocator);
}

VkResult m_vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer){
    VK_RESULT_CALL(vkCreateFramebuffer, device, pCreateInfo, pAllocator, pFramebuffer);
}

void m_vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyFramebuffer, device, framebuffer, pAllocator);
}

VkResult m_vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass){
    VK_RESULT_CALL(vkCreateRenderPass, device, pCreateInfo, pAllocator, pRenderPass);
}

void m_vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyRenderPass, device, renderPass, pAllocator);
}

VkResult m_vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool){
    VK_RESULT_CALL(vkCreateCommandPool, device, pCreateInfo, pAllocator, pCommandPool);
}

void m_vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator){
    VK_VOID_CALL(vkDestroyCommandPool, device, commandPool, pAllocator);
}

void m_vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers){
    VK_VOID_CALL(vkFreeCommandBuffers, device, commandPool, commandBufferCount, pCommandBuffers);
}


void m_vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo){
    VK_VOID_CALL(vkBeginCommandBuffer, commandBuffer, pBeginInfo);
}

void m_vkEndCommandBuffer(VkCommandBuffer commandBuffer){
    VK_VOID_CALL(vkEndCommandBuffer, commandBuffer);
}

void m_vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags){
    VK_VOID_CALL(vkResetCommandBuffer, commandBuffer, flags);
}

void m_vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline){
    VK_VOID_CALL(vkCmdBindPipeline, commandBuffer, pipelineBindPoint, pipeline);
}

void m_vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports){
    VK_VOID_CALL(vkCmdSetViewport, commandBuffer, firstViewport, viewportCount, pViewports);
}

void m_vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors){
    VK_VOID_CALL(vkCmdSetScissor, commandBuffer, firstScissor, scissorCount, pScissors);
}

void m_vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth){
    VK_VOID_CALL(vkCmdSetLineWidth, commandBuffer, lineWidth);
}

void m_vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor){
    VK_VOID_CALL(vkCmdSetDepthBias, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void m_vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]){
    VK_VOID_CALL(vkCmdSetBlendConstants, commandBuffer, blendConstants);
}

void m_vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds){
    VK_VOID_CALL(vkCmdSetDepthBounds, commandBuffer, minDepthBounds, maxDepthBounds);
}

void m_vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask){
    VK_VOID_CALL(vkCmdSetStencilCompareMask, commandBuffer, faceMask, compareMask);
}

void m_vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference){    
    VK_VOID_CALL(vkCmdSetStencilReference, commandBuffer, faceMask, reference);
}

void m_vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets){
    VK_VOID_CALL(vkCmdBindDescriptorSets, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void m_vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType){
    VK_VOID_CALL(vkCmdBindIndexBuffer, commandBuffer, buffer, offset, indexType);
}

void m_vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets){
    VK_VOID_CALL(vkCmdBindVertexBuffers, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void m_vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance){
    VK_VOID_CALL(vkCmdDraw, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void m_vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance){
    VK_VOID_CALL(vkCmdDrawIndexed, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void m_vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride){
    VK_VOID_CALL(vkCmdDrawIndirect, commandBuffer, buffer, offset, drawCount, stride);
}

void m_vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride){
    VK_VOID_CALL(vkCmdDrawIndexedIndirect, commandBuffer, buffer, offset, drawCount, stride);
}

void m_vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ){
    VK_VOID_CALL(vkCmdDispatch, commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void m_vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset){
    VK_VOID_CALL(vkCmdDispatchIndirect, commandBuffer, buffer, offset);
}

void m_vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions){
    VK_VOID_CALL(vkCmdCopyBuffer, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void m_vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions){
    VK_VOID_CALL(vkCmdCopyImage, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void m_vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter){
    VK_VOID_CALL(vkCmdBlitImage, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void m_vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions){
    VK_VOID_CALL(vkCmdCopyBufferToImage, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void m_vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions){
    VK_VOID_CALL(vkCmdCopyImageToBuffer, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void m_vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData){
    VK_VOID_CALL(vkCmdUpdateBuffer, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void m_vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data){
    VK_VOID_CALL(vkCmdFillBuffer, commandBuffer, dstBuffer, dstOffset, size, data);
}

void m_vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges){
    VK_VOID_CALL(vkCmdClearColorImage, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void m_vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges){
    VK_VOID_CALL(vkCmdClearDepthStencilImage, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void m_vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects){
    VK_VOID_CALL(vkCmdClearAttachments, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void m_vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions){
    VK_VOID_CALL(vkCmdResolveImage, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void m_vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask){
    VK_VOID_CALL(vkCmdSetEvent, commandBuffer, event, stageMask);
}

void m_vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask){   
    VK_VOID_CALL(vkCmdResetEvent, commandBuffer, event, stageMask);
}

void m_vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers){
    VK_VOID_CALL(vkCmdWaitEvents, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void m_vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers){
    VK_VOID_CALL(vkCmdPipelineBarrier, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void m_vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents){
    VK_VOID_CALL(vkCmdBeginRenderPass, commandBuffer, pRenderPassBegin, contents);
}

void m_vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents){
    VK_VOID_CALL(vkCmdNextSubpass, commandBuffer, contents);
}

void m_vkCmdEndRenderPass(VkCommandBuffer commandBuffer){
    VK_VOID_CALL(vkCmdEndRenderPass, commandBuffer);
}

void m_vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers){
    VK_VOID_CALL(vkCmdExecuteCommands, commandBuffer, commandBufferCount, pCommandBuffers);
}

void m_vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes){
    VK_VOID_CALL(vkCmdBindTransformFeedbackBuffersEXT, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void m_vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets){
    VK_VOID_CALL(vkCmdBeginTransformFeedbackEXT, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void m_vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets){
    VK_VOID_CALL(vkCmdEndTransformFeedbackEXT, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}


VkResult m_vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches){
    VK_RESULT_CALL(vkMergePipelineCaches, device, dstCache, srcCacheCount, pSrcCaches);
}

VkResult m_vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines){
    VK_RESULT_CALL(vkCreateGraphicsPipelines, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult m_vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines){
    VK_RESULT_CALL(vkCreateComputePipelines, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}






