#include <react3d/bridge/ode/SkeletonBridgeODE.h>

#include <ode/ode.h>

////////////////////////////////////////////////////////////////////////////////

SkeletonBridgeODE::SkeletonBridgeODE()
   : mJointGroupID(0) // set to 0 to allocate the joint normally
{
   // lazily create joint group
   if (!mJointGroupID)
   {
      mJointGroupID = dJointGroupCreate(0);
   }
}

SkeletonBridgeODE::~SkeletonBridgeODE()
{
   // destroy joints
   if (mJointGroupID)
   {
      dJointGroupDestroy(mJointGroupID);
      mJointGroupID = 0;
   }
}

////////////////////////////////////////////////////////////////////////////////
