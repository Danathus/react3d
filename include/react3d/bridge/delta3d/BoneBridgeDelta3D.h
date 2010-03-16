#ifndef RAGDOLL_BRIDGE_DELTA3D_BONE__H
#define RAGDOLL_BRIDGE_DELTA3D_BONE__H

////////////////////////////////////////////////////////////////////////////////

#include <dtCore/refptr.h>

#include <ragdoll/core/math/Matrix.h>

#include <ragdoll/LibraryExport.h>

namespace dtCore
{
   class Object;
}

class Bone;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT BoneBridgeDelta3D
{
public:
   BoneBridgeDelta3D();

   // setup
   void Configure(dtCore::Object* object, const Vec3f& dimensions);
   /// Call this once after adding to the scene to enable and initialize simulation
   void InitializePhysics(const Vec3f &dimensions);

   const Vec3f& GetDimensions() const { return mDimensions; }

   dtCore::Object* GetObject() { return mpObject.get(); }
   const dtCore::Object* GetObject() const { return mpObject.get(); }

   void SetWorldTransform(const Matrix4x4f& matrix);
   Matrix4x4f GetWorldTransform() const;

protected:
   void SetDimensions(const Vec3f& dimensions) { mDimensions = dimensions; }
   void SetObject(dtCore::Object* object) { mpObject = object; }

private:
   Vec3f                          mDimensions;
   EXPORT_RAGDOLL_REF_PTR(dtCore::Object);
   dtCore::RefPtr<dtCore::Object> mpObject;
};

////////////////////////////////////////////////////////////////////////////////

#endif //RAGDOLL_BRIDGE_DELTA3D_BONE__H
