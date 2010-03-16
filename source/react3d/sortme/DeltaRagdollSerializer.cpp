#include <ragdoll/sortme/DeltaRagdollSerializer.h>

#include <stack>

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeFilter.hpp>   // for xerces parameters
#include <xercesc/dom/DOMTreeWalker.hpp>   // for interface
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/util/XMLString.hpp>

#include <dtCore/refptr.h>
#include <dtUtil/xerceswriter.h>
#include <dtUtil/xercesutils.h>
#include <dtUtil/xercesparser.h>
#include <dtUtil/log.h>

#include <ragdoll/bridge/cal3d/BoneBridgeCAL3D.h>
#include <ragdoll/sortme/DeltaRagdoll.h>
#include <ragdoll/sortme/Ragdoll.h>
#include <ragdoll/sortme/RagdollJoint.h>

XERCES_CPP_NAMESPACE_USE;

////////////////////////////////////////////////////////////////////////////////

namespace XERCES_CPP_NAMESPACE
{
   class Locator;
   class Attributes;
}

class RAGDOLL_LIBRARY_EXPORT RagdollFileHandler : public XERCES_CPP_NAMESPACE_QUALIFIER ContentHandler
{
public:
   RagdollFileHandler();

   // implementations of pure virtuals
   void endDocument();
   void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
   void processingInstruction(const XMLCh* const target, const XMLCh* const data) {}
   void setDocumentLocator(const XERCES_CPP_NAMESPACE::Locator* const locator) {}
   void startDocument();
   void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs);
   void startPrefixMapping(const XMLCh* const prefix, const XMLCh* const uri) {}
   void endPrefixMapping(const XMLCh* const prefix) {}
   void skippedEntity(const XMLCh* const name) {}

#if XERCES_VERSION_MAJOR < 3
   virtual void characters(const XMLCh* const chars, const unsigned int length);
   virtual void ignorableWhitespace(const XMLCh* const, const unsigned int) {}
#else
   virtual void characters(const XMLCh* const chars, const XMLSize_t length);
   virtual void ignorableWhitespace(const XMLCh* const, const XMLSize_t) {}
#endif

private:
   typedef std::stack<std::string> ElementStack;
   ElementStack mElements;
   dtUtil::Log* mLogger;
};

////////////////////////////////////////////////////////////////////////////////

RagdollFileHandler::RagdollFileHandler()
   : mLogger(NULL)
{
   mLogger = &dtUtil::Log::GetInstance("DeltaRagdollSerializer.cpp");
}

#if XERCES_VERSION_MAJOR < 3
void RagdollFileHandler::characters(const XMLCh* const chars, const unsigned int length)
#else
void RagdollFileHandler::characters(const XMLCh* const chars, const XMLSize_t length)
#endif
{
   if (mElements.empty())
   {
      mLogger->LogMessage(dtUtil::Log::LOG_ERROR, __FUNCTION__, __LINE__,
         "Characters should not be found outside of an element: \"%s\"",
         dtUtil::XMLStringConverter(chars).c_str());
      return;
   }

   static struct Element
   {
      std::string name;
   } elements[] =
   {
      { "bone" },
      { "joint" },
      { "skeletalMesh" },
      { "ragdoll" },
   };

   const std::string& currentElement = mElements.top();
   for (int i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
   {
      if (elements[i].name == currentElement)
      {
         //elements[i].ProcessCharacters(chars);
      }
   }
}

void RagdollFileHandler::endDocument()
{
   if (mLogger->IsLevelEnabled(dtUtil::Log::LOG_DEBUG))
   {
      mLogger->LogMessage(dtUtil::Log::LOG_DEBUG, __FUNCTION__,  __LINE__,
         "End Parsing File");
   }
}

void RagdollFileHandler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
{
   if (mElements.empty())
   {
      mLogger->LogMessage(dtUtil::Log::LOG_ERROR, __FUNCTION__, __LINE__,
         "Attempting to pop elements off of stack and the stack is empty."
         "it should at least contain element %s.",
         dtUtil::XMLStringConverter(localname).c_str());
      return;
   }

   const char* lname = mElements.top().c_str();

   if (mLogger->IsLevelEnabled(dtUtil::Log::LOG_DEBUG))
   {
      mLogger->LogMessage(dtUtil::Log::LOG_DEBUG, __FUNCTION__,  __LINE__,
         "Ending element: \"%s\"", lname);
   }

   std::string elementStr = dtUtil::XMLStringConverter(localname).ToString();

   // todo

   mElements.pop();
}

void RagdollFileHandler::startDocument()
{
   if (mLogger->IsLevelEnabled(dtUtil::Log::LOG_DEBUG))
   {
      mLogger->LogMessage(dtUtil::Log::LOG_DEBUG, __FUNCTION__,  __LINE__,
         "Begin Parsing File");
   }

   // todo
}

void RagdollFileHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
{
   dtUtil::XMLStringConverter elementName(localname);
   std::string elementStr = elementName.ToString();

   std::string errorString;

   // todo

   if (!errorString.empty())
   {
      mLogger->LogMessage(dtUtil::Log::LOG_WARNING, __FUNCTION__, __LINE__, errorString);
   }

   // push elemet
   mElements.push(elementStr);
}

////////////////////////////////////////////////////////////////////////////////

DeltaRagdollSerializer::DeltaRagdollSerializer()
   : DeltaRagdollBuilder()
   , mFilename("")
   , mFileLoaded(false)
   , mpRagdoll(NULL)
{
   // nada
}

void DeltaRagdollSerializer::WriteFile() const
{
   dtCore::RefPtr<dtUtil::XercesWriter> writer = new dtUtil::XercesWriter();

   writer->CreateDocument("ragdoll");
   {
      DOMDocument* doc = writer->GetDocument();
      DOMElement* root = doc->getDocumentElement();

      // fill out bones
      for (size_t i = 0; i < mpRagdoll->GetRagdoll()->GetNumBones(); ++i)
      {
         const RagdollBone* bone = mpRagdoll->GetRagdoll()->GetBoneByIndex(i);
         assert(bone);

         DOMElement* boneElem = doc->createElement(XMLString::transcode("bone"));
         root->appendChild(boneElem);

         // write out bridges
         {
            DOMAttr* nameAttr = doc->createAttribute(XMLString::transcode("name"));
            const BoneBridgeCAL3D* cal3dBone = bone->GetBridgeCAL3D();
            if (cal3dBone)
            {
               nameAttr->setValue(XMLString::transcode(cal3dBone->GetBoneName().c_str()));
               boneElem->getAttributes()->setNamedItem(nameAttr);
            }
         }
      }

      // fill out joints
      for (size_t i = 0; i < mpRagdoll->GetRagdoll()->GetNumJoints(); ++i)
      {
         const RagdollJoint* joint = mpRagdoll->GetRagdoll()->GetJointByIndex(i);

         DOMElement* jointElem = doc->createElement(XMLString::transcode("joint"));
         root->appendChild(jointElem);

         // write out parent / child
         {
            DOMAttr* parentAttr = doc->createAttribute(XMLString::transcode("parent"));
            parentAttr->setValue(XMLString::transcode(joint->GetParentBone()->GetBridgeCAL3D()->GetBoneName().c_str()));
            jointElem->getAttributes()->setNamedItem(parentAttr);

            DOMAttr* childAttr = doc->createAttribute(XMLString::transcode("child"));
            childAttr->setValue(XMLString::transcode(joint->GetChildBone()->GetBridgeCAL3D()->GetBoneName().c_str()));
            jointElem->getAttributes()->setNamedItem(childAttr);
         }
      }
   }

   // now the tree has been created; write it out to a file
   writer->WriteFile(mFilename);
}

void DeltaRagdollSerializer::BuildBones(DeltaRagdoll* deltaRagdoll)
{
   dtUtil::XercesParser parser;

   RagdollFileHandler handler;

   if (parser.Parse(mFilename, handler, "ragdolldefinition.xsd"))
   {
      // todo
   }
}

void DeltaRagdollSerializer::BuildJoints(dWorldID worldID, DeltaRagdoll* deltaRagdoll)
{
   //
}

////////////////////////////////////////////////////////////////////////////////

void DeltaRagdollSerializer::ToString(std::string& toFill) const
{
   // fill out bones
   for (size_t i = 0; i < mpRagdoll->GetRagdoll()->GetNumBones(); ++i)
   {
      const RagdollBone* bone = mpRagdoll->GetRagdoll()->GetBoneByIndex(i);
      assert(bone);

      // write out bridges
      {
         const BoneBridgeCAL3D* cal3dBone = bone->GetBridgeCAL3D();
         if (cal3dBone)
         {
            cal3dBone->GetBoneName().c_str();
         }
      }
   }

   // fill out joints
   for (size_t i = 0; i < mpRagdoll->GetRagdoll()->GetNumJoints(); ++i)
   {
      const RagdollJoint* joint = mpRagdoll->GetRagdoll()->GetJointByIndex(i);

      // write out parent / child
      {
         joint->GetParentBone()->GetBridgeCAL3D()->GetBoneName().c_str();
         joint->GetChildBone()->GetBridgeCAL3D()->GetBoneName().c_str();
      }
   }
}

bool DeltaRagdollSerializer::FromString(const std::string& value)
{
   bool success = false;
   return success;
}

void DeltaRagdollSerializer::ToDataStream(dtUtil::DataStream& stream) const
{
   //
}

bool DeltaRagdollSerializer::FromDataStream(dtUtil::DataStream& stream)
{
   bool success = false;
   return success;
}

////////////////////////////////////////////////////////////////////////////////
