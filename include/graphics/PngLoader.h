#ifndef GRAPHICS_PNGLOADER_H
#define GRAPHICS_PNGLOADER_H

#include <string>
#include <initializer_list>

#include "graphics/opengl/Texture.h"
#include "graphics/opengl/Cubemap.h"

Texture *loadPngTexture(const std::string &filename, bool mipmaps = false);
Cubemap *loadPngCubemap(std::initializer_list<std::string> filenames, bool mipmaps = false);

#endif
