#pragma once

#define USE_FBO

#include "hm/texturerenderer.copy.h"
#include "hm/texturerenderer.fbo.h"


#ifdef USE_FBO
    using TextureRenderer = TextureRendererFbo;
#else
    using TextureRenderer = TextureRendererFbo;
#endif
