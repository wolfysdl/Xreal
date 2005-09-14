/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// q_math.c -- stateless support routines that are included in each code module
#include "q_shared.h"


vec3_t          vec3_origin = { 0, 0, 0 };
vec3_t          axisDefault[3] = { {1, 0, 0}
, {0, 1, 0}
, {0, 0, 1}
};


vec4_t          colorBlack = { 0, 0, 0, 1 };
vec4_t          colorRed = { 1, 0, 0, 1 };
vec4_t          colorGreen = { 0, 1, 0, 1 };
vec4_t          colorBlue = { 0, 0, 1, 1 };
vec4_t          colorYellow = { 1, 1, 0, 1 };
vec4_t          colorMagenta = { 1, 0, 1, 1 };
vec4_t          colorCyan = { 0, 1, 1, 1 };
vec4_t          colorWhite = { 1, 1, 1, 1 };
vec4_t          colorLtGrey = { 0.75, 0.75, 0.75, 1 };
vec4_t          colorMdGrey = { 0.5, 0.5, 0.5, 1 };
vec4_t          colorDkGrey = { 0.25, 0.25, 0.25, 1 };

vec4_t          g_color_table[8] = {
	{0.0, 0.0, 0.0, 1.0}
	,
	{1.0, 0.0, 0.0, 1.0}
	,
	{0.0, 1.0, 0.0, 1.0}
	,
	{1.0, 1.0, 0.0, 1.0}
	,
	{0.0, 0.0, 1.0, 1.0}
	,
	{0.0, 1.0, 1.0, 1.0}
	,
	{1.0, 0.0, 1.0, 1.0}
	,
	{1.0, 1.0, 1.0, 1.0}
	,
};


vec3_t          bytedirs[NUMVERTEXNORMALS] = {
	{-0.525731f, 0.000000f, 0.850651f}
	, {-0.442863f, 0.238856f, 0.864188f}
	,
	{-0.295242f, 0.000000f, 0.955423f}
	, {-0.309017f, 0.500000f, 0.809017f}
	,
	{-0.162460f, 0.262866f, 0.951056f}
	, {0.000000f, 0.000000f, 1.000000f}
	,
	{0.000000f, 0.850651f, 0.525731f}
	, {-0.147621f, 0.716567f, 0.681718f}
	,
	{0.147621f, 0.716567f, 0.681718f}
	, {0.000000f, 0.525731f, 0.850651f}
	,
	{0.309017f, 0.500000f, 0.809017f}
	, {0.525731f, 0.000000f, 0.850651f}
	,
	{0.295242f, 0.000000f, 0.955423f}
	, {0.442863f, 0.238856f, 0.864188f}
	,
	{0.162460f, 0.262866f, 0.951056f}
	, {-0.681718f, 0.147621f, 0.716567f}
	,
	{-0.809017f, 0.309017f, 0.500000f}
	, {-0.587785f, 0.425325f, 0.688191f}
	,
	{-0.850651f, 0.525731f, 0.000000f}
	, {-0.864188f, 0.442863f, 0.238856f}
	,
	{-0.716567f, 0.681718f, 0.147621f}
	, {-0.688191f, 0.587785f, 0.425325f}
	,
	{-0.500000f, 0.809017f, 0.309017f}
	, {-0.238856f, 0.864188f, 0.442863f}
	,
	{-0.425325f, 0.688191f, 0.587785f}
	, {-0.716567f, 0.681718f, -0.147621f}
	,
	{-0.500000f, 0.809017f, -0.309017f}
	, {-0.525731f, 0.850651f, 0.000000f}
	,
	{0.000000f, 0.850651f, -0.525731f}
	, {-0.238856f, 0.864188f, -0.442863f}
	,
	{0.000000f, 0.955423f, -0.295242f}
	, {-0.262866f, 0.951056f, -0.162460f}
	,
	{0.000000f, 1.000000f, 0.000000f}
	, {0.000000f, 0.955423f, 0.295242f}
	,
	{-0.262866f, 0.951056f, 0.162460f}
	, {0.238856f, 0.864188f, 0.442863f}
	,
	{0.262866f, 0.951056f, 0.162460f}
	, {0.500000f, 0.809017f, 0.309017f}
	,
	{0.238856f, 0.864188f, -0.442863f}
	, {0.262866f, 0.951056f, -0.162460f}
	,
	{0.500000f, 0.809017f, -0.309017f}
	, {0.850651f, 0.525731f, 0.000000f}
	,
	{0.716567f, 0.681718f, 0.147621f}
	, {0.716567f, 0.681718f, -0.147621f}
	,
	{0.525731f, 0.850651f, 0.000000f}
	, {0.425325f, 0.688191f, 0.587785f}
	,
	{0.864188f, 0.442863f, 0.238856f}
	, {0.688191f, 0.587785f, 0.425325f}
	,
	{0.809017f, 0.309017f, 0.500000f}
	, {0.681718f, 0.147621f, 0.716567f}
	,
	{0.587785f, 0.425325f, 0.688191f}
	, {0.955423f, 0.295242f, 0.000000f}
	,
	{1.000000f, 0.000000f, 0.000000f}
	, {0.951056f, 0.162460f, 0.262866f}
	,
	{0.850651f, -0.525731f, 0.000000f}
	, {0.955423f, -0.295242f, 0.000000f}
	,
	{0.864188f, -0.442863f, 0.238856f}
	, {0.951056f, -0.162460f, 0.262866f}
	,
	{0.809017f, -0.309017f, 0.500000f}
	, {0.681718f, -0.147621f, 0.716567f}
	,
	{0.850651f, 0.000000f, 0.525731f}
	, {0.864188f, 0.442863f, -0.238856f}
	,
	{0.809017f, 0.309017f, -0.500000f}
	, {0.951056f, 0.162460f, -0.262866f}
	,
	{0.525731f, 0.000000f, -0.850651f}
	, {0.681718f, 0.147621f, -0.716567f}
	,
	{0.681718f, -0.147621f, -0.716567f}
	, {0.850651f, 0.000000f, -0.525731f}
	,
	{0.809017f, -0.309017f, -0.500000f}
	, {0.864188f, -0.442863f, -0.238856f}
	,
	{0.951056f, -0.162460f, -0.262866f}
	, {0.147621f, 0.716567f, -0.681718f}
	,
	{0.309017f, 0.500000f, -0.809017f}
	, {0.425325f, 0.688191f, -0.587785f}
	,
	{0.442863f, 0.238856f, -0.864188f}
	, {0.587785f, 0.425325f, -0.688191f}
	,
	{0.688191f, 0.587785f, -0.425325f}
	, {-0.147621f, 0.716567f, -0.681718f}
	,
	{-0.309017f, 0.500000f, -0.809017f}
	, {0.000000f, 0.525731f, -0.850651f}
	,
	{-0.525731f, 0.000000f, -0.850651f}
	, {-0.442863f, 0.238856f, -0.864188f}
	,
	{-0.295242f, 0.000000f, -0.955423f}
	, {-0.162460f, 0.262866f, -0.951056f}
	,
	{0.000000f, 0.000000f, -1.000000f}
	, {0.295242f, 0.000000f, -0.955423f}
	,
	{0.162460f, 0.262866f, -0.951056f}
	, {-0.442863f, -0.238856f, -0.864188f}
	,
	{-0.309017f, -0.500000f, -0.809017f}
	, {-0.162460f, -0.262866f, -0.951056f}
	,
	{0.000000f, -0.850651f, -0.525731f}
	, {-0.147621f, -0.716567f, -0.681718f}
	,
	{0.147621f, -0.716567f, -0.681718f}
	, {0.000000f, -0.525731f, -0.850651f}
	,
	{0.309017f, -0.500000f, -0.809017f}
	, {0.442863f, -0.238856f, -0.864188f}
	,
	{0.162460f, -0.262866f, -0.951056f}
	, {0.238856f, -0.864188f, -0.442863f}
	,
	{0.500000f, -0.809017f, -0.309017f}
	, {0.425325f, -0.688191f, -0.587785f}
	,
	{0.716567f, -0.681718f, -0.147621f}
	, {0.688191f, -0.587785f, -0.425325f}
	,
	{0.587785f, -0.425325f, -0.688191f}
	, {0.000000f, -0.955423f, -0.295242f}
	,
	{0.000000f, -1.000000f, 0.000000f}
	, {0.262866f, -0.951056f, -0.162460f}
	,
	{0.000000f, -0.850651f, 0.525731f}
	, {0.000000f, -0.955423f, 0.295242f}
	,
	{0.238856f, -0.864188f, 0.442863f}
	, {0.262866f, -0.951056f, 0.162460f}
	,
	{0.500000f, -0.809017f, 0.309017f}
	, {0.716567f, -0.681718f, 0.147621f}
	,
	{0.525731f, -0.850651f, 0.000000f}
	, {-0.238856f, -0.864188f, -0.442863f}
	,
	{-0.500000f, -0.809017f, -0.309017f}
	, {-0.262866f, -0.951056f, -0.162460f}
	,
	{-0.850651f, -0.525731f, 0.000000f}
	, {-0.716567f, -0.681718f, -0.147621f}
	,
	{-0.716567f, -0.681718f, 0.147621f}
	, {-0.525731f, -0.850651f, 0.000000f}
	,
	{-0.500000f, -0.809017f, 0.309017f}
	, {-0.238856f, -0.864188f, 0.442863f}
	,
	{-0.262866f, -0.951056f, 0.162460f}
	, {-0.864188f, -0.442863f, 0.238856f}
	,
	{-0.809017f, -0.309017f, 0.500000f}
	, {-0.688191f, -0.587785f, 0.425325f}
	,
	{-0.681718f, -0.147621f, 0.716567f}
	, {-0.442863f, -0.238856f, 0.864188f}
	,
	{-0.587785f, -0.425325f, 0.688191f}
	, {-0.309017f, -0.500000f, 0.809017f}
	,
	{-0.147621f, -0.716567f, 0.681718f}
	, {-0.425325f, -0.688191f, 0.587785f}
	,
	{-0.162460f, -0.262866f, 0.951056f}
	, {0.442863f, -0.238856f, 0.864188f}
	,
	{0.162460f, -0.262866f, 0.951056f}
	, {0.309017f, -0.500000f, 0.809017f}
	,
	{0.147621f, -0.716567f, 0.681718f}
	, {0.000000f, -0.525731f, 0.850651f}
	,
	{0.425325f, -0.688191f, 0.587785f}
	, {0.587785f, -0.425325f, 0.688191f}
	,
	{0.688191f, -0.587785f, 0.425325f}
	, {-0.955423f, 0.295242f, 0.000000f}
	,
	{-0.951056f, 0.162460f, 0.262866f}
	, {-1.000000f, 0.000000f, 0.000000f}
	,
	{-0.850651f, 0.000000f, 0.525731f}
	, {-0.955423f, -0.295242f, 0.000000f}
	,
	{-0.951056f, -0.162460f, 0.262866f}
	, {-0.864188f, 0.442863f, -0.238856f}
	,
	{-0.951056f, 0.162460f, -0.262866f}
	, {-0.809017f, 0.309017f, -0.500000f}
	,
	{-0.864188f, -0.442863f, -0.238856f}
	, {-0.951056f, -0.162460f, -0.262866f}
	,
	{-0.809017f, -0.309017f, -0.500000f}
	, {-0.681718f, 0.147621f, -0.716567f}
	,
	{-0.681718f, -0.147621f, -0.716567f}
	, {-0.850651f, 0.000000f, -0.525731f}
	,
	{-0.688191f, 0.587785f, -0.425325f}
	, {-0.587785f, 0.425325f, -0.688191f}
	,
	{-0.425325f, 0.688191f, -0.587785f}
	, {-0.425325f, -0.688191f, -0.587785f}
	,
	{-0.587785f, -0.425325f, -0.688191f}
	, {-0.688191f, -0.587785f, -0.425325f}
};

//==============================================================

int Q_rand(int *seed)
{
	*seed = (69069 * *seed + 1);
	return *seed;
}

float Q_random(int *seed)
{
	return (Q_rand(seed) & 0xffff) / (float)0x10000;
}

float Q_crandom(int *seed)
{
	return 2.0 * (Q_random(seed) - 0.5);
}

#ifdef __LCC__

int VectorCompare(const vec3_t v1, const vec3_t v2)
{
	if(v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2])
	{
		return 0;
	}
	return 1;
}

vec_t VectorLength(const vec3_t v)
{
	return (vec_t) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

vec_t VectorLengthSquared(const vec3_t v)
{
	return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

vec_t Distance(const vec3_t p1, const vec3_t p2)
{
	vec3_t          v;

	VectorSubtract(p2, p1, v);
	return VectorLength(v);
}

vec_t DistanceSquared(const vec3_t p1, const vec3_t p2)
{
	vec3_t          v;

	VectorSubtract(p2, p1, v);
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}


// fast vector normalize routine that does not check to make sure
// that length != 0, nor does it return length, uses rsqrt approximation
void VectorNormalizeFast(vec3_t v)
{
	float           ilength;

	ilength = Q_rsqrt(DotProduct(v, v));

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

void VectorInverse(vec3_t v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void CrossProduct(const vec3_t v1, const vec3_t v2, vec3_t cross)
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

#endif

//=======================================================

signed char ClampChar(int i)
{
	if(i < -128)
	{
		return -128;
	}
	if(i > 127)
	{
		return 127;
	}
	return i;
}

signed short ClampShort(int i)
{
	if(i < -32768)
	{
		return -32768;
	}
	if(i > 0x7fff)
	{
		return 0x7fff;
	}
	return i;
}


// this isn't a real cheap function to call!
int DirToByte(vec3_t dir)
{
	int             i, best;
	float           d, bestd;

	if(!dir)
	{
		return 0;
	}

	bestd = 0;
	best = 0;
	for(i = 0; i < NUMVERTEXNORMALS; i++)
	{
		d = DotProduct(dir, bytedirs[i]);
		if(d > bestd)
		{
			bestd = d;
			best = i;
		}
	}

	return best;
}

void ByteToDir(int b, vec3_t dir)
{
	if(b < 0 || b >= NUMVERTEXNORMALS)
	{
		VectorCopy(vec3_origin, dir);
		return;
	}
	VectorCopy(bytedirs[b], dir);
}


unsigned ColorBytes3(float r, float g, float b)
{
	unsigned        i;

	((byte *) & i)[0] = r * 255;
	((byte *) & i)[1] = g * 255;
	((byte *) & i)[2] = b * 255;

	return i;
}

unsigned ColorBytes4(float r, float g, float b, float a)
{
	unsigned        i;

	((byte *) & i)[0] = r * 255;
	((byte *) & i)[1] = g * 255;
	((byte *) & i)[2] = b * 255;
	((byte *) & i)[3] = a * 255;

	return i;
}

float NormalizeColor(const vec3_t in, vec3_t out)
{
	float           max;

	max = in[0];
	if(in[1] > max)
	{
		max = in[1];
	}
	if(in[2] > max)
	{
		max = in[2];
	}

	if(!max)
	{
		VectorClear(out);
	}
	else
	{
		out[0] = in[0] / max;
		out[1] = in[1] / max;
		out[2] = in[2] / max;
	}
	return max;
}

void ClampColor(vec4_t color)
{
	int             i;

	for(i = 0; i < 4; i++)
	{
		if(color[i] < 0)
			color[i] = 0;

		if(color[i] > 1)
			color[i] = 1;
	}
}



/*
=====================
PlaneFromPoints

Returns false if the triangle is degenrate.
The normal will point out of the clock for clockwise ordered points
=====================
*/
qboolean PlaneFromPoints(vec4_t plane, const vec3_t a, const vec3_t b, const vec3_t c)
{
	vec3_t          d1, d2;

	VectorSubtract(b, a, d1);
	VectorSubtract(c, a, d2);
	CrossProduct(d2, d1, plane);
	if(VectorNormalize(plane) == 0)
	{
		return qfalse;
	}

	plane[3] = DotProduct(a, plane);
	return qtrue;
}


/*
===============
RotatePointAroundVector

This is not implemented very well...
===============
*/
void RotatePointAroundVector(vec3_t dst, const vec3_t dir, const vec3_t point, float degrees)
{
	float           m[3][3];
	float           im[3][3];
	float           zrot[3][3];
	float           tmpmat[3][3];
	float           rot[3][3];
	int             i;
	vec3_t          vr, vup, vf;
	float           rad;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector(vr, dir);
	CrossProduct(vr, vf, vup);

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy(im, m, sizeof(im));

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset(zrot, 0, sizeof(zrot));
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	rad = DEG2RAD(degrees);
	zrot[0][0] = cos(rad);
	zrot[0][1] = sin(rad);
	zrot[1][0] = -sin(rad);
	zrot[1][1] = cos(rad);

	AxisMultiply(m, zrot, tmpmat);
	AxisMultiply(tmpmat, im, rot);

	for(i = 0; i < 3; i++)
	{
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}


/*
===============
RotateAroundDirection
===============
*/
void RotateAroundDirection(vec3_t axis[3], float yaw)
{

	// create an arbitrary axis[1] 
	PerpendicularVector(axis[1], axis[0]);

	// rotate it around axis[0] by yaw
	if(yaw)
	{
		vec3_t          temp;

		VectorCopy(axis[1], temp);
		RotatePointAroundVector(axis[1], axis[0], temp, yaw);
	}

	// cross to get axis[2]
	CrossProduct(axis[0], axis[1], axis[2]);
}



void vectoangles(const vec3_t value1, vec3_t angles)
{
	float           forward;
	float           yaw, pitch;

	if(value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if(value1[2] > 0)
		{
			pitch = 90;
		}
		else
		{
			pitch = 270;
		}
	}
	else
	{
		if(value1[0])
		{
			yaw = (atan2(value1[1], value1[0]) * 180 / M_PI);
		}
		else if(value1[1] > 0)
		{
			yaw = 90;
		}
		else
		{
			yaw = 270;
		}
		if(yaw < 0)
		{
			yaw += 360;
		}

		forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
		pitch = (atan2(value1[2], forward) * 180 / M_PI);
		if(pitch < 0)
		{
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}


/*
=================
AnglesToAxis
=================
*/
void AnglesToAxis(const vec3_t angles, vec3_t axis[3])
{
	vec3_t          right;

	// angle vectors returns "right" instead of "y axis"
	AngleVectors(angles, axis[0], right, axis[2]);
	VectorSubtract(vec3_origin, right, axis[1]);
}

void AxisClear(vec3_t axis[3])
{
	axis[0][0] = 1;
	axis[0][1] = 0;
	axis[0][2] = 0;
	axis[1][0] = 0;
	axis[1][1] = 1;
	axis[1][2] = 0;
	axis[2][0] = 0;
	axis[2][1] = 0;
	axis[2][2] = 1;
}

void AxisCopy(vec3_t in[3], vec3_t out[3])
{
	VectorCopy(in[0], out[0]);
	VectorCopy(in[1], out[1]);
	VectorCopy(in[2], out[2]);
}

void ProjectPointOnPlane(vec3_t dst, const vec3_t p, const vec3_t normal)
{
	float           d;
	vec3_t          n;
	float           inv_denom;

	inv_denom = DotProduct(normal, normal);
#ifndef Q3_VM
	assert(Q_fabs(inv_denom) != 0.0f);	// bk010122 - zero vectors get here
#endif
	inv_denom = 1.0f / inv_denom;

	d = DotProduct(normal, p) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}


/*
================
MakeNormalVectors

Given a normalized forward vector, create two
other perpendicular vectors
================
*/
void MakeNormalVectors(const vec3_t forward, vec3_t right, vec3_t up)
{
	float           d;

	// this rotate and negate guarantees a vector
	// not colinear with the original
	right[1] = -forward[0];
	right[2] = forward[1];
	right[0] = forward[2];

	d = DotProduct(right, forward);
	VectorMA(right, -d, forward, right);
	VectorNormalize(right);
	CrossProduct(right, forward, up);
}


void VectorRotate(vec3_t in, vec3_t matrix[3], vec3_t out)
{
	out[0] = DotProduct(in, matrix[0]);
	out[1] = DotProduct(in, matrix[1]);
	out[2] = DotProduct(in, matrix[2]);
}


//============================================================================

#if !idppc
/*
** float q_rsqrt( float number )
*/
float Q_rsqrt(float number)
{
	float           y;

#if id386_3dnow && defined __GNUC__
//#error Q_rqsrt
	femms();
	asm volatile    (			// lo                   | hi
						"movd		(%%eax),	%%mm0\n"	// in                   |   -
						 "pfrsqrt	%%mm0,		%%mm1\n"	// 1/sqrt(in)           | 1/sqrt(in)    (approx)
						"movq		%%mm1,		%%mm2\n"	// 1/sqrt(in)           | 1/sqrt(in)    (approx)
						"pfmul		%%mm1,		%%mm1\n"	// (1/sqrt(in))?        | (1/sqrt(in))?     step 1
						"pfrsqit1	%%mm0,		%%mm1\n"	// intermediate                             step 2
						"pfrcpit2	%%mm2,		%%mm1\n"	// 1/sqrt(in) (full 24-bit precision)       step 3
						 "movd		%%mm1,		(%%edx)\n"::"a" (&number), "d"(&y):"memory");

	femms();
#else
	long            i;
	float           x2;
	const float     threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;			// evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);	// what the fuck?
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));	// 1st iteration
//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

#ifndef Q3_VM
#ifdef __linux__
	assert(!isnan(y));			// bk010122 - FPE?
#endif
#endif
#endif							// id386_3dnow
	return y;
}

float Q_fabs(float f)
{
	int             tmp = *(int *)&f;

	tmp &= 0x7FFFFFFF;
	return *(float *)&tmp;
}
#endif

//============================================================

/*
===============
LerpAngle

===============
*/
float LerpAngle(float from, float to, float frac)
{
	float           a;

	if(to - from > 180)
	{
		to -= 360;
	}
	if(to - from < -180)
	{
		to += 360;
	}
	a = from + frac * (to - from);

	return a;
}


/*
=================
AngleSubtract

Always returns a value from -180 to 180
=================
*/
float AngleSubtract(float a1, float a2)
{
	float           a;

	a = a1 - a2;
	while(a > 180)
	{
		a -= 360;
	}
	while(a < -180)
	{
		a += 360;
	}
	return a;
}


void AnglesSubtract(vec3_t v1, vec3_t v2, vec3_t v3)
{
	v3[0] = AngleSubtract(v1[0], v2[0]);
	v3[1] = AngleSubtract(v1[1], v2[1]);
	v3[2] = AngleSubtract(v1[2], v2[2]);
}


float AngleMod(float a)
{
	a = (360.0 / 65536) * ((int)(a * (65536 / 360.0)) & 65535);
	return a;
}


/*
=================
AngleNormalize360

returns angle normalized to the range [0 <= angle < 360]
=================
*/
float AngleNormalize360(float angle)
{
	return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535);
}


/*
=================
AngleNormalize180

returns angle normalized to the range [-180 < angle <= 180]
=================
*/
float AngleNormalize180(float angle)
{
	angle = AngleNormalize360(angle);
	if(angle > 180.0)
	{
		angle -= 360.0;
	}
	return angle;
}


/*
=================
AngleDelta

returns the normalized delta from angle1 to angle2
=================
*/
float AngleDelta(float angle1, float angle2)
{
	return AngleNormalize180(angle1 - angle2);
}


//============================================================


/*
=================
SetPlaneSignbits
=================
*/
void SetPlaneSignbits(cplane_t * out)
{
	int             bits, j;

	// for fast box on planeside test
	bits = 0;
	for(j = 0; j < 3; j++)
	{
		if(out->normal[j] < 0)
		{
			bits |= 1 << j;
		}
	}
	out->signbits = bits;
}


/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2

// this is the slow, general version
int BoxOnPlaneSide2 (vec3_t emins, vec3_t emaxs, struct cplane_s *p)
{
	int		i;
	float	dist1, dist2;
	int		sides;
	vec3_t	corners[2];

	for (i=0 ; i<3 ; i++)
	{
		if (p->normal[i] < 0)
		{
			corners[0][i] = emins[i];
			corners[1][i] = emaxs[i];
		}
		else
		{
			corners[1][i] = emins[i];
			corners[0][i] = emaxs[i];
		}
	}
	dist1 = DotProduct (p->normal, corners[0]) - p->dist;
	dist2 = DotProduct (p->normal, corners[1]) - p->dist;
	sides = 0;
	if (dist1 >= 0)
		sides = 1;
	if (dist2 < 0)
		sides |= 2;

	return sides;
}
 
