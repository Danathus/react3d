#ifndef DELTA_RAGDOLL_DRAWABLE__H
#define DELTA_RAGDOLL_DRAWABLE__H

////////////////////////////////////////////////////////////////////////////////

#include <dtAnim/chardrawable.h>

#include <ragdoll/LibraryExport.h>
#include <ragdoll/State.h>

class DeltaRagdoll;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT DeltaRagdollDrawable : public dtAnim::CharDrawable
{
public:
   DeltaRagdollDrawable(dtCore::Scene* scene, dtAnim::Cal3DModelWrapper* wrapper);
   ~DeltaRagdollDrawable();

   /// Get/set the state of whether the ragdoll is controlled via simulation or "canned" animation
   PhysicsAnimationState GetPhysicsAnimationState() const { return mPhysicsAnimationState; }
   void SetPhysicsAnimationState(PhysicsAnimationState state) { mPhysicsAnimationState = state; }

   DeltaRagdoll* GetDeltaRagdoll() { return mpDeltaRagdoll; }
   const DeltaRagdoll* GetDeltaRagdoll() const { return mpDeltaRagdoll; }

   /// Enable/disable the visualization of the bones as red OSG boxes
   void EnableVisualization(bool enable);

   dtCore::Scene* GetScene();
   const dtCore::Scene* GetScene() const;

   // overriding
   void OnMessage(dtCore::Base::MessageData* data);

private:
   DeltaRagdoll*         mpDeltaRagdoll;
   PhysicsAnimationState mPhysicsAnimationState;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DELTA_RAGDOLL_DRAWABLE__H
