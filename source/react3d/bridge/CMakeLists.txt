if(CAL3D_FOUND)
  add_subdirectory(cal3d)
endif(CAL3D_FOUND)

if(DELTA3D_FOUND)
   add_subdirectory(delta3d)
endif(DELTA3D_FOUND)

if(ODE_FOUND)
   add_subdirectory(ode)
endif(ODE_FOUND)

if(OSG_FOUND)
   add_subdirectory(osg)
endif(OSG_FOUND)
