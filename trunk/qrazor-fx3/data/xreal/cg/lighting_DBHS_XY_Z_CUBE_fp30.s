!!FP1.0
# NV_fragment_program generated by NVIDIA Cg compiler
# cgc version 1.2.1001, build date Mar 17 2004  10:58:07
# command line args: -profile fp30
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.diffusemap
#semantic main.bumpmap
#semantic main.heightmap
#semantic main.specularmap
#semantic main.attenuationmap_xy
#semantic main.attenuationmap_z
#semantic main.attenuationmap_cube
#semantic main.view_origin
#semantic main.light_origin
#semantic main.light_color
#semantic main.bump_scale
#semantic main.height_scale
#semantic main.height_bias
#semantic main.specular_exponent
#var sampler2D diffusemap :  : texunit 0 : 1 : 1
#var sampler2D bumpmap :  : texunit 1 : 2 : 1
#var sampler2D heightmap :  : texunit 2 : 3 : 1
#var sampler2D specularmap :  : texunit 3 : 4 : 1
#var sampler2D attenuationmap_xy :  : texunit 4 : 5 : 1
#var sampler2D attenuationmap_z :  : texunit 5 : 6 : 1
#var samplerCUBE attenuationmap_cube :  : texunit 6 : 7 : 1
#var float3 view_origin :  :  : 8 : 1
#var float3 light_origin :  :  : 9 : 1
#var float3 light_color :  :  : 10 : 1
#var float bump_scale :  :  : 11 : 1
#var float height_scale :  :  : 12 : 1
#var float height_bias :  :  : 13 : 1
#var float specular_exponent :  :  : 14 : 1
#var float4 IN.position : $vin.TEXCOORD0 : TEX0 : 0 : 1
#var float4 IN.tex_diffuse_bump : $vin.TEXCOORD1 : TEX1 : 0 : 1
#var float4 IN.tex_height_spec : $vin.TEXCOORD2 : TEX2 : 0 : 1
#var float4 IN.tex_atten_xy_z : $vin.TEXCOORD3 : TEX3 : 0 : 1
#var float4 IN.tex_atten_cube : $vin.TEXCOORD4 : TEX4 : 0 : 1
#var float3 IN.tangent : $vin.TEXCOORD5 : TEX5 : 0 : 1
#var float3 IN.binormal : $vin.TEXCOORD6 : TEX6 : 0 : 1
#var float3 IN.normal : $vin.TEXCOORD7 : TEX7 : 0 : 1
#var half4 main.color : $vout.COLOR : COL : -1 : 1
DECLARE view_origin;
DECLARE light_origin;
DECLARE light_color;
DECLARE bump_scale;
DECLARE height_scale;
DECLARE height_bias;
DECLARE specular_exponent;
TEX R0.x, f[TEX2].xyxx, TEX2, 2D;
TEX R1.xyz, f[TEX3].xyxx, TEX4, 2D;
ADDR R2.xyz, view_origin.xyzx, -f[TEX0].xyzx;
DP3R R2.w, f[TEX5].xyzx, R2.xyzx;
MOVR R3.x, R2.w;
DP3R R2.w, f[TEX6].xyzx, R2.xyzx;
MOVR R3.y, R2.w;
DP3R R2.x, f[TEX7].xyzx, R2.xyzx;
MOVR R3.z, R2.x;
DP3R R2.x, R3.xyzx, R3.xyzx;
RSQR R2.x, R2.x;
MULR R3.xyz, R2.x, R3.xyzx;
ADDR R2.xyz, light_origin.xyzx, -f[TEX0].xyzx;
MOVR R2.w, height_bias.x;
MADR R2.w, R0.x, height_scale.x, R2.w;
DP3R R0.x, f[TEX5].xyzx, R2.xyzx;
MADR R4.xy, R2.w, R3.xyxx, f[TEX1].zwzz;
TEX R4.xyz, R4.xyxx, TEX1, 2D;
TEX R5.xyz, f[TEX4].xyzx, TEX6, CUBE;
ADDR R4.xyz, R4.xyzx, -{0.5}.x;
MULR R4.xyz, R4.xyzx, {2}.x;
MOVR H0.xy, R4.xyxx;
MULR R0.w, R4.z, bump_scale.x;
MOVH H0.z, R0.w;
DP3H H0.w, H0.xyzx, H0.xyzx;
RSQH H0.w, H0.w;
MULH H0.xyz, H0.w, H0.xyzx;
DP3R R0.w, f[TEX6].xyzx, R2.xyzx;
MOVR R0.y, R0.w;
DP3R R0.w, f[TEX7].xyzx, R2.xyzx;
MOVR R0.z, R0.w;
DP3R R0.w, R0.xyzx, R0.xyzx;
RSQR R0.w, R0.w;
MULR R0.xyz, R0.w, R0.xyzx;
ADDX H1.xyz, R0.xyzx, R3.xyzx;
DP3X_SAT H0.w, H0.xyzx, R0.xyzx;
DP3H H1.w, H1.xyzx, H1.xyzx;
RSQH H1.w, H1.w;
MULH H1.xyz, H1.w, H1.xyzx;
DP3X_SAT H0.x, H0.xyzx, H1.xyzx;
MULR R0.xyz, light_color.xyzx, H0.w;
POWR R0.w, H0.x, specular_exponent.x;
MADR R2.xy, R2.w, R3.xyxx, f[TEX1].xyxx;
MADR R3.xy, R2.w, R3.xyxx, f[TEX2].zwzz;
TEX R3.xyz, R3.xyxx, TEX3, 2D;
TEX R2, R2.xyxx, TEX0, 2D;
MULR R0.xyz, R2.xyzx, R0.xyzx;
MULR R3.xyz, R3.xyzx, light_color.xyzx;
MULR R3.xyz, R3.xyzx, R0.w;
MOVR H0.w, R2.w;
MOVX H0.xyz, R0.xyzx;
ADDH H1.xyz, H0.xyzx, R3.xyzx;
MOVH H0.xyz, H1.xyzx;
MULH H1.xyz, H0.xyzx, R1.xyzx;
MOVH H0.xyz, H1.xyzx;
MOVH H1.w, H0.w;
MOVR R0.x, f[TEX3].z;
MOVR R0.y, {0, 0}.y;
TEX R0.xyz, R0.xyxx, TEX5, 2D;
MULH H0.xyz, H0.xyzx, R0.xyzx;
MOVH H1.xyz, H0.xyzx;
MULH H0.xyz, H1.xyzx, R5.xyzx;
MOVH H1.xyz, H0.xyzx;
MOVH o[COLH], H1;
END
# 64 instructions, 6 R-regs, 2 H-regs.
# End of program
