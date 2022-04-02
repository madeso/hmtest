set(app_src
    hm/angle.cc hm/angle.h
    hm/main.cc
    hm/math.cc hm/math.h
    hm/quat.cc hm/quat.h
    hm/vec2.cc hm/vec2.h
    hm/vec3.cc hm/vec3.h
    hm/vfs.cc hm/vfs.h
    hm/opengl.cc hm/opengl.h
    hm/vecop.h
    hm/assert.h
    hm/gl.h

    hm/texturerenderer.h
    hm/texturerenderer.fbo.cc hm/texturerenderer.fbo.h
    hm/texturerenderer.copy.cc hm/texturerenderer.copy.h

    hm/stb.cc
)

set(glew_src
    ../external/glew/src/glew.c
)


source_group("" FILES ${app_src})
source_group("glew" FILES ${glew_src})
add_executable(hmtest WIN32 MACOSX_BUNDLE ${app_src} ${glew_src})
target_include_directories(hmtest SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})
target_include_directories(hmtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hmtest ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
