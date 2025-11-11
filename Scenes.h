#pragma once

#include "SceneSwitcher.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Scene selection enum exported for cross-module use

typedef enum ActiveSceneTag
{
    ACTIVE_SCENE_NONE   = -1,
    ACTIVE_SCENE_SCENE0 = 0,
    ACTIVE_SCENE_SCENE1 = 1,
    ACTIVE_SCENE_SCENE2 = 2,
    ACTIVE_SCENE_SCENE3 = 3
} ActiveScene;

extern ActiveScene gActiveScene;

static inline void Scenes_InitUniformIdentity(GlobalContext_MyUniformData* data)
{
    if (data == NULL)
    {
        return;
    }

    memset(data, 0, sizeof(GlobalContext_MyUniformData));
    data->modelMatrix = glm::mat4(1.0f);
    data->viewMatrix = glm::mat4(1.0f);
    data->projectionMatrix = glm::mat4(1.0f);
    data->fade = glm::vec4(0.0f);
}

static inline VkResult Scenes_WriteUniformData(VkDevice device,
                                               VkDeviceMemory memory,
                                               const GlobalContext_MyUniformData* data,
                                               FILE* logFile,
                                               const char* tag)
{
    if (device == VK_NULL_HANDLE || memory == VK_NULL_HANDLE || data == NULL)
    {
        if (logFile != NULL && tag != NULL)
        {
            fprintf(logFile, "%s --> invalid uniform buffer parameters\n", tag);
        }
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    void* mapped = NULL;
    VkResult vkResult = vkMapMemory(device,
                                    memory,
                                    0,
                                    sizeof(GlobalContext_MyUniformData),
                                    0,
                                    &mapped);
    if (vkResult != VK_SUCCESS)
    {
        if (logFile != NULL && tag != NULL)
        {
            fprintf(logFile, "%s --> vkMapMemory() failed %d\n", tag, vkResult);
        }
        return vkResult;
    }

    if (mapped == NULL)
    {
        if (logFile != NULL && tag != NULL)
        {
            fprintf(logFile, "%s --> vkMapMemory() returned NULL pointer\n", tag);
        }
        vkUnmapMemory(device, memory);
        return VK_ERROR_MEMORY_MAP_FAILED;
    }

    memcpy(mapped, data, sizeof(GlobalContext_MyUniformData));
    vkUnmapMemory(device, memory);
    return VK_SUCCESS;
}

// Scene 0 interface
VkResult Scene0_UpdateUniformBuffer(void);

// Scene 1 shader modules (shared with SceneSwitcher.cpp)
extern VkShaderModule gShaderModule_vertex_scene1;
extern VkShaderModule gShaderModule_fragment_scene1;

// Scene 2 shader modules
extern VkShaderModule gShaderModule_vertex_scene2;
extern VkShaderModule gShaderModule_fragment_scene2;

// Scene 3 shader modules
extern VkShaderModule gShaderModule_vertex_scene3;
extern VkShaderModule gShaderModule_fragment_scene3;

// Scene 1 interface
VkResult Scene1_CreateTextures(void);
void Scene1_DestroyTextures(void);
void Scene1_BeginNewPan(void);
void Scene1_UpdateCameraAnim(void);
VkResult Scene1_UpdateUniformBuffer(void);
void Scene1_StartSequence(void);
void Scene1_StopSequence(void);
void Scene1_UpdateSequence(void);
BOOL Scene1_IsSequenceActive(void);
void Scene1_AdjustPanSpeed(float delta);
void Scene1_AdjustOverlaySize(float delta);
float Scene1_GetPendingBlendFade(void);
void Scene1_CommitPendingBlendFade(void);
void Scene1_UpdateBlendFade(float fade);
BOOL Scene1_IsCommandBufferDirty(void);
void Scene1_ClearCommandBufferDirty(void);
void Scene1_MarkCommandBufferDirty(void);
BOOL Scene1_HasPendingOverlay(void);
VkResult Scene1_BindPendingOverlay(VkDescriptorSet descriptorSet);
void Scene1_GetSkyDescriptor(VkDescriptorImageInfo* info);
void Scene1_GetOverlayDescriptor(VkDescriptorImageInfo* info);

// Scene 2 interface
VkResult Scene2_UpdateUniformBuffer(void);

// Scene 3 interface
VkResult Scene3_UpdateUniformBuffer(void);

#ifdef __cplusplus
}
#endif

