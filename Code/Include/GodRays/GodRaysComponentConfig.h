/* SPDX-License-Identifier: Apache-2.0 OR MIT */
#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/Math/Color.h>

namespace GodRays
{
    enum class GodRaysResolution : AZ::u32 { Full = 1, Half = 2, Quarter = 4 };

    class GodRaysComponentConfig final : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(GodRaysComponentConfig, "{2D20322C-48FB-4394-933E-52E439D59625}", AZ::ComponentConfig);
        AZ_CLASS_ALLOCATOR(GodRaysComponentConfig, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);
        bool m_enabled = true;
        AZ::Color m_color = AZ::Color(1.0f, 0.95f, 0.85f, 1.0f);
        float m_rayLength = 0.850f;
        float m_falloff = 2.000f;
        float m_sourceRadius = 0.350f;
        float m_sourceSoftness = 0.650f;
        float m_intensity = 0.150f;
        float m_maxBrightness = 0.250f;
        float m_backgroundAmount = 0.000f;
        float m_edgeFade = 0.300f;
        float m_endFadeStart = 0.800f;
        float m_filterStrength = 0.650f;
        AZ::u32 m_samples = 32;
        GodRaysResolution m_resolution = GodRaysResolution::Half;
    };
}
