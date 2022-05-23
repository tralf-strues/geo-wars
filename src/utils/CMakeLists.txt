target_sources(gwars
  PUBLIC
    ${GWARS_SOURCE_DIR}/include/utils/float_compare.hpp
    ${GWARS_SOURCE_DIR}/include/utils/random.hpp
  PRIVATE
    ${GWARS_SOURCE_DIR}/src/utils/float_compare.cpp
    ${GWARS_SOURCE_DIR}/src/utils/random.cpp
  )