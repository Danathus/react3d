#ifndef RAGDOLL_BRIDGE_ODE_JOINT__H
#define RAGDOLL_BRIDGE_ODE_JOINT__H

////////////////////////////////////////////////////////////////////////////////

#include <ragdoll/LibraryExport.h>

typedef struct dxJoint* dJointID;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT JointBridgeODE
{
public:
   JointBridgeODE();

   void SetDataODE(dJointID dataODE) { mDataODE = dataODE; }
   dJointID GetDataODE() { return mDataODE; }
   const dJointID GetDataODE() const { return mDataODE; }

protected:
private:
   dJointID mDataODE;
};

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_BRIDGE_ODE_JOINT__H
