SET(LIB_NAME dtRagdoll)

FILE(GLOB DTRAGDOLL_SOURCE_FILES
     ${SOURCE_PATH}/ragdoll/*.cpp
     ${SOURCE_PATH}/ragdoll/sortme/*.cpp
     ${SOURCE_PATH}/ragdoll/core/*.cpp
     ${SOURCE_PATH}/ragdoll/core/kernel/*.cpp
     ${SOURCE_PATH}/ragdoll/bridge/*.cpp
     ${SOURCE_PATH}/ragdoll/bridge/cal3d/*.cpp
     ${SOURCE_PATH}/ragdoll/bridge/delta3d/*.cpp
     ${SOURCE_PATH}/ragdoll/bridge/ode/*.cpp
     ${SOURCE_PATH}/ragdoll/bridge/osg/*.cpp
     )

FILE(GLOB DTRAGDOLL_HEADER_FILES
     ${HEADER_PATH}/ragdoll/*.h*
     ${HEADER_PATH}/ragdoll/sortme/*.h*
     ${HEADER_PATH}/ragdoll/core/*.h*
     ${HEADER_PATH}/ragdoll/core/kernel/*.h*
     ${HEADER_PATH}/ragdoll/core/math/*.h*
     ${HEADER_PATH}/ragdoll/bridge/*.h*
     ${HEADER_PATH}/ragdoll/bridge/cal3d/*.h*
     ${HEADER_PATH}/ragdoll/bridge/delta3d/*.h*
     ${HEADER_PATH}/ragdoll/bridge/ode/*.h*
     ${HEADER_PATH}/ragdoll/bridge/osg/*.h*
     )

ADD_LIBRARY(${LIB_NAME} SHARED
            ${DTRAGDOLL_SOURCE_FILES}
            ${DTRAGDOLL_HEADER_FILES}
            ) #source files for static lib

INCLUDE_DIRECTORIES(${HEADER_PATH}
                    ${DELTA3D_INCLUDE_DIR}
                    ${CAL3D_INCLUDE_DIR}
                    )

#little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../")
ENDIF (MSVC)

#libraries to link into the executable, both Release and Debug
TARGET_LINK_LIBRARIES(${LIB_NAME}
                      optimized ${DTCORE_LIBRARY}      debug ${DTCORE_DEBUG_LIBRARY}
                      optimized ${DTUTIL_LIBRARY}      debug ${DTUTIL_DEBUG_LIBRARY}
                      optimized ${DTANIM_LIBRARY}      debug ${DTANIM_DEBUG_LIBRARY}
                      optimized ${DTGAME_LIBRARY}      debug ${DTGAME_DEBUG_LIBRARY}
                      optimized ${OSG_LIBRARY}         debug ${OSG_LIBRARY_DEBUG}
                      optimized ${OSGDB_LIBRARY}       debug ${OSGDB_LIBRARY_DEBUG}
                      optimized ${CAL3D_LIBRARY}       debug ${CAL3D_LIBRARY_DEBUG}
                      optimized ${ODE_LIBRARY}         debug ${ODE_LIBRARY_DEBUG}
                      optimized ${XERCES_LIBRARY}      debug ${XERCES_LIBRARY_DEBUG}
                      optimized ${OPENTHREADS_LIBRARY} debug ${OPENTHREADS_LIBRARY_DEBUG}
                      )

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL DTRAGDOLL_EXPORTS)
