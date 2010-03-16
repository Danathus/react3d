#ifndef RAGDOLL_SKELETON__H
#define RAGDOLL_SKELETON__H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <react3d/LibraryExport.h>

#include <react3d/core/math/Matrix.h>

class Bone;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT Skeleton
{
public:
   Skeleton();

   void AddBone(Bone* bone);
   /// Get the number of bones in the skeleton
   size_t GetNumBones() const;
   /// Get a skeleton bone by index, in range [ 0, GetNumBones() )
   Bone* GetBoneByIndex(unsigned int index);
   const Bone* GetBoneByIndex(unsigned int index) const;

   /// Set the transformation matrix that defines the position/orientation of the ragdoll generally in world space
   void SetWorldSpaceTransform(const Matrix4x4f& worldSpaceTransform) { mWorldSpaceTransform = worldSpaceTransform; }

   /// Get the transformation matrix that defines the position/orientation of the skeleton generally in world space
   const Matrix4x4f& GetWorldSpaceTransform() const { return mWorldSpaceTransform; }

protected:
private:
   Matrix4x4f mWorldSpaceTransform;  // from world space, find skeleton root

   EXPORT_RAGDOLL_STL_VECTOR(Bone*);
   std::vector<Bone*> mBones;
};

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_SKELETON__H
