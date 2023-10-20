add_library(
  ActsExamplesTrackFindingExaTrkXTriton SHARED
  src/TrackFindingAlgorithmExaTrkXTriton.cpp
  src/ExaTrkXTrackFindingTriton.cpp
)

target_include_directories(
  ActsExamplesTrackFindingExaTrkXTriton
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
  ActsExamplesTrackFindingExaTrkXTriton
  PUBLIC
    ActsPluginExaTrkX
    ActsExamplesFramework
    libgrpcclient.so
    libprotobuf.a    
)

install(
  TARGETS ActsExamplesTrackFindingExaTrkXTriton
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
