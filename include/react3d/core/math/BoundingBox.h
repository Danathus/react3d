#ifndef RAGDOLL_BOUNDING_BOX__H
#define RAGDOLL_BOUNDING_BOX__H

////////////////////////////////////////////////////////////////////////////////

#include <react3d/core/math/Vec.h>

////////////////////////////////////////////////////////////////////////////////

template <unsigned int N, typename T> class BoundingBox
{
public:
   BoundingBox() {}
   BoundingBox(const Vec<N, T>& minimum, const Vec<N, T>& dimensions)
      : mMinimum(minimum)
      , mDimensions(dimensions)
      , mMaximum(minimum + dimensions)
   {
      // nada
   }

   const Vec<N, T>& GetMinimum() const { return mMinimum; }
   const Vec<N, T>& GetMaximum() const { return mMaximum; }
   const Vec<N, T>& GetDimensions() const { return mDimensions; }

protected:
private:
   Vec<N, T> mMinimum, mMaximum, mDimensions;
};

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_BOUNDING_BOX__H
