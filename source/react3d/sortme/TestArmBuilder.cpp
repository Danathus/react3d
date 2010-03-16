#include <ragdoll/sortme/TestArmBuilder.h>

#include <cassert>
#include <iostream>

#include <ode/ode.h>

#include <dtCore/scene.h>
#include <dtCore/object.h>

#include <ragdoll/bridge/osg/BoneBridgeOSG.h>
#include <ragdoll/bridge/delta3d/BoneBridgeDelta3D.h>
#include <ragdoll/bridge/ode/SkeletonBridgeODE.h>
#include <ragdoll/bridge/ode/JointBridgeODE.h>
#include <ragdoll/bridge/osg/Math.h>
#include <ragdoll/sortme/Ragdoll.h>
#include <ragdoll/sortme/DeltaRagdoll.h>
#include <ragdoll/sortme/RagdollJoint.h>

////////////////////////////////////////////////////////////////////////////////

TestArmBuilder::TestArmBuilder()
   : DeltaRagdollBuilder()
   , mNumBones(0)
{
   // nada
}

void TestArmBuilder::Configure(size_t numBones)
{
   mNumBones = numBones;
}

void TestArmBuilder::BuildBones(DeltaRagdoll* deltaRagdoll)
{
   Ragdoll* ragdoll = deltaRagdoll->GetRagdoll();
   //const size_t num_raw_bones = ragdoll->GetSkeleton().GetNumBones();

   float next_arm_start = 0.0f;
   for (size_t i = 0; i < mNumBones; ++i)
   {
      // create and add ragdoll bone
      {
         RagdollBone* ragdollbone = new RagdollBone(deltaRagdoll->GetRagdoll()->GetSkeleton(), false);

         // add components
         const float arm_length = 1.0f * (1.0f / (i+1));
         //const float arm_length = 1.0f;
         ragdollbone->GetKernelBone().SetDimensions(Vec3f(arm_length, 0.1f, 0.1f));
         {
            Matrix4x4f transform = Matrix4x4f::Identity();
            transform.SetTranslation(Vec3f(ragdollbone->GetKernelBone().GetDimensions()[X] / 2.0f, 0.0f, 0.0f));
            ragdollbone->GetKernelBone().SetBoneSpaceTransform(transform);
         }
         {
            Matrix4x4f transform = Matrix4x4f::Identity();
            //ragdollbone->GetKernelBone().GetDimensions()[X] * i * 1.1f
            transform.SetTranslation(Vec3f(next_arm_start, 0.0f, 0.0f));
            next_arm_start += arm_length + 0.1f;
            ragdollbone->GetKernelBone().SetModelSpaceTransform(transform);
         }

         // finished adding components
         ragdollbone->Configure();

         deltaRagdoll->GetRagdoll()->AddBone(ragdollbone);
      }

      // create and add delta ragdoll bone
      {
         RagdollBone* ragdollBone = ragdoll->GetBoneByIndex(i);

         //DeltaRagdollBone* parentDeltaBone = deltaRagdoll->GetBoneByCalBoneID(ragdollBone->GetBridgeCAL3D()->GetParentID());
         //const bool cal_parent_added = (parentDeltaBone != 0);
         const bool create_bone = true;

         if (create_bone)
         {
            // create and add components
            {
               // create and configure OSG component
               {
                  BoneBridgeOSG* boneOSG = new BoneBridgeOSG();
                  if (ragdollBone)
                  {
                     ragdollBone->SetBridgeOSG(boneOSG);
                  }
                  {
                     Vec3f dimensions = Vec3f::Zero();
                     
                     if (ragdollBone)
                     {
                        dimensions = ragdollBone->GetKernelBone().GetDimensions();
                     }

                     boneOSG->SetDimensions(dimensions);
                  }
               }

               // create and configure Delta3D component
               {
                  BoneBridgeDelta3D* boneDelta3D = new BoneBridgeDelta3D();

                  Vec3f dimensions = Vec3f::Zero();
                  if (ragdollBone)
                  {
                     ragdollBone->GetKernelBone().GetDimensions();
                  }

                  boneDelta3D->Configure(new dtCore::Object("DeltaRagdollBone"), dimensions);
                  if (ragdollBone)
                  {
                     ragdollBone->SetBridgeDelta3D(boneDelta3D);
                  }
               }
            }

            deltaRagdoll->AddBone(i, ragdollBone);

            // assign color to bone
            {
               Vec4f color(1.0f, 0.0f, 0.0f, 1.0f);

               // set color
               const float intensity = float(mNumBones - i) / float(mNumBones);
               color = color * intensity;
               ragdollBone->GetBridgeOSG()->SetColor(color);
            }

            /*
            if (deltaRagdollBone)
            {
               // we're creating a new bone -- set its collision polarity so that parents and children do not collide
               // get parent category bit mask -- if no parent, call it even, so we'll start off arbitrarily as odd
               const unsigned int parentCategory = parentDeltaBone ? parentDeltaBone->GetCollisionCategoryBits() : kBoneCollisionMaskEven;
               // now get the child category (for the new bone being created) -- defined as the opposite polarity
               const unsigned int childCategory = parentCategory & kBoneCollisionMaskOdd
                  ? kBoneCollisionMaskEven
                  : kBoneCollisionMaskOdd;

               // set our category bits as the odd or even value we've determined
               deltaRagdollBone->SetCollisionCategoryBits(childCategory);

               // set out collide bits so we will collide with everything except for our parents (and children's) polarity
               deltaRagdollBone->SetCollisionCollideBits(COLLISION_CATEGORY_MASK_ALL & ~parentCategory);
            }
            //*/
         }
      }
   }

   const size_t num_cooked_bones = deltaRagdoll->GetRagdoll()->GetNumBones();
   printf("bones: raw %d vs cooked %d\n", mNumBones, num_cooked_bones);
}

void TestArmBuilder::BuildJoints(dWorldID worldID, DeltaRagdoll* deltaRagdoll)
{
   // create joints between our bones -- technique two
   for (size_t i = 0; i < deltaRagdoll->GetRagdoll()->GetNumBones(); ++i)
   {
      RagdollBone* childbone = deltaRagdoll->GetRagdoll()->GetBoneByIndex(i);

      // initialize physics here
      childbone->GetBridgeDelta3D()->InitializePhysics(childbone->GetKernelBone().GetDimensions());

      int parentID = i-1;
      RagdollBone* parentBone = deltaRagdoll->GetBoneByCalBoneID(parentID);

      /*
      // DJMC new -- keep going to grandparents if necessary
      {
         RagdollBone* ancestor = childbone->GetRagdollBone();
         while (parentID != -1 && !parentBone)
         {
            parentID   = ancestor->GetBridgeCAL3D()->GetParentID();
            parentBone = deltaRagdoll->GetBoneByCalBoneID(parentID);

            if (parentID != -1)
            {
               ancestor = deltaRagdoll->GetRagdoll()->GetBoneByIndex(parentID);
            }
         }
      }
      // DJMC new -- keep going to grandparents if necessary
      //*/

      if (parentBone)
      {
         dBodyID body1 = parentBone->GetBridgeDelta3D()->GetObject()->GetBodyID();
         assert(body1);
         dBodyID body2 = childbone->GetBridgeDelta3D()->GetObject()->GetBodyID();
         assert(body2);

         // create joint
         dJointID jointID = 0;
         {
            jointID = dJointCreateBall(worldID, deltaRagdoll->GetRagdoll()->GetSkeletonBridgeODE()->GetJointGroupID());
            assert(jointID);
         }

         // attach and anchor
         {
            dJointAttach(jointID, body1, body2);

            Vec3f socketPos = childbone->CalculateSocketPosition();
            //*
            dJointSetBallAnchor(jointID, socketPos[X], socketPos[Y], socketPos[Z]);
            /*/
            dJointSetBallAnchor2(jointID, socketPos.x(), socketPos.y(), socketPos.z()); // no good!
            //*/

            {
               RagdollJoint* ragdollJoint = new RagdollJoint();
               ragdollJoint->SetParentBone(parentBone);
               ragdollJoint->SetChildBone(childbone);
               {
                  JointBridgeODE* odeJoint = new JointBridgeODE();
                  odeJoint->SetDataODE(jointID);
                  ragdollJoint->SetBridgeODE(odeJoint);
               }
               deltaRagdoll->GetRagdoll()->AddJoint(ragdollJoint);
            }
         }

         // configure
         {
            ConfigureJoint(jointID, worldID);
         }

         // now do amotor...
         {
            dJointID amotorJointID = dJointCreateAMotor(worldID, deltaRagdoll->GetRagdoll()->GetSkeletonBridgeODE()->GetJointGroupID()); 
            assert(amotorJointID);
            dJointAttach(amotorJointID, body1, body2);
            {
               RagdollJoint* ragdollJoint = new RagdollJoint();
               ragdollJoint->SetParentBone(parentBone);
               ragdollJoint->SetChildBone(childbone);
               {
                  JointBridgeODE* odeJoint = new JointBridgeODE();
                  odeJoint->SetDataODE(jointID);
                  ragdollJoint->SetBridgeODE(odeJoint);
               }
               deltaRagdoll->GetRagdoll()->AddJoint(ragdollJoint);
            }

            // specify the axes relative to link i-1 and initial angles to zero
            /*
            dJointSetAMotorMode(amotorJointID, dAMotorUser);
            /*/
            dJointSetAMotorMode(amotorJointID, dAMotorEuler); 
            //*/
            dJointSetAMotorNumAxes(amotorJointID, 3);
            dJointSetAMotorAxis(amotorJointID, 0, 1,  1, 0, 0);
            dJointSetAMotorAxis(amotorJointID, 1, 1,  0, 1, 0);
            dJointSetAMotorAxis(amotorJointID, 2, 1,  0, 0, 1);
            dJointSetAMotorAngle(amotorJointID, 0, 0);
            dJointSetAMotorAngle(amotorJointID, 1, 0);
            dJointSetAMotorAngle(amotorJointID, 2, 0);

            // set angular motor parameters for all three axes the same
            dReal stop = 3.14f / 2.0f; //3.14f / 8.0f;
            dReal fmax = 0.1f;
            dReal cfm = 0.0f;
            dReal erp = 1.0f;
            dJointSetAMotorParam(amotorJointID, dParamLoStop,  -stop);
            dJointSetAMotorParam(amotorJointID, dParamHiStop,   stop);
            dJointSetAMotorParam(amotorJointID, dParamVel,      0);
            dJointSetAMotorParam(amotorJointID, dParamFMax,     fmax);
            dJointSetAMotorParam(amotorJointID, dParamBounce,   0);
            dJointSetAMotorParam(amotorJointID, dParamStopCFM,  cfm);
            dJointSetAMotorParam(amotorJointID, dParamStopERP,  erp);
            //
            dJointSetAMotorParam(amotorJointID, dParamLoStop2, -stop);
            dJointSetAMotorParam(amotorJointID, dParamHiStop2,  stop);
            dJointSetAMotorParam(amotorJointID, dParamVel2,     0);
            dJointSetAMotorParam(amotorJointID, dParamFMax2,    fmax);
            dJointSetAMotorParam(amotorJointID, dParamBounce2,  0);
            dJointSetAMotorParam(amotorJointID, dParamStopCFM2, cfm);
            dJointSetAMotorParam(amotorJointID, dParamStopERP2, erp);
            //
            dJointSetAMotorParam(amotorJointID, dParamLoStop3, -stop);
            dJointSetAMotorParam(amotorJointID, dParamHiStop3,  stop);
            dJointSetAMotorParam(amotorJointID, dParamVel3,     0);
            dJointSetAMotorParam(amotorJointID, dParamFMax3,    fmax);
            dJointSetAMotorParam(amotorJointID, dParamBounce3,  0);
            dJointSetAMotorParam(amotorJointID, dParamStopCFM3, cfm);
            dJointSetAMotorParam(amotorJointID, dParamStopERP3, erp);
         }
      }
   }
   printf("%d joints created.\n", deltaRagdoll->GetRagdoll()->GetNumJoints());

   // now do some simple sanity checks
   {
      // the whole system should be a tree. As such, there should be only one bone that does not have a parent...
      int noParentCount = 0;
      for (size_t i = 0; i < deltaRagdoll->GetRagdoll()->GetNumBones(); ++i)
      {
         RagdollBone* ragdollbone = deltaRagdoll->GetRagdoll()->GetBoneByIndex(i);

         int parentID = i-1;
         RagdollBone* parentBone = deltaRagdoll->GetBoneByCalBoneID(parentID);

         if (!parentBone)
         {
            ++noParentCount;
         }
      }
      assert(noParentCount == 1);
   }
}

////////////////////////////////////////////////////////////////////////////////

void TestArmBuilder::ConfigureJoint(dJointID jointID, dWorldID worldID)
{
   // set parameters for joint
   {
      std::cout << "parameters for new joint:" << std::endl;
      // erp is "error reduction parameter"
      // it specifies the proportion of the joint error to be fixed during the next simulation step
      // 0 means no correcting force, 1 means attempt to fix all (not recommended, not completely fixed)
      // [0.1 to 0.8] recommended; default 0.2
      {
         dReal erp = dJointGetBallParam(jointID, dParamERP);
         erp = 1.0f; // 0.8f
         std::cout << "\tjoint's ERP is " << erp << std::endl;
         dJointSetBallParam(jointID, dParamERP, erp);

         //dWorldSetERP(deltaRagdoll->GetScene().GetWorldID(), erp);
      }

      // cfm is "constraint force mixing"
      // it specifies the softness / hardness of a constraint
      // 0 means hard; no violation of the constraint allowed
      // >0 means some amount of violation is allowed -- increasing can increase stability
      // <0 can cause instability -- don't do it!
      // default is 1e-005
      // Specifically this is the CFM value used when not at a stop.
      {
         dReal cfm = dJointGetBallParam(jointID, dParamCFM);
         // notes: 0.0f seems to look alright
         cfm = 0.0f;
         std::cout << "\tjoint's CFM is " << cfm << std::endl;
         dJointSetBallParam(jointID, dParamCFM, cfm);

         dWorldSetCFM(worldID, cfm);
      }

      // Low stop angle or position. For rotational joints, this stop must be greater than - pi to be effective.
      {
         dReal loStop = dJointGetBallParam(jointID, dParamLoStop);
         std::cout << "\tjoint's LoStop is " << loStop << std::endl;

         loStop = -3.1415926f;
         dJointSetBallParam(jointID, dParamLoStop, loStop);

         loStop = dJointGetBallParam(jointID, dParamLoStop);
         std::cout << "\t\tjoint's LoStop is now " << loStop << std::endl;
      }

      // High stop angle or position. For rotational joints, this stop must be less than pi to be effective.
      {
         dReal hiStop = dJointGetBallParam(jointID, dParamHiStop);
         std::cout << "\tjoint's HiStop is " << hiStop << std::endl;

         hiStop = 3.1415926f;
         dJointSetBallParam(jointID, dParamHiStop, hiStop);

         hiStop = dJointGetBallParam(jointID, dParamHiStop);
         std::cout << "\t\tjoint's HiStop is now " << hiStop << std::endl;
      }

      // The bouncyness of the stops. This is a restitution parameter in the range 0..1. 0 means the stops are not bouncy at all, 1 means maximum bouncyness.
      {
         dReal bounce = dJointGetBallParam(jointID, dParamBounce);
         std::cout << "\tjoint's bounce is " << bounce << std::endl;

         bounce = 1.0f;
         dJointSetBallParam(jointID, dParamBounce, bounce);

         bounce = dJointGetBallParam(jointID, dParamBounce);
         std::cout << "\t\tjoint's bounce is now " << bounce << std::endl;
      }

      // The error reduction parameter (ERP) used by the stops.
      {
         dReal stopERP = dJointGetBallParam(jointID, dParamStopERP);
         std::cout << "\tjoint's stop ERP is " << stopERP << std::endl;

         stopERP = 0.2f;
         dJointSetBallParam(jointID, dParamStopERP, stopERP);

         stopERP = dJointGetBallParam(jointID, dParamStopERP);
         std::cout << "\t\tjoint's stop ERP is now " << stopERP << std::endl;
      }

      // The constraint force mixing (CFM) value used by the stops.
      {
         dReal stopCFM = dJointGetBallParam(jointID, dParamStopCFM);
         std::cout << "\tjoint's stop CFM is " << stopCFM << std::endl;

         stopCFM = 1e-005f;
         dJointSetBallParam(jointID, dParamStopCFM, stopCFM);

         stopCFM = dJointGetBallParam(jointID, dParamStopCFM);
         std::cout << "\t\tjoint's stop CFM is now " << stopCFM << std::endl;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
