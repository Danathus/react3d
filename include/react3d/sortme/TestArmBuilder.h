#ifndef TEST_ARM_BUILDER__H
#define TEST_ARM_BUILDER__H

////////////////////////////////////////////////////////////////////////////////

#include <ragdoll/LibraryExport.h>
#include <ragdoll/sortme/DeltaRagdollBuilder.h>

#include <stdlib.h>

typedef struct dxJoint *dJointID;

////////////////////////////////////////////////////////////////////////////////

/**
 * This builder procedurally generates a simple mechanical arm, for testing
 * purposes. It's been left in here because we might some day want to perform
 * these tests again...
 */
class RAGDOLL_LIBRARY_EXPORT TestArmBuilder : public DeltaRagdollBuilder
{
public:
   TestArmBuilder();

   void Configure(size_t numBones); // set the number of bones to build in the arm

   // implementations of pure virtual methods...

   /// Procedurally build a mechanical arm
   void BuildBones(DeltaRagdoll* deltaRagdoll);

   /// Procedurally create ball-and-socket joints segments of mechanical arm
   void BuildJoints(dWorldID worldID, DeltaRagdoll* deltaRagdoll); // call after added to scene

protected:
   void ConfigureJoint(dJointID jointID, dWorldID worldID);

private:
   size_t mNumBones;
};

////////////////////////////////////////////////////////////////////////////////

#endif // TEST_ARM_BUILDER__H
