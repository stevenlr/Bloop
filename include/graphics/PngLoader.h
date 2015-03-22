#ifndef GRAPHICS_PNGLOADER_H
#define GRAPHICS_PNGLOADER_H

#include <string>

#include "graphics/opengl/Texture.h"

Texture *loadPngTexture(const std::string &filename, bool mipmaps = false);

#endif
