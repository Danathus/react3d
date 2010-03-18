#include <react3d/bridge/cal3d/BoneBridgeCAL3D.h>

#include <cal3d/bone.h>
#include <cal3d/model.h>

#include <react3d/core/math/Matrix.hpp>
#include <react3d/core/math/BoundingBox.h>
#include <react3d/core/kernel/Skeleton.h>
#include <react3d/core/kernel/Bone.h>

#include <react3d/bridge/cal3d/Math.h>

#define WRITE_CAL3D_BONE_TRANSLATION 1 // 1 is more correct, but 0 looks better ftm :P
#define WRITE_CAL3D_BONE_ROTATION    1 // this one is fine

////////////////////////////////////////////////////////////////////////////////

static bool PlanesParallel(const CalPlane& a, const CalPlane& b)
{
   const Vec3f a_normal(a.a, a.b, a.c);
   const Vec3f b_normal(b.a, b.b, b.c);

   const bool parallel = (a_normal == b_normal || a_normal == -b_normal);

   return parallel;
}

////////////////////////////////////////////////////////////////////////////////

BoneBridgeCAL3D::BoneBridgeCAL3D(Skeleton& skeleton, CalBone* calBone, CalModel* calModel, int calBoneID)
   : mSkeleton(skeleton)
   , mpCalBone(calBone)
   , mpCalModel(calModel)
   , mCalBoneID(calBoneID)
{
   // create box first
   {
      CalCoreBone* coreBone = mpCalBone->getCoreBone();

      // start by computing our bounding box and our bounding box corners
      ComputeBoundingBox();
      std::vector<Vec3f> corners = ComputeBoundingBoxCorners(coreBone->getBoundingBox());

      Vec3f bbMin = FindMinBoundingBoxXYZ(corners);
      Vec3f bbMax = FindMaxBoundingBoxXYZ(corners);
      BoundingBox<3, float> bbox(bbMin, bbMax - bbMin);

      // don't allow any boxes of no area
      {
         Vec3f dimensions = bbox.GetDimensions();

         const bool no_volume = dimensions[X] <= 0 || dimensions[Y] <= 0 || dimensions[Z] <= 0;
         if (no_volume)
         {
            static const float kThinnest = 0.01f; //0.01f;
            // enforce a volume!
            bbox = BoundingBox<3, float>(bbox.GetMinimum(), Vec3f(
               dimensions[X] > 0 ? dimensions[X] : kThinnest,
               dimensions[Y] > 0 ? dimensions[Y] : kThinnest,
               dimensions[Z] > 0 ? dimensions[Z] : kThinnest));
         }
      }

      SetDimensions(bbox.GetDimensions());
   }
}

int BoneBridgeCAL3D::GetParentID() const
{
   int parentID = mpCalBone->getCoreBone()->getParentId();
   return parentID;
}

Bone* BoneBridgeCAL3D::GetParent()
{
   int parentID = mpCalBone->getCoreBone()->getParentId();
   Bone* parentBone = mSkeleton.GetBoneByIndex(parentID);

   return parentBone;
}

const Bone* BoneBridgeCAL3D::GetParent() const
{
   int parentID = mpCalBone->getCoreBone()->getParentId();
   const Bone* parentBone = mSkeleton.GetBoneByIndex(parentID);

   return parentBone;
}

const std::string& BoneBridgeCAL3D::GetBoneName() const
{
#if defined(CAL3D_VERSION) && CAL3D_VERSION >= 1300
   return GetCalBone()->getCoreBone()->getName();
#else
   return ((CalBone*)GetCalBone())->getCoreBone()->getName();
#endif
}

const std::list<int>& BoneBridgeCAL3D::GetListChildBoneID() const
{
   CalBone*     calBone  = (CalBone*)GetCalBone();
   CalCoreBone* coreBone = calBone->getCoreBone();

   std::list<int>& listChildID = coreBone->getListChildId();

   return listChildID;
}

Matrix4x4f BoneBridgeCAL3D::CalculateBoneSpaceTransform() const
{
   Matrix4x4f boneSpaceTransform = Matrix4x4f::Identity();

   // set up bone space geometry transform
   {
      // transform forward by half the box length
      const Vec3f& dimensions = GetDimensions();
      boneSpaceTransform.SetTranslation(Vec3f(dimensions[Y] / 2.0f, 0.0f, 0.0f));

      // set rotational offset (the inversion of the core bone absolute rotation)
      {
         CalCoreBone* coreBone = mpCalBone->getCoreBone();

         CalQuaternion calRotAbsolute = coreBone->getRotationAbsolute();
         calRotAbsolute.invert();

         Quaternionf kernelRot = ConvertCAL3DtoKernel(calRotAbsolute);

         boneSpaceTransform.SetRotate(kernelRot);
      }
   }

   return boneSpaceTransform;
}

Matrix4x4f BoneBridgeCAL3D::GetModelSpaceTransform() const
{
   Matrix4x4f matrix = Matrix4x4f::Identity();

   // update model space position
   {
      Vec3f kernelPosVec;

      const CalVector& calPosVec = mpCalBone->getTranslationAbsolute();
      for (int i = 0; i < 3; ++i)
      {
         kernelPosVec[i] = calPosVec[i];
      }

      matrix.SetTranslation(kernelPosVec);
   }

   // update model space orientation
   {
      Quaternionf kernelRotQuat;

      const CalQuaternion& calRotQuat = mpCalBone->getRotationAbsolute();
      kernelRotQuat = ConvertCAL3DtoKernel(calRotQuat);

      matrix.SetRotate(kernelRotQuat);
   }

   return matrix;
}

void BoneBridgeCAL3D::SetModelSpaceTransform(const Matrix4x4f& modelSpaceTransform) const
{
   // calculate the transform as relative to the parent bone
   Matrix4x4f localModelSpaceTransform = Matrix4x4f::Identity();
   {
      // for now use absolute model space positioning
      localModelSpaceTransform = modelSpaceTransform;

      // divide by any existing parent's model space position
      if (const Bone* parent = GetParent())
      {
         //const osg::Matrix parentTransform = ConvertKerneltoOSG(parent->GetModelSpaceTransform());
         const Matrix4x4f parentTransform = parent->GetModelSpaceTransform();

         // get inverse of parent transform
         const Matrix4x4f invParentTransform = parentTransform.Inverse();

         localModelSpaceTransform *= invParentTransform;
      }
   }

   // now update the cal3d side from this new model space information
#if WRITE_CAL3D_BONE_TRANSLATION
   // update model space position
   {
      Vec3f kernelPosVec = localModelSpaceTransform.GetTranslation();
      CalVector calPosVec;

      for (int i = 0; i < 3; ++i)
      {
         calPosVec[i] = kernelPosVec[i];
      }

      mpCalBone->setTranslation(calPosVec); // setting the relative position
   }
#endif

#if WRITE_CAL3D_BONE_ROTATION
   // update model space orientation
   {
      const Quaternionf& kernelRotQuat = localModelSpaceTransform.GetRotate();

      CalQuaternion calRotQuat;
      calRotQuat = ConvertKerneltoCAL3D(kernelRotQuat);

      mpCalBone->setRotation(calRotQuat); // setting the relative orientation
   }
#endif
}

////////////////////////////////////////////////////////////////////////////////

void BoneBridgeCAL3D::ComputeBoundingBox()
{
   CalCoreBone*  coreBone  = mpCalBone->getCoreBone();
   assert(coreBone);
   CalCoreModel* coreModel = mpCalModel->getCoreModel();
   assert(coreModel);

   if (!coreBone->isBoundingBoxPrecomputed())
   {
      coreBone->calculateBoundingBox(coreModel);
      assert(coreBone->isBoundingBoxPrecomputed());
   }
   mpCalBone->calculateBoundingBox();
}


std::vector<Vec3f> BoneBridgeCAL3D::ComputeBoundingBoxCorners(const CalBoundingBox& box) const
{
   std::vector<Vec3f> corners;

   // what we have is a set of 6 planes, stored as plane equation variables
   // what we need is 8 points for drawing a box
   // each point is the intersection of three of the planes
   // what we need to do is choose each set of 3 planes that intersects at a viable corner

   // begin at the beginning
   corners.clear();

   // iterate through all combinations of distinct planes a, b, c
   for (int a = 0; a < 6-2; ++a)
   {
      for (int b = a+1; b < 6-1; ++b)
      {
         // if a and b are parallel, skip
         if (PlanesParallel(box.plane[a], box.plane[b]))
         {
            continue;
         }

         for (int c = b+1; c < 6-0; ++c)
         {
            // the three planes indexed a, b, c potentially intersect at a viable corner

            // if any two are parallel, we should skip this trio
            // getting this far, we know a and b are not parallel -- let's check a,c and b,c
            if (PlanesParallel(box.plane[a], box.plane[c]) || PlanesParallel(box.plane[b], box.plane[c]))
            {
               continue;
            }

            // we have three orthogonal planes, they should intersect at a single point, serving as a corner to the box
            const Matrix4x4f M(
               box.plane[a].a, box.plane[a].b, box.plane[a].c, 0,
               box.plane[b].a, box.plane[b].b, box.plane[b].c, 0,
               box.plane[c].a, box.plane[c].b, box.plane[c].c, 0,
               0,              0,              0,              1);

            const Matrix4x4f Mi = M.Inverse();
            {
               const Vec4f offsets(
                  box.plane[a].d,
                  box.plane[b].d,
                  box.plane[c].d,
                  0.0f);

               const Vec4f corner = Mi * offsets;

               const Vec3f fixedCorner(corner[X], corner[Y], -corner[Z]);
               corners.push_back(fixedCorner);
            }
         }
      }
   }

   // make sure we're all good
   assert(corners.size() == 8);

   return corners;
}


Vec3f BoneBridgeCAL3D::FindMinBoundingBoxXYZ(const std::vector<Vec3f>& corners) const
{
   Vec3f result = corners[0];

   for (int i = 0; i < 8; ++i)
   {
      for (int d = 0; d < 3; ++d)
      {
         result[d] = std::min(result[d], corners[i][d]);
      }
   }

   return result;
}


Vec3f BoneBridgeCAL3D::FindMaxBoundingBoxXYZ(const std::vector<Vec3f>& corners) const
{
   Vec3f result = corners[0];

   for (int i = 0; i < 8; ++i)
   {
      for (int d = 0; d < 3; ++d)
      {
         result[d] = std::max(result[d], corners[i][d]);
      }
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////
