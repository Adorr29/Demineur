cmake_minimum_required(VERSION 3.0)

project(Demineur)

include_directories(Include)

set(SRCDIR Src/)
set(INCDIR Include/)

set(SRC
  ${SRCDIR}/Main.cpp
  ${SRCDIR}/World.cpp
  )

set(INCLUDE
  ${INCDIR}/World.hpp
  )

add_executable(Demineur ${SRC} ${INCLUDE})
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
target_link_libraries(Demineur sfml-system sfml-window sfml-graphics)
