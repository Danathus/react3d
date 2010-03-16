#include <ragdoll/bridge/delta3d/BoneBridgeDelta3D.h>

#include <cassert>

#include <ode/ode.h>

#include <dtCore/object.h>
#include <dtCore/odebodywrap.h>
#include <dtCore/transform.h>

#include <ragdoll/bridge/osg/Math.h>
#include <ragdoll/core/kernel/Skeleton.h>
#include <ragdoll/core/kernel/Bone.h>

////////////////////////////////////////////////////////////////////////////////

BoneBridgeDelta3D::BoneBridgeDelta3D()
   : mDimensions(0.0f, 0.0f, 0.0f)
   , mpObject(NULL)
{
}

void BoneBridgeDelta3D::Configure(dtCore::Object* object, const Vec3f& dimensions)
{
   SetDimensions(dimensions);
   SetObject(object);
}

void BoneBridgeDelta3D::InitializePhysics(const Vec3f &dimensions)
{
   mpObject->SetCollisionBox();

   // set the mass for objects
   {
      dMass mass;
      dMassSetBox(&mass, 1.0,
         dimensions[X], dimensions[Y], dimensions[Z]);
      mpObject->SetMass(&mass);
   }

   // enable this body
   mpObject->EnableDynamics(true);

   // apply some damping
   {
      dtCore::ODEBodyWrap* odeWrapper = mpObject->GetBodyWrapper();

      odeWrapper->SetAutoDisableLinearThreshold( 1.0f); // 1.0f
      odeWrapper->SetAutoDisableAngularThreshold(1.0f); // 1.0f
      odeWrapper->SetAutoDisableSteps(0); // disable consideration of number of steps
      odeWrapper->SetAutoDisableTime(0.5f); // wait for half a second before disabling

      odeWrapper->SetLinearDamping( 0.01f); // 0.01f
      odeWrapper->SetAngularDamping(0.001f); // 0.001f
   }
}

void BoneBridgeDelta3D::SetWorldTransform(const Matrix4x4f& matrix)
{
   if (mpObject.get())
   {
      dtCore::Transform position;
      position.Set(ConvertKerneltoOSG(matrix));
      mpObject->SetTransform(position);

      // also reset the speed to zero...
      {
         const osg::Vec3 linear_vel(0.0, 0.0, 0.0);
         dBodySetLinearVel(mpObject->GetBodyID(), linear_vel.x(), linear_vel.y(), linear_vel.z());
      }
   }
}

Matrix4x4f BoneBridgeDelta3D::GetWorldTransform() const
{
   Matrix4x4f matrix = Matrix4x4f::Identity();

   if (mpObject.get())
   {
      // query our position/orientation in world space
      osg::Matrix matrixOSG;
      {
         dtCore::Transform position;
         mpObject->GetTransform(position);
         position.Get(matrixOSG);
      }

      matrix = ConvertOSGtoKernel(matrixOSG);
   }

   return matrix;
}

////////////////////////////////////////////////////////////////////////////////
