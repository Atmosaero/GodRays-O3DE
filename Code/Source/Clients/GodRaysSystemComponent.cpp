/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GodRaysSystemComponent.h"

#include <GodRays/GodRaysTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/JSON/document.h>
#include <AzCore/JSON/writer.h>
#include <AzCore/JSON/stringbuffer.h>
#include <Atom/RPI.Public/Pass/PassFilter.h>

#include <Atom/RPI.Public/FeatureProcessorFactory.h>

#include <Render/GodRaysFeatureProcessor.h>
#include <Render/GodRaysPass.h>

namespace GodRays
{
    AZ_COMPONENT_IMPL(GodRaysSystemComponent, "GodRaysSystemComponent",
        GodRaysSystemComponentTypeId);

    void GodRaysSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GodRaysSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }

        GodRaysFeatureProcessor::Reflect(context);
        if (auto* behavior = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behavior->EBus<GodRaysRequestBus>("GodRaysRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ->Attribute(AZ::Script::Attributes::Module, "render")
                ->Event("SetProfilingEnabled", &GodRaysRequests::SetProfilingEnabled)
                ->Event("GetPassDiagnostics", &GodRaysRequests::GetPassDiagnostics);
        }
    }

    void GodRaysSystemComponent::SetProfilingEnabled(bool enabled)
    {
        if (auto* system = AZ::RPI::PassSystemInterface::Get())
        {
            system->ForEachPass(AZ::RPI::PassFilter::CreateWithPassClass<GodRaysPass>(),
                [enabled](AZ::RPI::Pass* pass)
                {
                    pass->SetTimestampQueryEnabled(enabled);
                    return AZ::RPI::PassFilterExecutionFlow::ContinueVisitingPasses;
                });
        }
    }

    AZStd::string GodRaysSystemComponent::GetPassDiagnostics() const
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        writer.StartArray();
        if (auto* system = AZ::RPI::PassSystemInterface::Get())
        {
            system->ForEachPass(AZ::RPI::PassFilter::CreateWithPassClass<GodRaysPass>(),
                [&writer](AZ::RPI::Pass* pass)
                {
                    bool enabled = true;
                    for (auto* ancestor = pass; ancestor; ancestor = ancestor->GetParent())
                    {
                        enabled = enabled && ancestor->IsEnabled();
                    }
                    const auto& binding = pass->GetOutputCount() ? pass->GetOutputBinding(0) : pass->GetInputOutputBinding(0);
                    writer.StartObject();
                    writer.Key("path"); writer.String(pass->GetPathName().GetCStr());
                    writer.Key("enabled"); writer.Bool(enabled);
                    writer.Key("queryEnabled"); writer.Bool(pass->IsTimestampQueryEnabled());
                    writer.Key("gpuNanoseconds");
                    writer.Uint64(enabled && pass->IsTimestampQueryEnabled() ?
                        pass->GetLatestTimestampResult().GetDurationInNanoseconds() : 0);
                    if (binding.GetAttachment())
                    {
                        const auto size = binding.GetAttachment()->m_descriptor.m_image.m_size;
                        writer.Key("width"); writer.Uint(size.m_width);
                        writer.Key("height"); writer.Uint(size.m_height);
                    }
                    writer.EndObject();
                    return AZ::RPI::PassFilterExecutionFlow::ContinueVisitingPasses;
                });
        }
        writer.EndArray();
        return AZStd::string(buffer.GetString(), buffer.GetSize());
    }

    void GodRaysSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GodRaysSystemService"));
    }

    void GodRaysSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GodRaysSystemService"));
    }

    void GodRaysSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("RPISystem"));
    }

    void GodRaysSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    GodRaysSystemComponent::GodRaysSystemComponent()
    {
        if (GodRaysInterface::Get() == nullptr)
        {
            GodRaysInterface::Register(this);
        }
    }

    GodRaysSystemComponent::~GodRaysSystemComponent()
    {
        if (GodRaysInterface::Get() == this)
        {
            GodRaysInterface::Unregister(this);
        }
    }

    void GodRaysSystemComponent::Init()
    {
    }

    void GodRaysSystemComponent::Activate()
    {
        auto* passSystem = AZ::RPI::PassSystemInterface::Get();
        passSystem->AddPassCreator(AZ::Name("GodRaysPass"), &GodRaysPass::Create);
        passSystem->AddPassCreator(AZ::Name("GodRaysParentPass"), &GodRaysParentPass::Create);
        m_loadTemplatesHandler = AZ::RPI::PassSystemInterface::OnReadyLoadTemplatesEvent::Handler([]()
        {
            AZ::RPI::PassSystemInterface::Get()->LoadPassTemplateMappings("Passes/GodRaysPassTemplates.azasset");
        });
        passSystem->ConnectEvent(m_loadTemplatesHandler);
        GodRaysRequestBus::Handler::BusConnect();

        AZ::RPI::FeatureProcessorFactory::Get()->RegisterFeatureProcessor<GodRaysFeatureProcessor>();
    }

    void GodRaysSystemComponent::Deactivate()
    {
        m_loadTemplatesHandler.Disconnect();
        AZ::RPI::FeatureProcessorFactory::Get()->UnregisterFeatureProcessor<GodRaysFeatureProcessor>();

        GodRaysRequestBus::Handler::BusDisconnect();
    }

} // namespace GodRays
