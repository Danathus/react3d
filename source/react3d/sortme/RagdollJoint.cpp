#include <ragdoll/sortme/RagdollJoint.h>

#include <stdio.h> // for NULL

////////////////////////////////////////////////////////////////////////////////

RagdollJoint::RagdollJoint()
   : mpParentBone(NULL)
   , mpChildBone(NULL)
   , mpJointBridgeODE(NULL)
{
   // nada
}

////////////////////////////////////////////////////////////////////////////////
