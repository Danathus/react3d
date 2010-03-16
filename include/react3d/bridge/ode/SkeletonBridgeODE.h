#ifndef RAGDOLL_BRIDGE_ODE_SKELETON__H
#define RAGDOLL_BRIDGE_ODE_SKELETON__H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <ragdoll/LibraryExport.h>

typedef struct dxJointGroup* dJointGroupID;
typedef struct dxJoint*      dJointID;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT SkeletonBridgeODE
{
public:
   SkeletonBridgeODE();
   ~SkeletonBridgeODE();

   dJointGroupID GetJointGroupID() { return mJointGroupID; }
   const dJointGroupID GetJointGroupID() const { return mJointGroupID; }

protected:
private:
   dJointGroupID mJointGroupID;
};

////////////////////////////////////////////////////////////////////////////////

#endif //RAGDOLL_BRIDGE_ODE_SKELETON__H
