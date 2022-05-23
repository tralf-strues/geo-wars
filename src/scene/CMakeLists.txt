target_sources(gwars
  PUBLIC
    ${GWARS_SOURCE_DIR}/include/scene/components.hpp
    ${GWARS_SOURCE_DIR}/include/scene/scene.hpp
  PRIVATE
    ${GWARS_SOURCE_DIR}/src/scene/components.cpp
    ${GWARS_SOURCE_DIR}/src/scene/scene.cpp
  )