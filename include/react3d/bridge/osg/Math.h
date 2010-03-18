#ifndef REACT3D_BRIDGE_OSG_MATH__H
#define REACT3D_BRIDGE_OSG_MATH__H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Matrix>
#include <react3d/core/math/Matrix.h>

////////////////////////////////////////////////////////////////////////////////

enum osgQuatComponentIndex
{
   OSG_QUAT_X, OSG_QUAT_Y, OSG_QUAT_Z, OSG_QUAT_W
};

////////////////////////////////////////////////////////////////////////////////

inline Vec3f ConvertOSGtoKernel(const osg::Vec3f& osgVec)
{
   Vec3f kernelVec;

   for (int i = 0; i < 3; ++i)
   {
      kernelVec[i] = osgVec[i];
   }

   return kernelVec;
}

inline Vec4f ConvertOSGtoKernel(const osg::Vec4f& osgVec)
{
   Vec4f kernelVec;

   for (int i = 0; i < 4; ++i)
   {
      kernelVec[i] = osgVec[i];
   }

   return kernelVec;
}

inline osg::Vec3f ConvertKerneltoOSG(const Vec3f& kernelVec)
{
   osg::Vec3f osgVec;

   for (int i = 0; i < 3; ++i)
   {
      osgVec[i] = kernelVec[i];
   }

   return osgVec;
}

inline osg::Vec4f ConvertKerneltoOSG(const Vec4f& kernelVec)
{
   osg::Vec4f osgVec;

   for (int i = 0; i < 4; ++i)
   {
      osgVec[i] = kernelVec[i];
   }

   return osgVec;
}

inline Matrix4x4f ConvertOSGtoKernel(const osg::Matrix& osgMatrix)
{
   Matrix4x4f kernelMatrix;

   for (int y = 0; y < 4; ++y)
   {
      for (int x = 0; x < 4; ++x)
      {
         kernelMatrix(y, x) = osgMatrix(y, x);
      }
   }

   return kernelMatrix;
}

inline osg::Matrix ConvertKerneltoOSG(const Matrix4x4f& kernelMatrix)
{
   osg::Matrix osgMatrix;

   for (int y = 0; y < 4; ++y)
   {
      for (int x = 0; x < 4; ++x)
      {
         osgMatrix(y, x) = kernelMatrix(y, x);
      }
   }

   return osgMatrix;
}

inline Quaternionf ConvertOSGtoKernel(const osg::Quat& osgQuat)
{
   Quaternionf kernelQuat;

   kernelQuat[X] = osgQuat[OSG_QUAT_X];
   kernelQuat[Y] = osgQuat[OSG_QUAT_Y];
   kernelQuat[Z] = osgQuat[OSG_QUAT_Z];
   kernelQuat[W] = osgQuat[OSG_QUAT_W];

   return kernelQuat;
}

/*
inline osg::Quat ConvertKerneltoOSG(const Quaternionf& kernelQuat)
{
   osg::Quat osgQuat;

   osgQuat[OSG_QUAT_X] = kernelQuat[X];
   osgQuat[OSG_QUAT_Y] = kernelQuat[Y];
   osgQuat[OSG_QUAT_Z] = kernelQuat[Z];
   osgQuat[OSG_QUAT_W] = kernelQuat[W];

   return osgQuat;
}
//*/

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_BRIDGE_OSG_MATH__H
