#ifndef DELTA_RAGDOLL__H
#define DELTA_RAGDOLL__H

////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>

#include <dtCore/refptr.h>

#include <ragdoll/LibraryExport.h>

namespace osg
{
   class Matrixd;
}

namespace dtCore
{
   class Scene;
   class Transformable;
}

namespace dtAnim
{
   class Cal3DModelWrapper;
}

class Ragdoll;
class RagdollBone;

typedef struct dxJointGroup* dJointGroupID;
typedef struct dxJoint*      dJointID;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT DeltaRagdoll
{
public:
   DeltaRagdoll(dtCore::Scene& scene, dtCore::Transformable* actor); //  = false
   ~DeltaRagdoll();

   Ragdoll* GetRagdoll() { return mpRagdoll; }
   const Ragdoll* GetRagdoll() const { return mpRagdoll; }

   dtCore::Scene& GetScene() { return mScene; }
   const dtCore::Scene& GetScene() const { return mScene; }

   // for bone building
   void AddBone(int calBoneID, RagdollBone* bone);
   bool RemoveBone(RagdollBone* bone);
   RagdollBone* GetBoneByCalBoneID(int calBoneID);
   const RagdollBone* GetBoneByCalBoneID(int calBoneID) const;

protected:
   osg::Matrixd CalculateRagdollWorldTransformFromBones() const;

private:
   dtCore::Scene&                                  mScene;
   typedef std::vector<RagdollBone*> BoneList;
   EXPORT_RAGDOLL_STL_MAP(int, RagdollBone*);
   std::map<int, RagdollBone*>                     mCalBoneIDtoRagdollBone;
   Ragdoll*                                        mpRagdoll;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DELTA_RAGDOLL__H
