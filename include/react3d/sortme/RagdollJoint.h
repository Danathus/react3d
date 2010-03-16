#ifndef RAGDOLL_JOINT__H
#define RAGDOLL_JOINT__H

////////////////////////////////////////////////////////////////////////////////

class RagdollBone;
class JointBridgeODE;

////////////////////////////////////////////////////////////////////////////////

class RagdollJoint
{
public:
   RagdollJoint();

   void SetParentBone(RagdollBone* parentBone) { mpParentBone = parentBone; }
   RagdollBone* GetParentBone() { return mpParentBone; }
   const RagdollBone* GetParentBone() const { return mpParentBone; }
   void SetChildBone(RagdollBone* childBone) { mpChildBone = childBone; }
   RagdollBone* GetChildBone() { return mpChildBone; }
   const RagdollBone* GetChildBone() const { return mpChildBone; }

   void SetBridgeODE(JointBridgeODE* jointBridgeODE) { mpJointBridgeODE = jointBridgeODE; }
   JointBridgeODE* GetBridgeODE() { return mpJointBridgeODE; }
   const JointBridgeODE* GetBridgeODE() const { return mpJointBridgeODE; }

protected:
private:
   RagdollBone* mpParentBone;
   RagdollBone* mpChildBone;
   JointBridgeODE* mpJointBridgeODE;
};

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_JOINT__H
