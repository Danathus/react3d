#ifndef RAGDOLL_STATE_H
#define RAGDOLL_STATE_H

////////////////////////////////////////////////////////////////////////////////

enum PhysicsAnimationState
{
   // no character animation
   PHYS_ANIM_STATE_NONE,
   // character animation controlled strictly by "canned" animations
   PHYS_ANIM_STATE_CANNED,
   // character animation controlled strictly by physical simulation
   PHYS_ANIM_STATE_PHYSICS,

   PHYS_ANIM_STATE_NUM,
   PHYS_ANIM_STATE_MIN     = 0,
   PHYS_ANIM_STATE_MAX     = PHYS_ANIM_STATE_NUM - 1,
   PHYS_ANIM_STATE_DEFAULT = PHYS_ANIM_STATE_CANNED
};

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_STATE_H
