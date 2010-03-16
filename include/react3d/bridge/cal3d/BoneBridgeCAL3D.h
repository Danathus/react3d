#ifndef RAGDOLL_BRIDGE_CAL3D_BONE__H
#define RAGDOLL_BRIDGE_CAL3D_BONE__H

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <list>

#include <ragdoll/LibraryExport.h>
#include <ragdoll/core/math/Matrix.h>

class CalBone;
class CalModel;
class CalBoundingBox;

class Skeleton;
class Bone;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT BoneBridgeCAL3D
{
public:
   BoneBridgeCAL3D(Skeleton& skeleton, CalBone* calBone, CalModel* calModel, int calBoneID);

   const Vec3f& GetDimensions() const { return mDimensions; }
   int GetCalBoneID() const { return mCalBoneID; }
   int GetParentID() const; // -1 if a root

   /// Get the CAL3D bone associated with this skeleton bone
   const CalBone* GetCalBone() const { return mpCalBone; }
   /// Get the CAL3D model associated with this skeleton
   CalModel* GetCalModel() { return mpCalModel; }
   const CalModel* GetCalModel() const { return mpCalModel; }

   /// Get the parent bone in the skeletal hierarchy (matches CAL3D hierarchy)
   Bone* GetParent();
   const Bone* GetParent() const;

   const std::string& GetBoneName() const;
   const std::list<int>& GetListChildBoneID() const;

   /**
    * Calculate the position/orientation of the bone in bone (parent joint-relative) space.
    * This need only be done once, at the end of initialization.
    */
   Matrix4x4f CalculateBoneSpaceTransform() const;

   /// Read the position/orientation of the joint (base of bone) in model (skeleton) space from CAL3D
   Matrix4x4f GetModelSpaceTransform() const;
   void SetModelSpaceTransform(const Matrix4x4f& modelSpaceTransform) const; // in absolute model coords

protected:
   void SetDimensions(const Vec3f& dimensions) { mDimensions = dimensions; }

   /// Tell CAL3D to compute internal bounding box. Call this first!
   void ComputeBoundingBox();                                                         // call first as part of update
   /// Decode CAL3D's representation to deduce bounding box dimensions in our format. Call this next!
   std::vector<Vec3f> ComputeBoundingBoxCorners(const CalBoundingBox& box) const; // call second as part of update
   /// Used to find one extreme corner of the bounding box. Call this after that!
   Vec3f FindMinBoundingBoxXYZ(const std::vector<Vec3f>& corners) const;
   /// Used to find the other extreme corner of the bounding box. Call this last!
   Vec3f FindMaxBoundingBoxXYZ(const std::vector<Vec3f>& corners) const;

private:
   // uplink
   Skeleton& mSkeleton;

   // animation
   CalBone*  mpCalBone;
   CalModel* mpCalModel;

   Vec3f     mDimensions;

   // other info
   int mCalBoneID;
};

////////////////////////////////////////////////////////////////////////////////

#endif //RAGDOLL_BRIDGE_CAL3D_BONE__H
