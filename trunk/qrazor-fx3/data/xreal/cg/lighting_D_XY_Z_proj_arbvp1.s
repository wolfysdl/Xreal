!!ARBvp1.0
# ARB_vertex_program generated by NVIDIA Cg compiler
# cgc version 1.2.1001, build date Mar 17 2004  10:58:07
# command line args: -profile arbvp1
# nv30vp backend compiling 'main' program
PARAM c0 = { 0, 1, 2, 0 };
#vendor NVIDIA Corporation
#version 1.0.02
#profile arbvp1
#program main
#var float4 IN.position : $vin.ATTR0 : ATTR0 : 0 : 1
#var float3 IN.normal : $vin.ATTR2 : ATTR2 : 0 : 1
#var float4 IN.tex0 : $vin.ATTR8 : ATTR8 : 0 : 1
#var float4 main.hposition : $vout.POSITION : HPOS : -1 : 1
#var float4 main.position : $vout.TEXCOORD0 : TEX0 : -1 : 1
#var float3 main.normal : $vout.TEXCOORD1 : TEX1 : -1 : 1
#var float4 main.tex_diffuse : $vout.TEXCOORD2 : TEX2 : -1 : 1
#var float4 main.tex_atten : $vout.TEXCOORD3 : TEX3 : -1 : 1
ATTRIB v8 = vertex.attrib[8];
ATTRIB v2 = vertex.attrib[2];
ATTRIB v0 = vertex.attrib[0];
PARAM s267[4] = { state.matrix.texture[1] };
PARAM s263[4] = { state.matrix.texture[0] };
PARAM s271[4] = { state.matrix.texture[2] };
PARAM s259[4] = { state.matrix.mvp };
	DP4 result.position.x, s259[0], v0;
	DP4 result.position.y, s259[1], v0;
	DP4 result.position.z, s259[2], v0;
	DP4 result.position.w, s259[3], v0;
	DP4 result.texcoord[0].x, s271[0], v0;
	DP4 result.texcoord[0].y, s271[1], v0;
	DP4 result.texcoord[0].z, s271[2], v0;
	DP4 result.texcoord[0].w, s271[3], v0;
	DP3 result.texcoord[1].x, s271[0].xyzx, v2.xyzx;
	DP3 result.texcoord[1].y, s271[1].xyzx, v2.xyzx;
	DP3 result.texcoord[1].z, s271[2].xyzx, v2.xyzx;
	DP4 result.texcoord[2].x, s263[0], v8;
	DP4 result.texcoord[2].y, s263[1], v8;
	DP4 result.texcoord[2].z, s263[2], v8;
	DP4 result.texcoord[2].w, s263[3], v8;
	DP4 result.texcoord[3].x, s267[0], v0;
	DP4 result.texcoord[3].y, s267[1], v0;
	DP4 result.texcoord[3].z, s267[2], v0;
	DP4 result.texcoord[3].w, s267[3], v0;
END
# 19 instructions
# 0 temp registers
# End of program
