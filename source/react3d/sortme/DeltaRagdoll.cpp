#include <ragdoll/sortme/DeltaRagdoll.h>

#include <assert.h>
#include <ode/ode.h>
#include <ode/collision.h>

#include <cal3d/model.h>
#include <cal3d/skeleton.h>

#include <osg/Node>
#include <osg/ShapeDrawable>

#include <dtCore/scene.h>
#include <dtCore/object.h>
#include <dtCore/odebodywrap.h>
#include <dtAnim/animationgameactor.h>
#include <dtAnim/skeletaldrawable.h>

#include <ragdoll/bridge/osg/Math.h>
#include <ragdoll/bridge/osg/BoneBridgeOSG.h>
#include <ragdoll/bridge/cal3d/BoneBridgeCAL3D.h>
#include <ragdoll/bridge/delta3d/BoneBridgeDelta3D.h>
#include <ragdoll/sortme/Ragdoll.h>
#include <ragdoll/sortme/RagdollBone.h>

#define RESET_WORLD_TRANSFORM 0 // 0->1
#define HIDE_SKIN             0 // (commit as 0)
#define DRAW_SKELETON         0 // (commit as 0)
#define DRAW_ROOT_SPHERE      0 // (commit as 0)

////////////////////////////////////////////////////////////////////////////////

DeltaRagdoll::DeltaRagdoll(dtCore::Scene& scene, dtCore::Transformable* actor)
   : mScene(scene)
   , mpRagdoll(NULL)
{
#if HIDE_SKIN
   // control animation game actor rendering characteristics
   {
      // get the geode so we can perform scenegraph surgery
      osg::Geode *cal3dGeode = dynamic_cast<osg::Geode *>(actor->GetHelper()->GetNode());
      assert(cal3dGeode);

      // remove the skin drawable as a hack
      cal3dGeode->removeDrawables(0, 1);
      //cal3dGeode->removeDrawables(0, cal3dGeode->getNumDrawables());
      //cal3dGeode->removeDrawables(cal3dGeode->getNumDrawables()-1, 1);
   }
#endif

   // create the rag doll within this world
   mpRagdoll = new Ragdoll(actor);

   // last-minute hackery
   const double stepSize =
      1.0f / 120.0f;
      //1.0f / 60.0f;
      //1.0f / 30.0f;
   mScene.SetPhysicsStepSize(stepSize);

#if DRAW_SKELETON
   // control animation game actor rendering characteristics
   {
      // get the geode so we can perform scenegraph surgery
      osg::Geode* cal3dGeode = dynamic_cast<osg::Geode *>(actor->GetHelper()->GetNode());
      assert(cal3dGeode);

      // remove the skin drawable as a hack
      //cal3dGeode->removeDrawables(0, 1);

      cal3dGeode->addDrawable(new dtAnim::SkeletalDrawable(actor->GetHelper()->GetModelWrapper()));
   }
#endif
#if DRAW_ROOT_SPHERE
   {
      // get the geode so we can perform scenegraph surgery
      osg::Geode* sphereGeode = new osg::Geode();
      assert(sphereGeode);
      sphereGeode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0), 10.0)));
      actor->GetOSGNode()->asTransform()->addChild(sphereGeode);
   }
#endif
}

////////////////////////////////////////////////////////////////////////////////
DeltaRagdoll::~DeltaRagdoll()
{
   // remove all the bones
   for (size_t i = 0; i < GetRagdoll()->GetNumBones(); ++i)
   {
      RagdollBone* bone = GetRagdoll()->GetBoneByIndex(i);
      bool success = RemoveBone(bone);
      assert(success);
      bone = 0;
   }

   if (mpRagdoll)
   {
      delete mpRagdoll;
      mpRagdoll = 0;
   }
}

////////////////////////////////////////////////////////////////////////////////
void DeltaRagdoll::AddBone(int calBoneID, RagdollBone* bone)
{
   // add to scene at this point
   if (bone->GetBridgeDelta3D())
   {
      mScene.AddDrawable(bone->GetBridgeDelta3D()->GetObject());
   }

   // initialize physics here
   bone->GetBridgeDelta3D()->InitializePhysics(bone->GetKernelBone().GetDimensions());

   // build cal bone ID to delta rag doll bone map through this phase
   mCalBoneIDtoRagdollBone[calBoneID] = bone;

   // and initialize pose (must go after adding to scene)
   bone->UpdatePhysicsFromAnimation();
}

////////////////////////////////////////////////////////////////////////////////
bool DeltaRagdoll::RemoveBone(RagdollBone* bone)
{
   // first find in vector
   BoneList::size_type index;
   for (index = 0; index < GetRagdoll()->GetNumBones(); ++index)
   {
      if (GetRagdoll()->GetBoneByIndex(index) == bone)
      {
         break;
      }
   }
   if (index >= GetRagdoll()->GetNumBones())
   {
      // could not find it -- bail
      return false;
   }

   // clear cal bone ID to delta rag doll bone map
   if (bone->GetBridgeCAL3D())
   {
      int calBoneID = bone->GetBridgeCAL3D()->GetCalBoneID();
      //printf("removing %d from mCalBoneIDtoDeltaRagdollBone\n", calBoneID);
      mCalBoneIDtoRagdollBone.erase(calBoneID);
   }

   // remove from scene
   if (bone->GetBridgeDelta3D())
   {
      mScene.RemoveDrawable(bone->GetBridgeDelta3D()->GetObject());
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////
RagdollBone* DeltaRagdoll::GetBoneByCalBoneID(int calBoneID)
{
   return mCalBoneIDtoRagdollBone[calBoneID];
}

////////////////////////////////////////////////////////////////////////////////
const RagdollBone* DeltaRagdoll::GetBoneByCalBoneID(int calBoneID) const
{
   std::map<int, RagdollBone*>::const_iterator it = mCalBoneIDtoRagdollBone.find(calBoneID);
   if (it == mCalBoneIDtoRagdollBone.end())
   {
      return NULL;
   }
   else
   {
      return it->second;
   }
}

////////////////////////////////////////////////////////////////////////////////
osg::Matrixd DeltaRagdoll::CalculateRagdollWorldTransformFromBones() const
{
   // find position and orientation of the body at large by finding position
   //   and orientation of a bone at the root, and subtracting out its offset
   //   in model space
   osg::Matrix bodyWorldTransform;
   {
      // let's assume that bone zero is close to the root...
      const RagdollBone* baseBone = GetRagdoll()->GetBoneByIndex(0);
      //const DeltaRagdollBone *baseBone = mBones[3]; // zero doesn't seem to work so well, uhh, let's go with 1

      // get the bone position in world space -- position of delta transformable in world space (total concatenated)
      const osg::Matrix boneWorldTransform = ConvertKerneltoOSG(baseBone->GetTransformOfBoneCenterInWorld());

      //*
      // get the bone position in model space
      const osg::Matrix boneModelTransform = ConvertKerneltoOSG(baseBone->GetModelSpaceCenterTransform()); // all but world space, concatenated

      // subtract the model space transform out
      //   to get approximate skeleton position
      {
         // do this by first finding the inverse of the model transform
         const osg::Matrix invBoneModelTransform = osg::Matrix::inverse(boneModelTransform);

         // then multiply!
         //bodyWorldTransform = invBoneModelTransform * boneWorldTransform;

         //*
         // one -- looks pretty good, but still a bit off...
         bodyWorldTransform = boneWorldTransform;
         bodyWorldTransform.preMult(invBoneModelTransform);
         //*/

         /*
         // two -- totally off!
         bodyWorldTransform = boneWorldTransform;
         bodyWorldTransform.postMult(invBoneModelTransform);
         //*/

         /*
         // three -- just trying things now...
         bodyWorldTransform = boneWorldTransform;
         //*/
      }
      /*/
      // wait a second...aren't these the same?
      bodyWorldTransform = boneWorldTransform;
      // no -- bone world transform is out of date...
      //*/
   }

   return bodyWorldTransform;
}

////////////////////////////////////////////////////////////////////////////////
