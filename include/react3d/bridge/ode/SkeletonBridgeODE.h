#ifndef REACT3D_BRIDGE_ODE_SKELETON__H
#define REACT3D_BRIDGE_ODE_SKELETON__H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <react3d/bridge/ode/LibraryExport.h>

typedef struct dxJointGroup* dJointGroupID;
typedef struct dxJoint*      dJointID;

////////////////////////////////////////////////////////////////////////////////

class REACT3D_BRIDGE_ODE_LIBRARY_EXPORT SkeletonBridgeODE
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

#endif // REACT3D_BRIDGE_ODE_SKELETON__H
