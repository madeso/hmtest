set(app_src
  angle.cc angle.h
  main.cc
  hmmath.cc hmmath.h
  quat.cc quat.h
  vec2.cc vec2.h
  vec3.cc vec3.h
  vecop.h
)

set(glew_src
../external/glew/src/glew.c
)


source_group("" FILES ${app_src})
source_group("glew" FILES ${glew_src})
add_executable(hmtest WIN32 MACOSX_BUNDLE ${app_src} ${glew_src})
target_include_directories(hmtest SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(hmtest ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
