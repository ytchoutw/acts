add_library(
  ActsAsAService SHARED
  src/AsAService.cpp
  src/RunStandalone.cpp
)
target_include_directories(
  ActsAsAService
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:/workspace/acts/Examples/Algorithms/TrackFinding/include>
)
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}/../../Algorithms/TrackFinding/include")

target_link_libraries(
  ActsAsAService
  PUBLIC
    ActsCore
    ActsExamplesFramework
    ActsExamplesTrackFinding
    ActsExamplesTrackFitting
    ActsPluginExaTrkX
    ActsExamplesIoCsv
)

install(
  TARGETS ActsAsAService
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_executable(
  ActsAsAServiceStandalone
  src/RunStandalone.cpp
)
target_link_libraries(
  ActsAsAServiceStandalone
  PRIVATE 
    ActsCore 
    ActsExamplesFramework 
    ActsExamplesTrackFinding 
    ActsExamplesTrackFitting 
    ActsPluginExaTrkX
    ActsExamplesIoCsv)