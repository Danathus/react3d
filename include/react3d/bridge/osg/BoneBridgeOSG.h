#ifndef REACT3D_BRIDGE_OSG_BONE__H
#define REACT3D_BRIDGE_OSG_BONE__H

////////////////////////////////////////////////////////////////////////////////

#include <osg/ref_ptr>

#include <react3d/core/math/Vec.h>
#include <react3d/bridge/osg/LibraryExport.h>

// todo -- try to get rid of these includes...
#include <osg/ShapeDrawable>
#include <osg/Switch>

namespace osg
{
   class Node;
   class Box;
   class ShapeDrawable;
   class Switch;
}

class Bone;

////////////////////////////////////////////////////////////////////////////////

class REACT3D_BRIDGE_OSG_LIBRARY_EXPORT BoneBridgeOSG
{
public:
   BoneBridgeOSG();
   ~BoneBridgeOSG();

   void SetDimensions(const Vec3f& dimensions);
   osg::Node* GetRootOSG();
   const osg::Node* GetRootOSG() const;

   /// Set the color of the boxy OSG visualization of the bone (if enabled)
   void SetColor(const Vec4f& color);
   void EnableVisualization(bool enable); // defaults to off

protected:

private:
   // visualization
   osg::ref_ptr<osg::Box>           mpBox;
   osg::ref_ptr<osg::ShapeDrawable> mpShapeDrawable; // keeping track to set color externally
   osg::ref_ptr<osg::Switch>        mpSwitchNode;
};

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_BRIDGE_OSG_BONE__H
