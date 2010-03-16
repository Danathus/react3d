#include <react3d/core/kernel/Bone.h>

#include <cassert>

#include <react3d/core/kernel/Skeleton.h>

////////////////////////////////////////////////////////////////////////////////

Bone::Bone(Skeleton& skeleton, bool hackFlip180)
   : mSkeleton(skeleton)
   , mDimensions(Vec3f::Zero())
   , mModelSpaceTransform(Matrix4x4f::Identity())
   , mBoneSpaceTransform(Matrix4x4f::Identity())
   , mConcatenatedWorldSpaceAnimationTransform(Matrix4x4f::Identity())
   , mConcatenatedWorldSpaceJointTransform(Matrix4x4f::Identity())
   , mConcatenatedModelSpaceTransform(Matrix4x4f::Identity())
   , mHackFlip180(hackFlip180)
{
   // todo
}

const Matrix4x4f& Bone::GetSkeletonOriginInWorldTransform() const
{
   const Matrix4x4f& worldSpace = mSkeleton.GetWorldSpaceTransform();

   // we're done
   return worldSpace;
}

const Matrix4x4f &Bone::GetHackFlip180Transform()
{
   static const Matrix4x4f matrix(
      -1.0f,  0.0f,  0.0f,  0.0f,
       0.0f, -1.0f,  0.0f,  0.0f,
       0.0f,  0.0f,  1.0f,  0.0f,
       0.0f,  0.0f,  0.0f,  1.0f);
   return matrix;
}

void Bone::ComputeConcatenatedTransforms()
{
   // re-compute and cache transforms as appropriate...

   // second-order transforms
   mConcatenatedWorldSpaceAnimationTransform = CalculateWorldSpaceAnimationTransform();
   mConcatenatedWorldSpaceJointTransform     = CalculateWorldSpaceJointTransform();
   mConcatenatedModelSpaceTransform          = CalculateModelSpaceTransform();
}

Matrix4x4f Bone::ConvertWorldSpaceToModelSpace(const Matrix4x4f& worldSpace) const
{
   // let's undo the above, shall we?
   Matrix4x4f boxTransformMatrix = worldSpace;

   // divide out the world space and hack flip 180 matrices from one side
   {
      const Matrix4x4f worldSpaceTransformMatrix     = GetSkeletonOriginInWorldTransform();
      const Matrix4x4f invWorldSpaceTransformMatrix  = worldSpaceTransformMatrix.Inverse();
      boxTransformMatrix *= invWorldSpaceTransformMatrix;

      if (mHackFlip180)
      {
         const Matrix4x4f hackFlip180TransformMatrix    = GetHackFlip180Transform();
         const Matrix4x4f invHackFlip180TransformMatrix = hackFlip180TransformMatrix.Inverse();
         boxTransformMatrix *= invHackFlip180TransformMatrix;
      }
   }

   // divide out the bone space transform from the other side
   {
      const Matrix4x4f boneSpaceTransformMatrix    = GetBoneSpaceTransform();
      const Matrix4x4f invBoneSpaceTransformMatrix = boneSpaceTransformMatrix.Inverse();

      boxTransformMatrix = invBoneSpaceTransformMatrix * boxTransformMatrix;
   }

   return boxTransformMatrix;
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4f Bone::CalculateWorldSpaceAnimationTransform() const
{
   // now multiply them all together and set the box position and orientation
   {
      const Matrix4x4f& worldSpaceTransformMatrix  = GetSkeletonOriginInWorldTransform();
      const Matrix4x4f& modelSpaceTransformMatrix  = GetModelSpaceTransform();
      const Matrix4x4f& boneSpaceTransformMatrix   = GetBoneSpaceTransform();

      // perform multiplications...
      Matrix4x4f boxTransformMatrix = Matrix4x4f::Identity();
      {
         boxTransformMatrix *= boneSpaceTransformMatrix;
         boxTransformMatrix *= modelSpaceTransformMatrix;
         if (mHackFlip180)
         {
            const Matrix4x4f& hackFlip180TransformMatrix = GetHackFlip180Transform();
            boxTransformMatrix *= hackFlip180TransformMatrix;
         }
         boxTransformMatrix *= worldSpaceTransformMatrix;
      }

      return boxTransformMatrix;
   }
}

Matrix4x4f Bone::CalculateWorldSpaceJointTransform() const // like above but no bone space transform
{
   // now multiply them all together and set the box position and orientation
   {
      const Matrix4x4f worldSpaceTransformMatrix  = GetSkeletonOriginInWorldTransform();
      Matrix4x4f modelSpaceTransformMatrix        = GetModelSpaceTransform();

      // perform multiplications...
      Matrix4x4f boxTransformMatrix = Matrix4x4f::Identity();
      {
         boxTransformMatrix *= modelSpaceTransformMatrix;
         if (mHackFlip180)
         {
            const Matrix4x4f hackFlip180TransformMatrix = GetHackFlip180Transform();
            boxTransformMatrix *= hackFlip180TransformMatrix;
         }
         boxTransformMatrix *= worldSpaceTransformMatrix;
      }

      return boxTransformMatrix;
   }
}

Matrix4x4f Bone::CalculateModelSpaceTransform() const
{
   // now multiply them all together and set the box position and orientation
   {
      const Matrix4x4f modelSpaceTransformMatrix  = GetModelSpaceTransform();
      const Matrix4x4f boneSpaceTransformMatrix   = GetBoneSpaceTransform();

      // perform multiplications...
      Matrix4x4f boxTransformMatrix = Matrix4x4f::Identity();
      {
         boxTransformMatrix *= boneSpaceTransformMatrix;
         boxTransformMatrix *= modelSpaceTransformMatrix;
         if (mHackFlip180)
         {
            const Matrix4x4f hackFlip180TransformMatrix = GetHackFlip180Transform();
            boxTransformMatrix *= hackFlip180TransformMatrix;
         }
      }

      return boxTransformMatrix;
   }
}

////////////////////////////////////////////////////////////////////////////////
