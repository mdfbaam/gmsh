# Gmsh - Copyright (C) 1997-2018 C. Geuzaine, J.-F. Remacle
#
# See the LICENSE.txt file for license information. Please report all
# bugs and problems to the public mailing list <gmsh@onelab.info>.

set(SRC
  OptHomMesh.cpp 
  OptHOM.cpp 
  OptHomRun.cpp 
  OptHomIntegralBoundaryDist.cpp
  OptHomCADDist.cpp
  ParamCoord.cpp 
  MetaEl.cpp 
  OptHomElastic.cpp
  OptHomFastCurving.cpp
  OptHomPeriodicity.cpp
  CADDistances.cpp
  BoundaryLayerCurver.cpp
)

file(GLOB_RECURSE HDR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
append_gmsh_src(contrib/HighOrderMeshOptimizer "${SRC};${HDR}")
