#ifndef DELTA_RAGDOLL_SERIALIZER__H
#define DELTA_RAGDOLL_SERIALIZER__H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <dtDAL/serializeable.h>

#include <ragdoll/LibraryExport.h>
#include <ragdoll/sortme/DeltaRagdollBuilder.h>

class DeltaRagdoll;

////////////////////////////////////////////////////////////////////////////////

class RAGDOLL_LIBRARY_EXPORT DeltaRagdollSerializer : public DeltaRagdollBuilder, public dtDAL::Serializeable
{
public:
   DeltaRagdollSerializer();

   void SetFileName(const std::string& filename) { mFilename = filename; mFileLoaded = false; }
   const std::string& GetFileName() const { return mFilename; }

   void SetRagdoll(const DeltaRagdoll* ragdoll) { mpRagdoll = ragdoll; }
   const DeltaRagdoll* GetRagdoll() const { return mpRagdoll; }

   void WriteFile() const;

   // implementations of pure virtuals
   void BuildBones(DeltaRagdoll* deltaRagdoll);
   void BuildJoints(dWorldID worldID, DeltaRagdoll* deltaRagdoll); // call after added to scene
   void ToString(std::string& toFill) const;
   bool FromString(const std::string& value);
   void ToDataStream(dtUtil::DataStream& stream) const;
   bool FromDataStream(dtUtil::DataStream& stream);

protected:
private:
   std::string mFilename;
   bool        mFileLoaded;

   const DeltaRagdoll* mpRagdoll;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DELTA_RAGDOLL_SERIALIZER__H
