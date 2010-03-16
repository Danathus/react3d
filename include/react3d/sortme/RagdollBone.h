#ifndef _RAGDOLLBONE_H_
#define _RAGDOLLBONE_H_

////////////////////////////////////////////////////////////////////////////////

#include <ragdoll/core/kernel/Bone.h>

// cal3d types
class CalBone;
class CalModel;

// library types
class BoneBridgeCAL3D;
class BoneBridgeOSG;
class BoneBridgeDelta3D;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT RagdollBone
{
public:
   RagdollBone(Skeleton& skeleton, bool hackFlip180);
   ~RagdollBone();

   void Configure(); // call to initialize, post-assembly

   /// Get the position of this bone's socket in world space
   Vec3f CalculateSocketPosition() const;

   /// Get the pos/rot of the center of this bone, in world space
   Matrix4x4f GetTransformOfBoneCenterInWorld() const;
   /// Get the pos/rot of the center of this bone, in model (ragdoll) space
   const Matrix4x4f& GetModelSpaceCenterTransform() const; // all but world space, concatenated

   // queries //////////////////////////
   Bone& GetKernelBone() { return mKernelBone; }
   const Bone& GetKernelBone() const { return mKernelBone; }

   void SetBridgeCAL3D(BoneBridgeCAL3D* boneBridgeCAL3D) { mpBoneBridgeCAL3D = boneBridgeCAL3D; }
   BoneBridgeCAL3D* GetBridgeCAL3D() { return mpBoneBridgeCAL3D; }
   const BoneBridgeCAL3D* GetBridgeCAL3D() const { return mpBoneBridgeCAL3D; }

   void SetBridgeOSG(BoneBridgeOSG* boneBridgeOSG);
   BoneBridgeOSG* GetBridgeOSG() { return mpBoneBridgeOSG; }
   const BoneBridgeOSG* GetBridgeOSG() const { return mpBoneBridgeOSG; }

   void SetBridgeDelta3D(BoneBridgeDelta3D* boneBridgeDelta3D);
   BoneBridgeDelta3D* GetBridgeDelta3D() { return mpBoneBridgeDelta3D; }
   const BoneBridgeDelta3D* GetBridgeDelta3D() const { return mpBoneBridgeDelta3D; }

   // data mixing
   /// Read CAL3D position/orientation; write to this dtCore::Object
   void UpdatePhysicsFromAnimation();
   /// Read position/orientation from this dtCore::Object; write to CAL3D
   void UpdateAnimationFromPhysics();

protected:
private:
   // kernel data
   Bone               mKernelBone;
   BoneBridgeCAL3D*   mpBoneBridgeCAL3D;
   BoneBridgeOSG*     mpBoneBridgeOSG;
   BoneBridgeDelta3D* mpBoneBridgeDelta3D;
};

////////////////////////////////////////////////////////////////////////////////

#endif // _RAGDOLLBONE_H_
