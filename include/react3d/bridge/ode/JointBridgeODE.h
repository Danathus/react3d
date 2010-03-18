#ifndef REACT3D_BRIDGE_ODE_JOINT__H
#define REACT3D_BRIDGE_ODE_JOINT__H

////////////////////////////////////////////////////////////////////////////////

#include <react3d/bridge/ode/LibraryExport.h>

typedef struct dxJoint* dJointID;

////////////////////////////////////////////////////////////////////////////////

class REACT3D_BRIDGE_ODE_LIBRARY_EXPORT JointBridgeODE
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

#endif // REACT3D_BRIDGE_ODE_JOINT__H
