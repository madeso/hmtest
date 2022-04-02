set(app_src
    hm/dep_gl.h
    hm/dep_sdl.h

    hm/angle.cc hm/angle.h
    hm/math.cc hm/math.h
    hm/quat.cc hm/quat.h
    hm/vec2.cc hm/vec2.h
    hm/vec3.cc hm/vec3.h
    hm/vfs.cc hm/vfs.h
    hm/opengl.cc hm/opengl.h
    hm/state.cc hm/state.cc
    hm/statemanager.cc hm/statemanager.h
    hm/imagedescription.cc hm/imagedescription.h
    hm/engine.cc hm/engine.h
    hm/media.cc hm/media.h
    hm/loadedimage.cc hm/loadedimage.h
    hm/texturerenderer.fbo.cc hm/texturerenderer.fbo.h
    hm/texturerenderer.copy.cc hm/texturerenderer.copy.h
    hm/util.cc hm/util.h
    hm/image.cc hm/image.h
    hm/shader.cc hm/shader.h
    
    hm/texturerenderer.h

    hm/vecop.h
    hm/assert.h
    hm/templatemedia.h
    
    hm/stb.cc

    hm/main.cc
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
