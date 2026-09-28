/* SPDX-License-Identifier: Apache-2.0 OR MIT */
#include "GodRaysPass.h"
#include <GodRays/GodRaysFeatureProcessorInterface.h>
#include <AzCore/Console/IConsole.h>
#include <AzCore/Math/Vector4.h>
#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/RPI.Public/Scene.h>
#include <cmath>

namespace GodRays
{
    AZ_CVAR(bool, r_godRays, true, nullptr, AZ::ConsoleFunctorFlags::Null, "Global enable for GodRays components.");

    namespace
    {
        float Bounded(float value, float fallback, float minimum, float maximum)
        {
            return std::isfinite(value) ? AZStd::clamp(value, minimum, maximum) : fallback;
        }
        const GodRaysData* GetSettings(const AZ::RPI::RenderPipeline* pipeline)
        {
            const auto* scene = pipeline ? pipeline->GetScene() : nullptr;
            const auto* fp = scene ? scene->GetFeatureProcessor<GodRaysFeatureProcessorInterface>() : nullptr;
            return fp ? fp->GetActiveGodRays() : nullptr;
        }
    }

    AZ::RPI::Ptr<GodRaysParentPass> GodRaysParentPass::Create(const AZ::RPI::PassDescriptor& descriptor)
    {
        return aznew GodRaysParentPass(descriptor);
    }
    GodRaysParentPass::GodRaysParentPass(const AZ::RPI::PassDescriptor& descriptor) : ParentPass(descriptor) {}
    bool GodRaysParentPass::IsEnabled() const
    {
        return ParentPass::IsEnabled() && r_godRays && GetSettings(GetRenderPipeline());
    }
    void GodRaysParentPass::FrameBeginInternal(FramePrepareParams params)
    {
        if (const auto* data = GetSettings(GetRenderPipeline()))
        {
            float scale = 0.5f;
            switch (data->m_configuration.m_resolution)
            {
            case GodRaysResolution::Full: scale = 1.0f; break;
            case GodRaysResolution::Quarter: scale = 0.25f; break;
            default: break;
            }
            // Set the mask dimensions BEFORE the child creates its transient
            // attachment. Other children inherit its size through SizeSource.
            for (const auto& child : GetChildren())
            {
                if (child->GetName() == AZ::Name("GodRaysMask"))
                {
                    if (auto* mask = azrtti_cast<GodRaysPass*>(child.get()))
                    {
                        mask->SetResolutionScale(scale);
                    }
                }
            }
        }
        ParentPass::FrameBeginInternal(params);
    }

    AZ::RPI::Ptr<GodRaysPass> GodRaysPass::Create(const AZ::RPI::PassDescriptor& descriptor)
    {
        return aznew GodRaysPass(descriptor);
    }
    GodRaysPass::GodRaysPass(const AZ::RPI::PassDescriptor& descriptor) : FullscreenTrianglePass(descriptor) {}
    void GodRaysPass::SetResolutionScale(float scale)
    {
        for (auto& attachment : m_ownedAttachments)
        {
            attachment->m_sizeMultipliers.m_widthMultiplier = scale;
            attachment->m_sizeMultipliers.m_heightMultiplier = scale;
        }
    }
    void GodRaysPass::FrameBeginInternal(FramePrepareParams params)
    {
        const auto* data = GetSettings(GetRenderPipeline());
        if (data && m_shaderResourceGroup)
        {
            const auto& cfg = data->m_configuration;
            auto set = [this](const char* name, const AZ::Vector4& value)
            {
                auto index = m_shaderResourceGroup->FindShaderInputConstantIndex(AZ::Name(name));
                if (index.IsValid()) { m_shaderResourceGroup->SetConstant(index, value); }
            };
            set("m_shape", AZ::Vector4(
                Bounded(cfg.m_rayLength, 0.85f, 0.0f, 1.0f),
                Bounded(cfg.m_falloff, 2.0f, 0.0f, 8.0f),
                Bounded(cfg.m_sourceRadius, 0.35f, 0.02f, 1.5f),
                Bounded(cfg.m_sourceSoftness, 0.65f, 0.01f, 1.0f)));
            set("m_mix", AZ::Vector4(
                Bounded(cfg.m_intensity, 0.15f, 0.0f, 4.0f),
                Bounded(cfg.m_maxBrightness, 0.25f, 0.0f, 4.0f),
                Bounded(cfg.m_backgroundAmount, 0.0f, 0.0f, 1.0f),
                Bounded(cfg.m_endFadeStart, 0.8f, 0.0f, 0.99f)));
            set("m_quality", AZ::Vector4(
                static_cast<float>(AZStd::clamp(cfg.m_samples, 8u, 96u)),
                Bounded(cfg.m_edgeFade, 0.3f, 0.01f, 1.0f),
                Bounded(cfg.m_filterStrength, 0.65f, 0.0f, 1.0f), 0.0f));
            set("m_tint", AZ::Vector4(
                Bounded(cfg.m_color.GetR(), 1.0f, 0.0f, 1.0f),
                Bounded(cfg.m_color.GetG(), 0.95f, 0.0f, 1.0f),
                Bounded(cfg.m_color.GetB(), 0.85f, 0.0f, 1.0f), 0.0f));
            set("m_direction", AZ::Vector4(data->m_direction, 0.0f));
            if (auto* parent = GetParent())
            {
                if (auto* binding = parent->FindAttachmentBinding(AZ::Name("Depth")); binding && binding->GetAttachment())
                {
                    const auto size = binding->GetAttachment()->m_descriptor.m_image.m_size;
                    const float w = static_cast<float>(AZStd::max(size.m_width, 1u));
                    const float h = static_cast<float>(AZStd::max(size.m_height, 1u));
                    set("m_viewport", AZ::Vector4(w, h, 1.0f / w, 1.0f / h));
                }
            }
            const auto& output = GetOutputCount() ? GetOutputBinding(0) : GetInputOutputBinding(0);
            if (output.GetAttachment())
            {
                const auto size = output.GetAttachment()->m_descriptor.m_image.m_size;
                const float w = static_cast<float>(AZStd::max(size.m_width, 1u));
                const float h = static_cast<float>(AZStd::max(size.m_height, 1u));
                set("m_targetSize", AZ::Vector4(w, h, 1.0f / w, 1.0f / h));
            }
        }
        FullscreenTrianglePass::FrameBeginInternal(params);
    }
}
