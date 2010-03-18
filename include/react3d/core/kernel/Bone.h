#ifndef REACT3D_BONE__H
#define REACT3D_BONE__H

////////////////////////////////////////////////////////////////////////////////

#include <react3d/core/LibraryExport.h>

#include <react3d/core/math/Matrix.h>

class Skeleton;

////////////////////////////////////////////////////////////////////////////////

class REACT3D_CORE_LIBRARY_EXPORT Bone
{
public:
   Bone(Skeleton& skeleton, bool hackFlip180);

   const Vec3f& GetDimensions() const { return mDimensions; }
   /// Set the dimensions of the rectangular box used for collision and visualization
   void SetDimensions(const Vec3f& dimensions) { mDimensions = dimensions; }

   Skeleton& GetSkeleton() { return mSkeleton; }
   const Skeleton& GetSkeleton() const { return mSkeleton; }

   /// Set the position/orientation of the joint (the base of this bone) in model (skeleton) space
   void SetModelSpaceTransform(const Matrix4x4f& modelSpaceTransform)
      { mModelSpaceTransform = modelSpaceTransform; }

   void SetBoneSpaceTransform(const Matrix4x4f& boneSpaceTransform)
      { mBoneSpaceTransform = boneSpaceTransform; }

   // first order transforms
   const Matrix4x4f& GetSkeletonOriginInWorldTransform() const;
   /// Get transform that reverses the direction the model is facing; used when models face backwards
   const static Matrix4x4f& GetHackFlip180Transform();
   /// Get the position/rotation of the joint (base of this bone) in model (skeleton) space
   const Matrix4x4f& GetModelSpaceTransform() const { return mModelSpaceTransform; }
   /// Get the position/rotation of the center of this bone in bone space (local to joint to parent)
   const Matrix4x4f& GetBoneSpaceTransform() const  { return mBoneSpaceTransform; }

   // concatenated transforms
   /// Get the position/rotation of the center of this bone in world space
   const Matrix4x4f& GetConcatenatedWorldSpaceAnimationTransform() const { return mConcatenatedWorldSpaceAnimationTransform; }
   /// Get the position/rotation of the joint (the base of this bone) in world space
   const Matrix4x4f& GetConcatenatedWorldSpaceJointTransform() const { return mConcatenatedWorldSpaceJointTransform; } // like above but no bone space transform
   /// Get the position/rotation of the center of this bone in model (skeleton) space
   const Matrix4x4f& GetConcatenatedModelSpaceTransform() const { return mConcatenatedModelSpaceTransform; }      // all but world space, concatenated

   void ComputeConcatenatedTransforms();

   /// Given a world space position/rotation, convert into model (skeleton) space
   Matrix4x4f ConvertWorldSpaceToModelSpace(const Matrix4x4f& worldSpace) const;

protected:
   // calculations /////////////////////
   // second-order transforms (these need recomputing every time the bone moves)
   Matrix4x4f CalculateWorldSpaceAnimationTransform() const;
   Matrix4x4f CalculateWorldSpaceJointTransform() const; // like above but no bone space transform
   Matrix4x4f CalculateModelSpaceTransform() const;      // all but world space, concatenated

private:
   // uplink
   Skeleton&  mSkeleton;

   // physics simulation
   Vec3f      mDimensions;

   // first order transforms
   Matrix4x4f mModelSpaceTransform;  // from model space, find bone root
   Matrix4x4f mBoneSpaceTransform;   // from bone space, find center

   // second order (concatenated) transforms
   Matrix4x4f mConcatenatedWorldSpaceAnimationTransform;
   Matrix4x4f mConcatenatedWorldSpaceJointTransform; // like above but no bone space transform
   Matrix4x4f mConcatenatedModelSpaceTransform;      // all but world space, concatenated

   // hack
   bool mHackFlip180;
};

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_BONE__H
