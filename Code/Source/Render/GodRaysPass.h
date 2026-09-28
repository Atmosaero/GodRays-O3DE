/* SPDX-License-Identifier: Apache-2.0 OR MIT */
#pragma once
#include <Atom/RPI.Public/Pass/FullscreenTrianglePass.h>
#include <Atom/RPI.Public/Pass/ParentPass.h>

namespace GodRays
{
    class GodRaysParentPass final : public AZ::RPI::ParentPass
    {
        AZ_RPI_PASS(GodRaysParentPass);
    public:
        AZ_RTTI(GodRaysParentPass, "{A0B8D691-370F-4B1F-B23D-ACD99E02808C}", AZ::RPI::ParentPass);
        AZ_CLASS_ALLOCATOR(GodRaysParentPass, AZ::SystemAllocator);
        static AZ::RPI::Ptr<GodRaysParentPass> Create(const AZ::RPI::PassDescriptor& descriptor);
        bool IsEnabled() const override;
    private:
        explicit GodRaysParentPass(const AZ::RPI::PassDescriptor& descriptor);
        void FrameBeginInternal(FramePrepareParams params) override;
    };

    class GodRaysPass final : public AZ::RPI::FullscreenTrianglePass
    {
        AZ_RPI_PASS(GodRaysPass);
    public:
        AZ_RTTI(GodRaysPass, "{A25C16CC-7C31-4A4D-B7BD-DC174BD728CF}", AZ::RPI::FullscreenTrianglePass);
        AZ_CLASS_ALLOCATOR(GodRaysPass, AZ::SystemAllocator);
        static AZ::RPI::Ptr<GodRaysPass> Create(const AZ::RPI::PassDescriptor& descriptor);
        void SetResolutionScale(float scale);
    private:
        explicit GodRaysPass(const AZ::RPI::PassDescriptor& descriptor);
        void FrameBeginInternal(FramePrepareParams params) override;
    };
}
