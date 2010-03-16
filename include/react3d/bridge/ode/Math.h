#ifndef RAGDOLL_BRIDGE_ODE_MATH__H
#define RAGDOLL_BRIDGE_ODE_MATH__H

////////////////////////////////////////////////////////////////////////////////

#include <ode/ode.h>
#include <ragdoll/core/math/Vec.h>

////////////////////////////////////////////////////////////////////////////////

enum odeQuatComponentIndex
{
   ODE_QUAT_S, ODE_QUAT_X, ODE_QUAT_Y, ODE_QUAT_Z
};

////////////////////////////////////////////////////////////////////////////////

inline Quaternionf ConvertODEtoKernel(const dQuaternion& odeQuat)
{
   Quaternionf kernelQuat;

   kernelQuat[X] =  odeQuat[ODE_QUAT_X];
   kernelQuat[Y] =  odeQuat[ODE_QUAT_Y];
   kernelQuat[Z] =  odeQuat[ODE_QUAT_Z];
   kernelQuat[W] =  odeQuat[ODE_QUAT_S];

   return kernelQuat;
}

inline dQuaternion ConvertKerneltoODE(const Quaternionf& kernelQuat)
{
   dQuaternion odeQuat;

   odeQuat[ODE_QUAT_X] =  kernelQuat[X];
   odeQuat[ODE_QUAT_Y] =  kernelQuat[Y];
   odeQuat[ODE_QUAT_Z] =  kernelQuat[Z];
   odeQuat[ODE_QUAT_S] =  kernelQuat[W];

   return odeQuat;
}

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_BRIDGE_ODE_MATH__H
