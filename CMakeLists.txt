cmake_minimum_required(VERSION 2.6)

PROJECT (Sharedcode)  #the project's name

FILE(TO_CMAKE_PATH "$ENV{DELTA_ROOT}" DELTA_ROOT_CMAKE)

# Set where to look for the Find*.cmake files
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules
                      ${DELTA_ROOT_CMAKE}/CMakeModules
                      ${CMAKE_MODULE_PATH})

################################################################################
# Visual studio specific
IF (WIN32)
   SET(CMAKE_DEBUG_POSTFIX  "d")
   
   OPTION(BUILD_WITH_MP "Enables the /MP multi-processor compiler option for Visual Studio 2005 and above" ON)
   MARK_AS_ADVANCED(BUILD_WITH_MP)
   IF(BUILD_WITH_MP)
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
   ENDIF(BUILD_WITH_MP)
   
   # Platform specific definitions
   ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
   ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)  
   ADD_DEFINITIONS(-DNOMINMAX)
ENDIF (WIN32)
################################################################################

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/include)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

MAKE_DIRECTORY(${EXECUTABLE_OUTPUT_PATH})
MAKE_DIRECTORY(${LIBRARY_OUTPUT_PATH})

# Add macro functions 
INCLUDE(UtilityMacros)

SET(QT_USE_QTOPENGL "true")
FIND_PACKAGE(Qt4) #for tool applicatmions

FIND_PACKAGE(OSG         REQUIRED)
FIND_PACKAGE(OpenThreads REQUIRED)
FIND_PACKAGE(Cal3d       REQUIRED)
FIND_PACKAGE(ODE         REQUIRED)
FIND_PACKAGE(XercesC     REQUIRED)
FIND_PACKAGE(CEGUI       REQUIRED)
FIND_PACKAGE(RakNet      REQUIRED)
FIND_PACKAGE(OpenAL      REQUIRED)
FIND_PACKAGE(FFMPEG      REQUIRED)
FIND_PACKAGE(OpenGL      REQUIRED)

FIND_PACKAGE(Delta3DLibs REQUIRED)

#New methods for finding the Delta3D libraries
FIND_PACKAGE(dtActors    REQUIRED)
FIND_PACKAGE(dtAudio     REQUIRED)
FIND_PACKAGE(dtCore      REQUIRED)
FIND_PACKAGE(dtDAL       REQUIRED)
FIND_PACKAGE(dtGame      REQUIRED)
FIND_PACKAGE(dtUtil      REQUIRED)



#FIND_PACKAGE(PPU REQUIRED)

INCLUDE_DIRECTORIES(${DELTA3D_INCLUDE_DIR}
                    ${DELTA3D_EXT_DIR}/inc
                    ${OSG_INCLUDE_DIR})

################################################################################
# handle the installation of the ext deps
install(DIRECTORY ${EXT_BIN_DIR} DESTINATION ${PROJECT_BINARY_DIR}
        FILES_MATCHING PATTERN "*.dll"
        PATTERN ".svn" EXCLUDE)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)
  
ADD_CUSTOM_TARGET(UNINSTALL
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

################################################################################

ADD_SUBDIRECTORY(${SOURCE_PATH}/SharedUtil)
ADD_SUBDIRECTORY(${SOURCE_PATH}/ragdoll)
ADD_SUBDIRECTORY(${SOURCE_PATH}/CommonAI)
ADD_SUBDIRECTORY(${SOURCE_PATH}/ExperimentFramework)
ADD_SUBDIRECTORY(${SOURCE_PATH}/SharedActors)
ADD_SUBDIRECTORY(${SOURCE_PATH}/BoatActors)
ADD_SUBDIRECTORY(${SOURCE_PATH}/Minimap)
ADD_SUBDIRECTORY(${SOURCE_PATH}/SerializationFPA)
ADD_SUBDIRECTORY(${SOURCE_PATH}/ShadowManager)
ADD_SUBDIRECTORY(${SOURCE_PATH}/AudioComponent)
ADD_SUBDIRECTORY(${SOURCE_PATH}/SharedHUD)
ADD_SUBDIRECTORY(${SOURCE_PATH}/SharedNet)
ADD_SUBDIRECTORY(${SOURCE_PATH}/Water)
ADD_SUBDIRECTORY(${SOURCE_PATH}/GUIManager)
ADD_SUBDIRECTORY(${SOURCE_PATH}/VOIP)

# applications
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/applications/SplineTool)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/applications/RagdollTool)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/applications/Unpackager)


################################################################################

