#include "graphics/CobjLoader.h"

#include <stdexcept>
#include <fstream>
#include <zlib.h>

#include "Logger.h"
#include "graphics/Mesh.h"

using namespace std;

#include <iostream>

#define CHUNK 16384

Mesh *loadCobjModel(string filename)
{
	Mesh *mesh = nullptr;
	ifstream file(filename, ios_base::binary | ios_base::in);
	unsigned int nbFaces;

	if (!file.is_open()) {
		LOGERROR << "Error when opening model file " << filename << endl;
		throw runtime_error("Error when opening model file. See logs.");
	}

	file.read(reinterpret_cast<char *>(&nbFaces), sizeof(unsigned int));
	mesh = new Mesh(nbFaces);

	int bufferSize = nbFaces * 3 * (3 + 3 + 2 + 3) * sizeof(float);

	float *buffer = new float[bufferSize];

	z_stream zstream;
	int zhave, zret;
	unsigned char zInBuffer[CHUNK];
	unsigned char *zOutBuffer = reinterpret_cast<unsigned char*>(buffer);

	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.opaque = Z_NULL;

    if (inflateInit(&zstream) != Z_OK)
        throw runtime_error("Error when initializing zip stream deflating. See logs.");
	
	do {
		file.read(reinterpret_cast<char *>(zInBuffer), CHUNK);
		zstream.avail_in = static_cast<unsigned int>(file.gcount());

		if (file.bad()) {
			inflateEnd(&zstream);
			LOGERROR << "Error when reading model file " << filename << endl;
			throw runtime_error("Error when reading model file. See logs.");
		}

		if (zstream.avail_in == 0)
			break;

		zstream.next_in = zInBuffer;

		do {
			zstream.avail_out = CHUNK;
			zstream.next_out = zOutBuffer;

			if ((zret = inflate(&zstream, Z_NO_FLUSH)) == Z_STREAM_ERROR) {
				LOGERROR << "Error when inflating model file " << filename << endl;
				throw runtime_error("Error when inflating model file. See logs.");
			}
			
			switch (zret) {
				case Z_NEED_DICT:
					zret = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&zstream);
					LOGERROR << "Memory error when inflating model file " << filename << endl;
					throw runtime_error("Memory error when inflating model file. See logs.");
			}

			zhave = CHUNK - zstream.avail_out;
			zOutBuffer += zhave;
		} while (zstream.avail_out == 0);
	} while (zret != Z_STREAM_END);

	inflateEnd(&zstream);
	file.close();

	mesh->setBufferData(buffer);

	delete[] buffer;

	return mesh;
}
