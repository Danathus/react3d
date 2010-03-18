#ifndef REACT3D_BRIDGE_CAL3D_MATH__H
#define REACT3D_BRIDGE_CAL3D_MATH__H

////////////////////////////////////////////////////////////////////////////////

#include <cal3d/quaternion.h>
#include <react3d/core/math/Vec.h>

////////////////////////////////////////////////////////////////////////////////

enum calQuatComponentIndex
{
   CAL_QUAT_X, CAL_QUAT_Y, CAL_QUAT_Z, CAL_QUAT_W
};

////////////////////////////////////////////////////////////////////////////////

inline Quaternionf ConvertCAL3DtoKernel(const CalQuaternion& calQuat)
{
   Quaternionf kernelQuat;

   kernelQuat[X] = -calQuat[CAL_QUAT_X];
   kernelQuat[Y] = -calQuat[CAL_QUAT_Y];
   kernelQuat[Z] = -calQuat[CAL_QUAT_Z];
   kernelQuat[W] =  calQuat[CAL_QUAT_W];

   return kernelQuat;
}

inline CalQuaternion ConvertKerneltoCAL3D(const Quaternionf& kernelQuat)
{
   CalQuaternion calQuat;

   calQuat[CAL_QUAT_X] = -kernelQuat[X];
   calQuat[CAL_QUAT_Y] = -kernelQuat[Y];
   calQuat[CAL_QUAT_Z] = -kernelQuat[Z];
   calQuat[CAL_QUAT_W] =  kernelQuat[W];

   return calQuat;
}

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_BRIDGE_CAL3D_MATH__H
