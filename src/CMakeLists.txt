target_include_directories(gwars
  PUBLIC
    ${GWARS_SOURCE_DIR}/include
  )

target_sources(gwars
  PUBLIC
    ${GWARS_SOURCE_DIR}/include/Engine.h
    ${GWARS_SOURCE_DIR}/src/Engine.cpp
    ${GWARS_SOURCE_DIR}/src/Game.cpp
  )

add_subdirectory(ecs)
add_subdirectory(events)
add_subdirectory(math)
add_subdirectory(renderer)
add_subdirectory(scene)