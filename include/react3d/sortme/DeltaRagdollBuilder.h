#ifndef DELTA_RAGDOLL_BUILDER__H
#define DELTA_RAGDOLL_BUILDER__H

////////////////////////////////////////////////////////////////////////////////

#include <ragdoll/LibraryExport.h>

typedef struct dxWorld* dWorldID;

class DeltaRagdoll;

////////////////////////////////////////////////////////////////////////////////

/**
 * This is an abstract base class for an object that can be used to generate
 * bones and joints for an appropriately configured DeltaRagdoll.
 */
class RAGDOLL_LIBRARY_EXPORT DeltaRagdollBuilder
{
public:
   DeltaRagdollBuilder();

   /**
    * Build bones for an empty (but configured) DeltaRagdoll.
    * @param deltaRagdoll The DeltaRagdoll to build bones for
    */
   virtual void BuildBones(DeltaRagdoll* deltaRagdoll) = 0;

   /**
    * Build joints for a ragdoll whose bones have been built.
    * @note This should only be called after the bones have been built and
    * individually added to the scene.
    * @param worldID The ID for the World the ragdoll exists within
    * @param deltaRagdoll The DeltaRagdoll to build joints for
    */
   virtual void BuildJoints(dWorldID worldID, DeltaRagdoll* deltaRagdoll) = 0; // call after added to scene

private:
};

////////////////////////////////////////////////////////////////////////////////

#endif // DELTA_RAGDOLL_BUILDER__H
