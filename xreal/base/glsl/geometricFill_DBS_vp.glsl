/*
===========================================================================
Copyright (C) 2007 Robert Beckebans <trebor_7@users.sourceforge.net>

This file is part of XreaL source code.

XreaL source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

XreaL source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with XreaL source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

attribute vec4		attr_TexCoord0;
attribute vec3		attr_Tangent;
attribute vec3		attr_Binormal;

uniform mat4		u_ModelMatrix;

varying vec4		var_Vertex;
varying vec2		var_TexDiffuse;
varying vec2		var_TexNormal;
varying vec2		var_TexSpecular;
varying vec4		var_Tangent;
varying vec4		var_Binormal;
varying vec4		var_Normal;

void	main()
{
	// transform vertex position into homogenous clip-space
	gl_Position = ftransform();
	
	// assign vertex position in object space
	var_Vertex = gl_Vertex;
	
	// transform diffusemap texcoords
	var_TexDiffuse = (gl_TextureMatrix[0] * attr_TexCoord0).st;
	
	// transform normalmap texcoords
	var_TexNormal = (gl_TextureMatrix[1] * attr_TexCoord0).st;
	
	// transform specularmap texture coords
	var_TexSpecular = (gl_TextureMatrix[2] * attr_TexCoord0).st;
	
	// construct tangent-space-to-world-space 3x3 matrix
	var_Tangent.xyz = (u_ModelMatrix * vec4(attr_Tangent, 0.0)).xyz;
	var_Binormal.xyz = (u_ModelMatrix * vec4(attr_Binormal, 0.0)).xyz;
	var_Normal.xyz = (u_ModelMatrix * vec4(gl_Normal, 0.0)).xyz;
}
