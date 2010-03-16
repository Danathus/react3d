#include <ragdoll/core/kernel/Skeleton.h>

////////////////////////////////////////////////////////////////////////////////

Skeleton::Skeleton()
   : mWorldSpaceTransform(Matrix4x4f::Identity())
{
}


void Skeleton::AddBone(Bone* bone)
{
   mBones.push_back(bone);
}


size_t Skeleton::GetNumBones() const
{
   return mBones.size();
}


Bone* Skeleton::GetBoneByIndex(unsigned int index)
{
   Bone* bone = (index >= 0 && index < GetNumBones()) ? mBones[index] : NULL;
   return bone;
}


const Bone* Skeleton::GetBoneByIndex(unsigned int index) const
{
   const Bone* bone = (index >= 0 && index < GetNumBones()) ? mBones[index] : NULL;
   return bone;
}

////////////////////////////////////////////////////////////////////////////////
