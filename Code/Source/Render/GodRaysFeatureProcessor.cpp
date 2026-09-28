/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GodRaysFeatureProcessor.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/RPI.Reflect/Pass/PassRequest.h>

namespace GodRays
{
    void GodRaysFeatureProcessor::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext
                ->Class<GodRaysFeatureProcessor, FeatureProcessor>()
                ;
        }
    }

    void GodRaysFeatureProcessor::Activate()
    {
        EnableSceneNotification();
    }

    void GodRaysFeatureProcessor::Deactivate()
    {
        DisableSceneNotification();
        for (auto& pass : m_passes)
        {
            pass->QueueForRemoval();
        }
        m_passes.clear();
        m_instances.clear();
    }

    void GodRaysFeatureProcessor::Simulate([[maybe_unused]] const FeatureProcessor::SimulatePacket& packet)
    {

    }

    void GodRaysFeatureProcessor::AddRenderPasses(AZ::RPI::RenderPipeline* pipeline)
    {
        if (pipeline->FindFirstPass(AZ::Name("GodRaysPass")) ||
            !pipeline->FindFirstPass(AZ::Name("DepthPrePass")) ||
            !pipeline->FindFirstPass(AZ::Name("OpaquePass")) ||
            !pipeline->FindFirstPass(AZ::Name("TransparentPass")))
        {
            return;
        }
        AZ::RPI::PassRequest request;
        request.m_passName = AZ::Name("GodRaysPass");
        request.m_templateName = AZ::Name("GodRaysPassTemplate");
        request.m_connections = {
            { AZ::Name("Depth"), { AZ::Name("DepthPrePass"), AZ::Name("Depth") } },
            { AZ::Name("Color"), { AZ::Name("OpaquePass"), AZ::Name("Output") } }
        };
        auto pass = AZ::RPI::PassSystemInterface::Get()->CreatePassFromRequest(&request);
        if (pass && pipeline->AddPassBefore(pass, AZ::Name("TransparentPass")))
        {
            // Release passes from a previous rebuild of this pipeline.
            AZStd::erase_if(m_passes, [pipeline](const auto& oldPass)
            {
                return !oldPass->GetRenderPipeline() || oldPass->GetRenderPipeline() == pipeline;
            });
            m_passes.push_back(pass);
            AZ_Printf("GodRays", "Installed screen-space GodRaysPass (depth only).\n");
        }
    }

    void GodRaysFeatureProcessor::OnRenderPipelineChanged(AZ::RPI::RenderPipeline* pipeline, RenderPipelineChangeType changeType)
    {
        if (changeType == RenderPipelineChangeType::Removed)
        {
            AZStd::erase_if(m_passes, [pipeline](const auto& pass)
            {
                return !pass->GetRenderPipeline() || pass->GetRenderPipeline() == pipeline;
            });
        }
    }

    GodRaysHandle GodRaysFeatureProcessor::AcquireGodRays()
    {
        auto handle = AZStd::make_shared<GodRaysData>();
        m_instances.push_back(handle);
        return handle;
    }

    void GodRaysFeatureProcessor::ReleaseGodRays(GodRaysHandle& handle)
    {
        AZStd::erase(m_instances, handle);
        handle.reset();
    }

    const GodRaysData* GodRaysFeatureProcessor::GetActiveGodRays() const
    {
        for (const auto& instance : m_instances)
        {
            if (instance->m_configuration.m_enabled && instance->m_configuration.m_intensity > 0.0f && instance->m_configuration.m_maxBrightness > 0.0f)
            {
                return instance.get();
            }
        }
        return nullptr;
    }
}
