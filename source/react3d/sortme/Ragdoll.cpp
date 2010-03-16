#include <ragdoll/sortme/Ragdoll.h>

#include <dtAnim/animationgameactor.h>
#include <dtAnim/cal3dmodelwrapper.h>
#include <cal3d/skeleton.h>

#include <osg/Geode>

#include <dtCore/scene.h>
#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>

#include <ragdoll/bridge/cal3d/BoneBridgeCAL3D.h>
#include <ragdoll/bridge/osg/BoneBridgeOSG.h>
#include <ragdoll/bridge/ode/SkeletonBridgeODE.h>
#include <ragdoll/sortme/RagdollJoint.h>

#define RESET_WORLD_TRANSFORM 0 // 0->1

////////////////////////////////////////////////////////////////////////////////

Ragdoll::Ragdoll(dtCore::Transformable* transformable)
   : mpTransformable(transformable)
   , mpSkeletonBridgeODE(NULL)
{
   assert(mpTransformable != NULL);

   // lazily create joint group
   mpSkeletonBridgeODE = new SkeletonBridgeODE();
}


Ragdoll::~Ragdoll()
{
   // destroy bones
   for (size_t i = 0; i < mRagdollBones.size(); ++i)
   {
      delete mRagdollBones[i];
   }
   mRagdollBones.clear();

   // destroy joints
   for (size_t i = 0; i < mJoints.size(); ++i)
   {
      delete mJoints[i];
   }
   mJoints.clear();

   // destroy components
   if (mpSkeletonBridgeODE)
   {
      delete mpSkeletonBridgeODE;
      mpSkeletonBridgeODE = NULL;
   }
}


void Ragdoll::UpdatePhysicsFromAnimation()
{
   for (size_t i = 0; i < mRagdollBones.size(); ++i)
   {
      RagdollBone* bone = GetBoneByIndex(i);
      bone->UpdatePhysicsFromAnimation();
   }
}


void Ragdoll::UpdateAnimationFromPhysics()
{
#if RESET_WORLD_TRANSFORM
   //* ...trying it first... (seems to have an effect, but not quite correct)
   //  (hrm, now this seems to fly away...)
   // now find a position and orientation for the body at large
   {
      // find position and orientation of the body at large by finding position
      //   and orientation of a bone at the root, and subtracting out its offset
      //   in model space
      const osg::Matrix bodyWorldTransform = CalculateRagdollWorldTransformFromBones();

      // now set it for the rag doll at large
      mpRagdoll->SetWorldTransform(bodyWorldTransform);
   }
   //*/
#endif

   for (size_t i = 0; i < mRagdollBones.size(); ++i)
   {
      RagdollBone* bone = GetBoneByIndex(i);
      bone->UpdateAnimationFromPhysics();
   }
   // now call CalSkeleton::calculateState to finish bone settings
   if (GetNumBones() > 0 && mRagdollBones[0]->GetBridgeCAL3D())
   {
      mRagdollBones[0]->GetBridgeCAL3D()->GetCalModel()->getSkeleton()->calculateState();
   }

   /* (doing second seems to have no effect)
   // now find a position and orientation for the body at large
   {
      // find position and orientation of the body at large by finding position
      //   and orientation of a bone at the root, and subtracting out its offset
      //   in model space
      const osg::Matrix bodyWorldTransform = CalculateRagdollWorldTransformFromBones();

      // now set it for the rag doll at large
      mpRagdoll->SetWorldTransform(bodyWorldTransform);
   }
   //*/
}


void Ragdoll::EnableVisualization(bool enable)
{
   for (std::vector<RagdollBone*>::size_type i = 0; i < mRagdollBones.size(); ++i)
   {
      mRagdollBones[i]->GetBridgeOSG()->EnableVisualization(enable);
   }
}


void Ragdoll::AddBone(RagdollBone* bone)
{
   mRagdollBones.push_back(bone);

   mKernelSekelton.AddBone(&bone->GetKernelBone());
}


size_t Ragdoll::GetNumBones() const
{
   return mRagdollBones.size();
}


RagdollBone* Ragdoll::GetBoneByIndex(unsigned int index)
{
   RagdollBone* bone = (index >= 0 && index < GetNumBones()) ? mRagdollBones[index] : NULL;
   return bone;
}


const RagdollBone* Ragdoll::GetBoneByIndex(unsigned int index) const
{
   const RagdollBone* bone = (index >= 0 && index < GetNumBones()) ? mRagdollBones[index] : NULL;
   return bone;
}


void Ragdoll::AddJoint(RagdollJoint* joint)
{
   mJoints.push_back(joint);
}


size_t Ragdoll::GetNumJoints() const
{
   return mJoints.size();
}


RagdollJoint* Ragdoll::GetJointByIndex(unsigned int index)
{
   RagdollJoint* joint = (index >= 0 && index < GetNumJoints()) ? mJoints[index] : NULL;
   return joint;
}


const RagdollJoint* Ragdoll::GetJointByIndex(unsigned int index) const
{
   const RagdollJoint* joint = (index >= 0 && index < GetNumJoints()) ? mJoints[index] : NULL;
   return joint;
}


osg::Matrixf Ragdoll::GetRagdollOriginInWorldTransform() const
{
   osg::Matrix osgMatrix;
   {
      dtCore::Transform xform;
      mpTransformable->GetTransform(xform);
      xform.Get(osgMatrix);
   }

   return osgMatrix;
}


void Ragdoll::SetWorldTransform(const osg::Matrixf& worldTransform)
{
   dtCore::Transform transform;
   transform.Set(worldTransform);
   mpTransformable->SetTransform(transform);
}

////////////////////////////////////////////////////////////////////////////////
