set(app_src
  angle.cc angle.h
  main.cc
  math.cc math.h
  quat.cc quat.h
  vec2.cc vec2.h
  vec3.cc vec3.h
  vecop.h
)

source_group("" FILES ${app_src})
add_executable(hmtest WIN32 MACOSX_BUNDLE ${app_src})
target_include_directories(hmtest SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(hmtest ${SDL2_LIBRARY})

