#include <ragdoll/sortme/DeltaRagdollDrawable.h>

#include <cassert>

#include <dtAnim/cal3danimator.h>

#include <ragdoll/sortme/DeltaRagdoll.h>
#include <ragdoll/sortme/Ragdoll.h>

////////////////////////////////////////////////////////////////////////////////

DeltaRagdollDrawable::DeltaRagdollDrawable(dtCore::Scene* scene, dtAnim::Cal3DModelWrapper* wrapper)
   : dtAnim::CharDrawable(wrapper)
   , mpDeltaRagdoll(NULL)
   , mPhysicsAnimationState(PHYS_ANIM_STATE_DEFAULT)
{
   // create a new ragdoll
   {
      // now create the new one
      assert(scene);
      dtCore::Transformable* actor = this;
      const bool hackflip180 = false; // false?
      mpDeltaRagdoll = new DeltaRagdoll(*scene, actor);
   }
}


DeltaRagdollDrawable::~DeltaRagdollDrawable()
{
   if (mpDeltaRagdoll)
   {
      delete mpDeltaRagdoll;
      mpDeltaRagdoll = NULL;
   }
}


void DeltaRagdollDrawable::EnableVisualization(bool enable)
{
   mpDeltaRagdoll->GetRagdoll()->EnableVisualization(enable);
}


dtCore::Scene* DeltaRagdollDrawable::GetScene()
{
   return mpDeltaRagdoll ? &mpDeltaRagdoll->GetScene() : NULL;
}


const dtCore::Scene* DeltaRagdollDrawable::GetScene() const
{
   return mpDeltaRagdoll ? &mpDeltaRagdoll->GetScene() : NULL;
}


void DeltaRagdollDrawable::OnMessage(dtCore::Base::MessageData* data)
{
   assert(mAnimator.get());

   // tick the animation
   if (data->message == "preframe")
   {
      double dt = *static_cast<double*>(data->userData);

      switch (mPhysicsAnimationState)
      {
      case PHYS_ANIM_STATE_CANNED:
         // performed canned animation update
         mAnimator->Update(dt);

         // rebuild submeshes as appropriate
         {
            dtAnim::Cal3DModelWrapper* wrapper = mAnimator->GetWrapper();
            if (wrapper && mLastMeshCount != wrapper->GetMeshCount())
            {
               // there are a different number of meshes; better rebuild our drawables
               RebuildSubmeshes();
               mLastMeshCount = wrapper->GetMeshCount();
            }
         }

         // inform rag doll of new animation status
         if (mpDeltaRagdoll)
         {
            mpDeltaRagdoll->GetRagdoll()->UpdatePhysicsFromAnimation();
         }
         break;
      case PHYS_ANIM_STATE_PHYSICS:
         if (mpDeltaRagdoll)
         {
            mpDeltaRagdoll->GetRagdoll()->UpdateAnimationFromPhysics();
         }
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
