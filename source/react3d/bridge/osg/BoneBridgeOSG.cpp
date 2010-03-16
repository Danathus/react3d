#include <ragdoll/bridge/osg/BoneBridgeOSG.h>

#include <osg/ShapeDrawable>
#include <osg/Switch>
#include <osg/Geode>

#include <ragdoll/bridge/osg/Math.h>
#include <ragdoll/core/kernel/Skeleton.h>
#include <ragdoll/core/kernel/Bone.h>

////////////////////////////////////////////////////////////////////////////////

BoneBridgeOSG::BoneBridgeOSG()
   : mpBox(NULL)
   , mpShapeDrawable(NULL)
   , mpSwitchNode(NULL)
{
   // create a shape drawable
   {
      // create a box
      mpBox = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f);

      // create a box shape drawable
      mpShapeDrawable = new osg::ShapeDrawable(mpBox.get());
   }

   // attach it to our root
   {
      // via a geode intermediary
      osg::Geode* geode = new osg::Geode();
      geode->addDrawable(mpShapeDrawable.get());

      // add a switch node...
      mpSwitchNode = new osg::Switch();
      mpSwitchNode->addChild(geode);
   }
}

BoneBridgeOSG::~BoneBridgeOSG()
{
   mpBox           = NULL;
   mpShapeDrawable = NULL;
   mpSwitchNode    = NULL;
}

void BoneBridgeOSG::SetDimensions(const Vec3f& dimensions)
{
   mpBox->setHalfLengths(osg::Vec3(dimensions[X]*0.5f, dimensions[Y]*0.5f, dimensions[Z]*0.5f));
}

osg::Node* BoneBridgeOSG::GetRootOSG()
{
   return mpSwitchNode.get();
}

const osg::Node* BoneBridgeOSG::GetRootOSG() const
{
   return mpSwitchNode.get();
}

void BoneBridgeOSG::SetColor(const Vec4f& color)
{
   mpShapeDrawable->setColor(ConvertKerneltoOSG(color));
}

void BoneBridgeOSG::EnableVisualization(bool enable)
{
   if (enable)
   {
      mpSwitchNode->setAllChildrenOn();
   }
   else
   {
      mpSwitchNode->setAllChildrenOff();
   }
}

////////////////////////////////////////////////////////////////////////////////
