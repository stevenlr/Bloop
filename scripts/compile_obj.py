#!/usr/bin/python

import sys
import struct
import zlib
from math import sqrt
from array import array

positions = []
normals = []
textures = []
triangles = []
tangents = []

def register_position(line):
	coordinates = [float(p) for p in line.split(' ')]
	positions.append(coordinates)

def register_normal(line):
	coordinates = [float(p) for p in line.split(' ')]
	normals.append(coordinates)

def register_texture(line):
	coordinates = [float(p) for p in line.split(' ')]
	textures.append(coordinates)

def register_face(line):
	faces = line.split(' ')
	attributes = []

	for face in faces:
		attributes.append([int(p) for p in face.split('/')])

	triangles.append(attributes)

def compute_tangent(face):
	uv0 = textures[face[0][1] - 1]
	uv1 = textures[face[1][1] - 1]
	uv2 = textures[face[2][1] - 1]

	pos0 = positions[face[0][0] - 1]
	pos1 = positions[face[1][0] - 1]
	pos2 = positions[face[2][0] - 1]

	edge0 = [(pos1[i] - pos0[i]) for i in range(0, 3)]
	edge1 = [(pos2[i] - pos0[i]) for i in range(0, 3)]

	duv0 = [(uv1[i] - uv0[i]) for i in range(0, 2)]
	duv1 = [(uv2[i] - uv0[i]) for i in range(0, 2)]

	f = 1.0 / (duv0[0] * duv1[1] - duv0[1] * duv1[0])

	tangent = [(f * (duv1[1] * edge0[i] - duv0[1] * edge1[i])) for i in range(0, 3)]
	length = 0

	for v in tangent:
		length += v * v

	length = sqrt(length)
	tangent = [(v / length) for v in tangent]
	tangents.append(tangent)

def compute_tangents():
	for face in triangles:
		compute_tangent(face)

def make_buffer():
	attributes_buffer = array('f')
	
	for f in range(0, len(triangles)):
		face = triangles[f]

		for i in range(0, 3):
			vertex = face[i]

			attributes_buffer.fromlist(positions[vertex[0] - 1])
			attributes_buffer.fromlist(normals[vertex[2] - 1])
			attributes_buffer.fromlist(textures[vertex[1] - 1])
			attributes_buffer.fromlist(tangents[f])

	return attributes_buffer

def main(args):
	if len(args) < 2:
		print "Not enough arguments"
		return

	input_filename = args[0]
	output_filename = args[1]

	f = open(input_filename, "r")
	lines = f.readlines()
	f.close()

	for line in lines:
		if line[0] == '#' or len(line) < 2:
			continue

		tag = line[0:2]

		if tag == "vt":
			register_texture(line[3:])
		elif tag == "vn":
			register_normal(line[3:])
		elif tag[0] == 'v' and tag != "vp":
			register_position(line[2:])
		elif tag[0] == 'f':
			register_face(line[2:])

	compute_tangents()
	buf_attribs = make_buffer()

	output_buffer = buf_attribs.tostring()

	f = open(output_filename, "wb+")
	f.write(struct.pack('I', len(triangles)))
	f.write(zlib.compress(output_buffer))
	f.close()


if __name__ == "__main__":
	main(sys.argv[1:])