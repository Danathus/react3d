#include <ragdoll/sortme/RagdollBone.h>

#include <dtCore/object.h>

#include <ragdoll/bridge/osg/Math.h>
#include <ragdoll/bridge/osg/BoneBridgeOSG.h>
#include <ragdoll/bridge/cal3d/BoneBridgeCAL3D.h>
#include <ragdoll/bridge/delta3d/BoneBridgeDelta3D.h>

////////////////////////////////////////////////////////////////////////////////

RagdollBone::RagdollBone(Skeleton& skeleton, bool hackFlip180)
   : mKernelBone(skeleton, hackFlip180)
   , mpBoneBridgeCAL3D(NULL)
   , mpBoneBridgeOSG(NULL)
   , mpBoneBridgeDelta3D(NULL)
{
   // nada
}


RagdollBone::~RagdollBone()
{
   if (mpBoneBridgeOSG)
   {
      delete mpBoneBridgeOSG;
      mpBoneBridgeOSG = NULL;
   }
}


void RagdollBone::Configure()
{
   // now we can create our ODE data
   if (GetBridgeCAL3D())
   {
      // create box first
      GetKernelBone().SetDimensions(GetBridgeCAL3D()->GetDimensions());

      // set up world space geometry transform

      // set up model space geometry transform

      // set up bone space geometry transform
      GetKernelBone().SetBoneSpaceTransform(GetBridgeCAL3D()->CalculateBoneSpaceTransform());
   }

   // initialize cache...
   {
      if (GetBridgeCAL3D())
      {
         GetKernelBone().SetModelSpaceTransform(GetBridgeCAL3D()->GetModelSpaceTransform());
      }

      // re-compute and cache transforms as appropriate...
      GetKernelBone().ComputeConcatenatedTransforms();
   }
}


Vec3f RagdollBone::CalculateSocketPosition() const
{
   const Matrix4x4f& matrix = GetKernelBone().GetConcatenatedWorldSpaceJointTransform();

   Vec3f socketPos = matrix.GetTranslation();

   return socketPos;
}


Matrix4x4f RagdollBone::GetTransformOfBoneCenterInWorld() const
{
   return GetBridgeDelta3D()->GetWorldTransform();
}


const Matrix4x4f& RagdollBone::GetModelSpaceCenterTransform() const
{
   return GetKernelBone().GetModelSpaceTransform();
}


void RagdollBone::SetBridgeOSG(BoneBridgeOSG* boneBridgeOSG)
{
   mpBoneBridgeOSG = boneBridgeOSG;

   if (mpBoneBridgeDelta3D && mpBoneBridgeOSG)
   {
      mpBoneBridgeDelta3D->GetObject()->GetMatrixTransform().addChild(mpBoneBridgeOSG->GetRootOSG());
   }
}


void RagdollBone::SetBridgeDelta3D(BoneBridgeDelta3D* boneBridgeDelta3D)
{
   mpBoneBridgeDelta3D = boneBridgeDelta3D;

   if (mpBoneBridgeDelta3D && mpBoneBridgeOSG)
   {
      mpBoneBridgeDelta3D->GetObject()->GetMatrixTransform().addChild(mpBoneBridgeOSG->GetRootOSG());
   }
}


void RagdollBone::UpdatePhysicsFromAnimation()
{
   // model space transform update CAL3D -> Internal
   {
      if (GetBridgeCAL3D())
      {
         GetKernelBone().SetModelSpaceTransform(GetBridgeCAL3D()->GetModelSpaceTransform());
      }

      // re-compute and cache transforms as appropriate...
      GetKernelBone().ComputeConcatenatedTransforms();
   }

   GetBridgeDelta3D()->SetWorldTransform(
      GetKernelBone().GetConcatenatedWorldSpaceAnimationTransform());
}


void RagdollBone::UpdateAnimationFromPhysics()
{
   Matrix4x4f matrix = GetBridgeDelta3D()->GetWorldTransform();

   // now set that transform -- go!
   {
      const Matrix4x4f boxTransformMatrix = GetKernelBone().ConvertWorldSpaceToModelSpace(matrix);

      // what's left should be in model space -- lets place it here officially
      GetKernelBone().SetModelSpaceTransform(boxTransformMatrix);

      // now update the cal3d side from this new model space information
      if (GetBridgeCAL3D())
      {
         GetBridgeCAL3D()->SetModelSpaceTransform(boxTransformMatrix);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
