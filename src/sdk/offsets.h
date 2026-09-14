#pragma once
#include <cstdint>

// Live offsets (current client build - update via RbxDumperV2)
// Source: user-provided dump + RbxDumperV2 structure

namespace Offsets {

// === Pointers / Bases ===
inline uintptr_t TaskScheduler_Pointer     = 0x8b5cee8;
inline uintptr_t VisualEngine_Pointer      = 0x83f0a08;
inline uintptr_t FakeDataModel_Pointer     = 0x8dc2258;

// === TaskScheduler ===
constexpr uintptr_t TaskScheduler_JobStart = 0xc8;
constexpr uintptr_t TaskScheduler_JobEnd   = 0xd0;
constexpr uintptr_t TaskScheduler_JobName  = 0x18;
constexpr uintptr_t TaskScheduler_MaxFPS   = 0xb0;

// === FakeDataModel / DataModel ===
constexpr uintptr_t FakeDataModel_RealDataModel = 0x1f8;
constexpr uintptr_t DataModel_PlaceId           = 0x190;
constexpr uintptr_t DataModel_GameId            = 0x188;
constexpr uintptr_t DataModel_CreatorId         = 0x180;
constexpr uintptr_t DataModel_GameLoaded        = 0x5d8;
constexpr uintptr_t DataModel_JobId             = 0x118;
constexpr uintptr_t DataModel_Workspace         = 0x158;
constexpr uintptr_t DataModel_ScriptContext     = 0x440;
constexpr uintptr_t DataModel_PlaceVersion      = 0x1ac;
constexpr uintptr_t DataModel_ServerIP          = 0x5c0;
constexpr uintptr_t DataModel_PrimitiveCount    = 0x420;
constexpr uintptr_t DataModel_ToRenderView1     = 0x1c8;
constexpr uintptr_t DataModel_ToRenderView2     = 0x8;
constexpr uintptr_t DataModel_ToRenderView3     = 0x28;

// === VisualEngine ===
constexpr uintptr_t VisualEngine_Dimensions    = 0xb10;
constexpr uintptr_t VisualEngine_ViewMatrix    = 0x1b0;
constexpr uintptr_t VisualEngine_RenderView    = 0xc30;
constexpr uintptr_t VisualEngine_FakeDataModel = 0xaf0;

// === RenderJob ===
constexpr uintptr_t RenderJob_RenderView       = 0x1d8;
constexpr uintptr_t RenderJob_FakeDataModel    = 0x38;
constexpr uintptr_t RenderJob_RealDataModel    = 0x1f0;

// === Instance ===
constexpr uintptr_t Instance_This              = 0x8;
constexpr uintptr_t Instance_NameContainer     = 0x70;
constexpr uintptr_t Instance_Name              = 0x8;
constexpr uintptr_t Instance_ChildrenStart     = 0x78;
constexpr uintptr_t Instance_ChildrenEnd       = 0x8;
constexpr uintptr_t Instance_Parent            = 0x68;
constexpr uintptr_t Instance_ClassDescriptor   = 0x18;
constexpr uintptr_t Instance_ClassName         = 0x8;
constexpr uintptr_t Instance_ClassBase         = 0x1b0;

// === Misc ===
constexpr uintptr_t Misc_StringLength          = 0x10;
constexpr uintptr_t Misc_Adornee               = 0xf0;
constexpr uintptr_t Misc_Value                 = 0xb8;
constexpr uintptr_t Misc_AnimationId           = 0xc0;

// === Player ===
constexpr uintptr_t Player_LocalPlayer         = 0x130;
constexpr uintptr_t Player_UserId              = 0xd0;
constexpr uintptr_t Player_DisplayName         = 0x138;
constexpr uintptr_t Player_HealthDisplayDistance = 0x394;
constexpr uintptr_t Player_NameDisplayDistance = 0x3a4;
constexpr uintptr_t Player_ModelInstance       = 0x298;
constexpr uintptr_t Player_Team                = 0x2d8;
constexpr uintptr_t Player_TeamColor           = 0x3b0;
constexpr uintptr_t Player_LocaleId            = 0x118;
constexpr uintptr_t Player_AccountAge          = 0x35c;
constexpr uintptr_t Player_MinZoomDistance     = 0x36c;
constexpr uintptr_t Player_MaxZoomDistance     = 0x368;
constexpr uintptr_t Player_CameraMode          = 0x370;
constexpr uintptr_t Player_Mouse               = 0x1208;

// === Team ===
constexpr uintptr_t Team_BrickColor            = 0xb8;

// === Humanoid ===
constexpr uintptr_t Humanoid_Health            = 0x190;
constexpr uintptr_t Humanoid_MaxHealth         = 0x1a8;
constexpr uintptr_t Humanoid_Walkspeed         = 0x1d0;
constexpr uintptr_t Humanoid_WalkspeedCheck    = 0x3b4;
constexpr uintptr_t Humanoid_JumpPower         = 0x1a4;
constexpr uintptr_t Humanoid_JumpHeight        = 0x1a0;
constexpr uintptr_t Humanoid_HipHeight         = 0x194;
constexpr uintptr_t Humanoid_MaxSlopeAngle     = 0x1ac;
constexpr uintptr_t Humanoid_SeatPart          = 0x108;
constexpr uintptr_t Humanoid_HumanoidRootPart  = 0x470;
constexpr uintptr_t Humanoid_CameraOffset      = 0x128;
constexpr uintptr_t Humanoid_HealthDisplayDistance = 0x188;
constexpr uintptr_t Humanoid_NameDisplayDistance = 0x1b0;
constexpr uintptr_t Humanoid_DisplayDistanceType = 0x180;
constexpr uintptr_t Humanoid_HealthDisplayType = 0x18c;
constexpr uintptr_t Humanoid_NameOcclusion     = 0x1b4;
constexpr uintptr_t Humanoid_DisplayName       = 0xb8;
constexpr uintptr_t Humanoid_MoveDirection     = 0x140;
constexpr uintptr_t Humanoid_RigType           = 0x1c0;
constexpr uintptr_t Humanoid_Jump              = 0x1da;
constexpr uintptr_t Humanoid_Sit               = 0x1dd;
constexpr uintptr_t Humanoid_PlatformStand     = 0x1dc;
constexpr uintptr_t Humanoid_UseJumpPower      = 0x1e0;
constexpr uintptr_t Humanoid_AutomaticScalingEnabled = 0x1d6;
constexpr uintptr_t Humanoid_BreakJointsOnDeath = 0x1d7;
constexpr uintptr_t Humanoid_EvaluateStateMachine = 0x1d8;
constexpr uintptr_t Humanoid_RequiresNeck      = 0x1dd;
constexpr uintptr_t Humanoid_AutoJumpEnabled   = 0x1d4;
constexpr uintptr_t Humanoid_AutoRotate        = 0x1d5;
constexpr uintptr_t Humanoid_IsWalking         = 0x95f;
constexpr uintptr_t Humanoid_MoveToPoint       = 0x164;
constexpr uintptr_t Humanoid_MoveToPart        = 0x118;
constexpr uintptr_t Humanoid_WalkTimer         = 0x400;
constexpr uintptr_t Humanoid_HumanoidState     = 0x8b8;
constexpr uintptr_t Humanoid_HumanoidStateID   = 0x20;
constexpr uintptr_t Humanoid_FloorMaterial     = 0x184;
constexpr uintptr_t Humanoid_TargetPoint       = 0x14c;

// === Workspace / World ===
constexpr uintptr_t Workspace_World            = 0x410;
constexpr uintptr_t Workspace_CurrentCamera    = 0x4b8;
constexpr uintptr_t Workspace_ReadOnlyGravity  = 0xa00;
constexpr uintptr_t Workspace_DistributedGameTime = 0x4d8;
constexpr uintptr_t World_Gravity              = 0x22c;
constexpr uintptr_t World_worldStepsPerSec     = 0x728;
constexpr uintptr_t World_FallenPartsDestroyHeight = 0x220;
constexpr uintptr_t World_AirProperties        = 0x240;
constexpr uintptr_t World_Primitives           = 0x2b0;

// === Camera ===
constexpr uintptr_t Camera_Position            = 0xfc;
constexpr uintptr_t Camera_Rotation            = 0xd8;
constexpr uintptr_t Camera_CameraSubject       = 0xc8;
constexpr uintptr_t Camera_FieldOfView         = 0x140;
constexpr uintptr_t Camera_ImagePlaneDepth     = 0x2d4;
constexpr uintptr_t Camera_CameraType          = 0x138;
constexpr uintptr_t Camera_Viewport            = 0x28c;
constexpr uintptr_t Camera_ViewportSize        = 0x2cc;

// === BasePart / Primitive ===
constexpr uintptr_t BasePart_Primitive         = 0x188;
constexpr uintptr_t BasePart_Transparency      = 0x130;
constexpr uintptr_t BasePart_Color3            = 0x1a8;
constexpr uintptr_t BasePart_Shape             = 0x1b8;
constexpr uintptr_t BasePart_Massless          = 0x137;
constexpr uintptr_t BasePart_CastShadow        = 0x135;
constexpr uintptr_t BasePart_Locked            = 0x136;
constexpr uintptr_t BasePart_Reflectance       = 0x10c;
constexpr uintptr_t Primitive_Position         = 0xec;
constexpr uintptr_t Primitive_Validate         = 0x6;
constexpr uintptr_t Primitive_Owner            = 0x210;
constexpr uintptr_t Primitive_Size             = 0x1bc;
constexpr uintptr_t Primitive_Rotation         = 0xc8;
constexpr uintptr_t Primitive_Flags            = 0x1b6;
constexpr uintptr_t Primitive_Material         = 0x0;
constexpr uintptr_t Primitive_AssemblyLinearVelocity = 0xf8;
constexpr uintptr_t Primitive_AssemblyAngularVelocity = 0x104;

// Primitive Flags bits
constexpr uintptr_t PrimitiveFlags_Anchored    = 0x2;
constexpr uintptr_t PrimitiveFlags_CanCollide  = 0x8;
constexpr uintptr_t PrimitiveFlags_CanTouch    = 0x10;
constexpr uintptr_t PrimitiveFlags_CanQuery    = 0x20;

// === Model ===
constexpr uintptr_t Model_PrimaryPart          = 0x258;
constexpr uintptr_t Model_Scale                = 0x144;

// === Tool ===
constexpr uintptr_t Tool_Tooltip               = 0x468;
constexpr uintptr_t Tool_TextureId             = 0x360;
constexpr uintptr_t Tool_Grip                  = 0x4ac;
constexpr uintptr_t Tool_Enabled               = 0x4b9;
constexpr uintptr_t Tool_CanBeDropped          = 0x4b8;
constexpr uintptr_t Tool_ManualActivationOnly  = 0x4ba;
constexpr uintptr_t Tool_RequiresHandle        = 0x4bb;

// === Mouse / Input ===
constexpr uintptr_t MouseService_InputObject   = 0xf0;
constexpr uintptr_t MouseService_InputObject2  = 0x100;
constexpr uintptr_t MouseService_MousePosition = 0xd4;
constexpr uintptr_t PlayerMouse_Workspace      = 0x150;
constexpr uintptr_t PlayerMouse_Icon           = 0xc8;

// === RunService ===
constexpr uintptr_t RunService_HeartbeatTask   = 0xe0;
constexpr uintptr_t RunService_HeartbeatFPS    = 0xc0;

} // namespace Offsets
