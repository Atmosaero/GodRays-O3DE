/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */
#include <Components/GodRaysComponentController.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <Atom/RPI.Public/Scene.h>

namespace GodRays
{
    void GodRaysComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<GodRaysComponentConfig, AZ::ComponentConfig>()
                ->Version(4)
                ->Field("Enabled", &GodRaysComponentConfig::m_enabled)
                ->Field("Color", &GodRaysComponentConfig::m_color)
                ->Field("RadialDensity", &GodRaysComponentConfig::m_rayLength)
                ->Field("Falloff", &GodRaysComponentConfig::m_falloff)
                ->Field("SourceRadius", &GodRaysComponentConfig::m_sourceRadius)
                ->Field("SourceSoftness", &GodRaysComponentConfig::m_sourceSoftness)
                ->Field("Intensity", &GodRaysComponentConfig::m_intensity)
                ->Field("MaxBrightness", &GodRaysComponentConfig::m_maxBrightness)
                ->Field("BackgroundAmount", &GodRaysComponentConfig::m_backgroundAmount)
                ->Field("EdgeFade", &GodRaysComponentConfig::m_edgeFade)
                ->Field("EndFadeStart", &GodRaysComponentConfig::m_endFadeStart)
                ->Field("FilterStrength", &GodRaysComponentConfig::m_filterStrength)
                ->Field("Samples", &GodRaysComponentConfig::m_samples)
                ->Field("Resolution", &GodRaysComponentConfig::m_resolution);
            if (auto* ec = sc->GetEditContext())
            {
                ec->Class<GodRaysComponentConfig>("GodRays", "Screen-space light shafts with a localized source and independent quality controls.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::CheckBox, &GodRaysComponentConfig::m_enabled, "Enabled", "Enable this component. r_godRays is the global switch.")
                    ->DataElement(AZ::Edit::UIHandlers::Color, &GodRaysComponentConfig::m_color, "Color", "Linear ray tint.")
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_rayLength, "Ray length", "Fraction of the screen-space path towards the source.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 1.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_falloff, "Falloff", "Attenuation along the ray. Independent of sample count.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 8.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_sourceRadius, "Source radius", "Radius of the source mask in screen-height units.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.020f)->Attribute(AZ::Edit::Attributes::Max, 1.500f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_sourceSoftness, "Source softness", "Soft edge as a fraction of the source radius.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.010f)->Attribute(AZ::Edit::Attributes::Max, 1.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_intensity, "Intensity", "Linear HDR ray intensity, independent of quality.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 4.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_maxBrightness, "Max brightness", "Maximum added RGB channel. Preserves ray hue.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 4.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_backgroundAmount, "Background amount", "0 accents partial occlusion only; 1 includes the full source glow.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 1.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_edgeFade, "Screen edge fade", "Offscreen distance over which the effect fades, in screen heights. Behind-camera sources always fade out.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.010f)->Attribute(AZ::Edit::Attributes::Max, 1.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_endFadeStart, "End fade start", "Normalized point where integration fades towards the end of the ray.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 0.990f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_filterStrength, "Filter strength", "Depth-aware spatial filtering strength in the reduced ray buffer.")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.000f)->Attribute(AZ::Edit::Attributes::Max, 1.000f)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &GodRaysComponentConfig::m_samples, "Samples", "Quality only: normalized integration, 8 to 96 samples.")
                        ->Attribute(AZ::Edit::Attributes::Min, 8)->Attribute(AZ::Edit::Attributes::Max, 96)
                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &GodRaysComponentConfig::m_resolution, "Resolution", "Resolution of the mask, integration and filter buffers. Composite remains full resolution.")
                        ->EnumAttribute(GodRaysResolution::Full, "Full")
                        ->EnumAttribute(GodRaysResolution::Half, "Half")
                        ->EnumAttribute(GodRaysResolution::Quarter, "Quarter");
            }
        }
    }

    void GodRaysComponentController::Reflect(AZ::ReflectContext* context)
    {
        GodRaysComponentConfig::Reflect(context);
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<GodRaysComponentController>()->Version(1)
                ->Field("Configuration", &GodRaysComponentController::m_configuration);
            if (auto* ec = sc->GetEditContext())
            {
                ec->Class<GodRaysComponentController>("GodRays controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &GodRaysComponentController::m_configuration, "Configuration", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }
    }

    void GodRaysComponentController::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType&) {}
    void GodRaysComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("GodRaysService"));
    }
    void GodRaysComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("GodRaysService"));
    }
    void GodRaysComponentController::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("TransformService"));
        services.push_back(AZ_CRC_CE("DirectionalLightService"));
    }

    GodRaysComponentController::GodRaysComponentController(const GodRaysComponentConfig& config) : m_configuration(config) {}

    void GodRaysComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;
        m_featureProcessor = AZ::RPI::Scene::GetFeatureProcessorForEntity<GodRaysFeatureProcessorInterface>(entityId);
        if (!m_featureProcessor)
        {
            AZ_Warning("GodRays", false, "No GodRays feature processor for entity %s.", entityId.ToString().c_str());
            return;
        }
        m_handle = m_featureProcessor->AcquireGodRays();
        m_handle->m_configuration = m_configuration;
        AZ::Transform world = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(world, entityId, &AZ::TransformBus::Events::GetWorldTM);
        OnTransformChanged(world, world);
        AZ::TransformNotificationBus::Handler::BusConnect(entityId);
    }

    void GodRaysComponentController::Deactivate()
    {
        AZ::TransformNotificationBus::Handler::BusDisconnect();
        if (m_featureProcessor && m_handle)
        {
            m_featureProcessor->ReleaseGodRays(m_handle);
        }
        m_featureProcessor = nullptr;
        m_entityId.SetInvalid();
    }

    void GodRaysComponentController::SetConfiguration(const GodRaysComponentConfig& config)
    {
        m_configuration = config;
        if (m_handle)
        {
            m_handle->m_configuration = config;
        }
    }
    const GodRaysComponentConfig& GodRaysComponentController::GetConfiguration() const { return m_configuration; }

    void GodRaysComponentController::OnTransformChanged(const AZ::Transform&, const AZ::Transform& world)
    {
        if (m_handle)
        {
            const auto direction = world.GetBasisY();
            m_handle->m_direction = direction.IsFinite() && direction.GetLengthSq() > 0.0001f
                ? direction.GetNormalized() : AZ::Vector3::CreateAxisY();
        }
    }

}
