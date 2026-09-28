/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GodRays/GodRaysFeatureProcessorInterface.h>
#include <Atom/RPI.Public/Pass/Pass.h>

namespace GodRays
{
    class GodRaysFeatureProcessor final
        : public GodRaysFeatureProcessorInterface
    {
    public:
        AZ_RTTI(GodRaysFeatureProcessor, "{0FD6C12C-0D2C-4C47-A7F4-658102887E73}", GodRaysFeatureProcessorInterface);
        AZ_CLASS_ALLOCATOR(GodRaysFeatureProcessor, AZ::SystemAllocator)

        static void Reflect(AZ::ReflectContext* context);

        GodRaysFeatureProcessor() = default;
        virtual ~GodRaysFeatureProcessor() = default;

        // FeatureProcessor overrides
        void Activate() override;
        void Deactivate() override;
        void Simulate(const FeatureProcessor::SimulatePacket& packet) override;
        GodRaysHandle AcquireGodRays() override;
        void ReleaseGodRays(GodRaysHandle& handle) override;
        const GodRaysData* GetActiveGodRays() const override;
        void AddRenderPasses(AZ::RPI::RenderPipeline* pipeline) override;
        void OnRenderPipelineChanged(AZ::RPI::RenderPipeline* pipeline, AZ::RPI::SceneNotification::RenderPipelineChangeType changeType) override;

    private:
        AZStd::vector<AZ::RPI::Ptr<AZ::RPI::Pass>> m_passes;
        AZStd::vector<GodRaysHandle> m_instances;

    };
}
