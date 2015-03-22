#include "graphics/opengl/Sampler.h"

#include <stdexcept>

using namespace std;

GLuint Sampler::_samplerBound[MAX_TEXTURE_UNITS];

Sampler::Sampler(MinFilter minFilter, MagFilter magFilter, Wrap wrap)
{
	glGenSamplers(1, &_id);

	if (!_id)
		throw runtime_error("Error when generating sampler.");

	glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, wrap);
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, wrap);
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, wrap);
}

Sampler::~Sampler()
{
	if (_id)
		glDeleteSamplers(1, &_id);
}

void Sampler::bind(int unit) const
{
	if (unit < 0 || unit >= MAX_TEXTURE_UNITS)
		throw runtime_error("Binding sampler to out-of-bounds texture unit.");

	if (_samplerBound[unit] != _id) {
		glBindSampler(unit, _id);
		_samplerBound[unit] = _id;
	}
}

void Sampler::unbind(int unit) const
{
	if (unit < 0 || unit >= MAX_TEXTURE_UNITS)
		throw runtime_error("Unbinding sampler to out-of-bounds texture unit.");

	if (_samplerBound[unit] != 0) {
		glBindSampler(unit, 0);
		_samplerBound[unit] = 0;
	}
}
