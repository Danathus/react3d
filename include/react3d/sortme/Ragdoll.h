#ifndef _RAGDOLL_H_
#define _RAGDOLL_H_

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <ragdoll/LibraryExport.h>
#include <ragdoll/sortme/RagdollBone.h>
#include <ragdoll/core/kernel/Skeleton.h>

namespace osg
{
   class Vec3f;
   class Matrixf;
}

namespace dtCore
{
   class Transformable;
};

namespace dtAnim
{
   class Cal3DModelWrapper;
}

// kernel classes
class Skeleton;

class SkeletonBridgeODE;

class RagdollJoint;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT Ragdoll
{
public:
   Ragdoll(dtCore::Transformable* transformable);
   ~Ragdoll();

   void UpdatePhysicsFromAnimation(); // set the physics state according to the current animation state
   void UpdateAnimationFromPhysics();

   void EnableVisualization(bool enable); // defaults to off

   void AddBone(RagdollBone* bone);
   /// Get the number of bones in the ragdoll
   size_t GetNumBones() const;
   /// Get a ragdoll bone by index, in range [ 0, GetNumBones() )
   RagdollBone* GetBoneByIndex(unsigned int index);
   const RagdollBone* GetBoneByIndex(unsigned int index) const;

   void AddJoint(RagdollJoint* joint);
   size_t GetNumJoints() const;
   RagdollJoint* GetJointByIndex(unsigned int index);
   const RagdollJoint* GetJointByIndex(unsigned int index) const;

   Skeleton& GetSkeleton() { return mKernelSekelton; }
   const Skeleton& GetSkeleton() const { return mKernelSekelton; }

   /// Get the transformation matrix that defines the position/orientation of the ragdoll generally in world space
   osg::Matrixf GetRagdollOriginInWorldTransform() const;

   /// Set the transformation matrix that defines the position/orientation of the ragdoll generally in world space
   void SetWorldTransform(const osg::Matrixf& worldTransform);

   SkeletonBridgeODE* GetSkeletonBridgeODE() { return mpSkeletonBridgeODE; }
   const SkeletonBridgeODE* GetSkeletonBridgeODE() const { return mpSkeletonBridgeODE; }

private:
   // kernel data
   Skeleton                  mKernelSekelton;

   dtCore::Transformable*    mpTransformable;
   EXPORT_RAGDOLL_STL_VECTOR(RagdollBone*);
   std::vector<RagdollBone*> mRagdollBones;

   EXPORT_RAGDOLL_STL_VECTOR(RagdollJoint*);
   std::vector<RagdollJoint*> mJoints;

   SkeletonBridgeODE*         mpSkeletonBridgeODE;
};

////////////////////////////////////////////////////////////////////////////////

#endif // _RAGDOLL_H_
