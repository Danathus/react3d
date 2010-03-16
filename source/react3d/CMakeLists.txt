#file(GLOB REACT3D_SOURCE_FILES
#     ${SOURCE_PATH}/react3d/*.cpp
#     ${SOURCE_PATH}/react3d/sortme/*.cpp
#     ${SOURCE_PATH}/react3d/core/*.cpp
#     ${SOURCE_PATH}/react3d/core/kernel/*.cpp
#     ${SOURCE_PATH}/react3d/bridge/*.cpp
#     ${SOURCE_PATH}/react3d/bridge/cal3d/*.cpp
#     ${SOURCE_PATH}/react3d/bridge/delta3d/*.cpp
#     ${SOURCE_PATH}/react3d/bridge/ode/*.cpp
#     ${SOURCE_PATH}/react3d/bridge/osg/*.cpp
#)

#file(GLOB REACT3D_HEADER_FILES
#     ${HEADER_PATH}/react3d/*.h*
#     ${HEADER_PATH}/react3d/sortme/*.h*
#     ${HEADER_PATH}/react3d/core/*.h*
#     ${HEADER_PATH}/react3d/core/kernel/*.h*
#     ${HEADER_PATH}/react3d/core/math/*.h*
#     ${HEADER_PATH}/react3d/bridge/*.h*
#     ${HEADER_PATH}/react3d/bridge/cal3d/*.h*
#     ${HEADER_PATH}/react3d/bridge/delta3d/*.h*
#     ${HEADER_PATH}/react3d/bridge/ode/*.h*
#     ${HEADER_PATH}/react3d/bridge/osg/*.h*
#)

## libraries to link into the executable, both Release and Debug
#TARGET_LINK_LIBRARIES(${LIB_NAME}
#                      #optimized ${DTCORE_LIBRARY}      debug ${DTCORE_DEBUG_LIBRARY}
#                      #optimized ${DTUTIL_LIBRARY}      debug ${DTUTIL_DEBUG_LIBRARY}
#                      #optimized ${DTANIM_LIBRARY}      debug ${DTANIM_DEBUG_LIBRARY}
#                      #optimized ${DTGAME_LIBRARY}      debug ${DTGAME_DEBUG_LIBRARY}
#                      optimized ${OSG_LIBRARY}         debug ${OSG_LIBRARY_DEBUG}
#                      #optimized ${OSGDB_LIBRARY}       debug ${OSGDB_LIBRARY_DEBUG}
#                      #optimized ${CAL3D_LIBRARY}       debug ${CAL3D_LIBRARY_DEBUG}
#                      #optimized ${ODE_LIBRARY}         debug ${ODE_LIBRARY_DEBUG}
#                      #optimized ${XERCES_LIBRARY}      debug ${XERCES_LIBRARY_DEBUG}
#                      #optimized ${OPENTHREADS_LIBRARY} debug ${OPENTHREADS_LIBRARY_DEBUG}
#)

##############

add_subdirectory(core)
add_subdirectory(bridge)
#add_subdirectory(sortme)
