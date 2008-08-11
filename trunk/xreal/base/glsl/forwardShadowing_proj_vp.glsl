/*
===========================================================================
Copyright (C) 2008 Robert Beckebans <trebor_7@users.sourceforge.net>

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

#if defined(r_VertexSkinning)
attribute vec4		attr_BoneIndexes;
attribute vec4		attr_BoneWeights;
uniform int			u_VertexSkinning;
uniform mat4		u_BoneMatrix[128];
#endif

uniform mat4		u_LightAttenuationMatrix;
uniform mat4		u_ShadowMatrix;
uniform mat4		u_ModelMatrix;

varying vec4		var_Vertex;
varying vec4		var_TexAtten;
varying vec4		var_TexShadow;
varying vec4        var_Color;

void	main()
{
#if defined(r_VertexSkinning)
	if(bool(u_VertexSkinning))
	{
		vec4 vertex = vec4(0.0);
		
		for(int i = 0; i < 4; i++)
		{
			int boneIndex = int(attr_BoneIndexes[i]);
			float boneWeight = attr_BoneWeights[i];
			mat4  boneMatrix = u_BoneMatrix[boneIndex];
			
			vertex += (boneMatrix * gl_Vertex) * boneWeight;
		}

		// transform vertex position into homogenous clip-space
		gl_Position = gl_ModelViewProjectionMatrix * vertex;
		
		// transform position into world space
		var_Vertex.xyz = (u_ModelMatrix * vertex).xyz;
	}
	else
#endif
	{
		// transform vertex position into homogenous clip-space
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		
		// transform position into world space
		var_Vertex.xyz = (u_ModelMatrix * gl_Vertex).xyz;
	}
	
	// calc light attenuation in light space
	var_TexAtten = u_LightAttenuationMatrix * gl_Vertex;
	
	// calc shadow attenuation in light space
	var_TexShadow = u_ShadowMatrix * gl_Vertex;
	
	// assign color
	var_Color = gl_Color;
}