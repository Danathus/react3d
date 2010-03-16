#ifndef DEFAULT_BUILDER__H
#define DEFAULT_BUILDER__H

////////////////////////////////////////////////////////////////////////////////

#include <ragdoll/LibraryExport.h>
#include <ragdoll/sortme/DeltaRagdollBuilder.h>

namespace dtAnim
{
   class Cal3DModelWrapper;
}

typedef struct dxJoint *dJointID;

////////////////////////////////////////////////////////////////////////////////

/**
 * This is a simple dumb builder that just creates a boxy bone for every bone
 * found in the skeletal model, and creates a simple ball-and-socket joint
 * between all adjacent bones in the graph.
 */
class RAGDOLL_LIBRARY_EXPORT DefaultBuilder : public DeltaRagdollBuilder
{
public:
   DefaultBuilder();

   void Configure(dtAnim::Cal3DModelWrapper* cal3dWrapper, bool hackFlip180)
   {
      mCal3dWrapper = cal3dWrapper;
      mHackFlip180  = hackFlip180;
   }

   // implementations of pure virtual methods...

   /// Procedurally build a physical bone for every animation bone
   void BuildBones(DeltaRagdoll* deltaRagdoll);

   /// Procedurally create ball-and-socket joints between connected bones
   void BuildJoints(dWorldID worldID, DeltaRagdoll* deltaRagdoll); // call after added to scene

protected:
   void GenerateBonesFromCAL3D(DeltaRagdoll* deltaRagdoll, dtAnim::Cal3DModelWrapper* cal3dWrapper, bool hackFlip180);
   void ConfigureJoint(dJointID jointID, dWorldID worldID);

private:
   dtAnim::Cal3DModelWrapper* mCal3dWrapper;
   bool                       mHackFlip180;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DEFAULT_BUILDER__H
