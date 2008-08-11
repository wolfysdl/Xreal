!!FP1.0
# NV_fragment_program generated by NVIDIA Cg compiler
# cgc version 1.2.1001, build date Mar 17 2004  10:58:07
# command line args: -profile fp30
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.colormap
#semantic main.view_origin
#semantic main.refraction_index
#semantic main.fresnel_power
#semantic main.fresnel_scale
#semantic main.fresnel_bias
#var samplerCUBE colormap :  : texunit 0 : 1 : 1
#var float3 view_origin :  :  : 2 : 1
#var float refraction_index :  :  : 3 : 1
#var float fresnel_power :  :  : 4 : 1
#var float fresnel_scale :  :  : 5 : 1
#var float fresnel_bias :  :  : 6 : 1
#var float4 IN.position : $vin.TEXCOORD0 : TEX0 : 0 : 1
#var float3 IN.normal : $vin.TEXCOORD1 : TEX1 : 0 : 1
#var half4 main.color : $vout.COLOR : COL : -1 : 1
DECLARE view_origin;
DECLARE refraction_index;
DECLARE fresnel_power;
DECLARE fresnel_scale;
DECLARE fresnel_bias;
ADDR R0.xyz, f[TEX0].xyzx, -view_origin.xyzx;
DP3R R0.w, R0.xyzx, R0.xyzx;
RSQR R0.w, R0.w;
DP3R R1.x, f[TEX1].xyzx, f[TEX1].xyzx;
RSQR R1.x, R1.x;
MULR R1.xyz, R1.x, f[TEX1].xyzx;
MULR R2.xyz, R0.w, R0.xyzx;
DP3R R1.w, -R2.xyzx, R1.xyzx;
MADR R2.w, -R1.w, R1.w, {1}.x;
MULR R3.x, refraction_index.x, refraction_index.x;
MADR R2.w, -R3.x, R2.w, {1}.x;
SGTR H0.x, R2.w, {0}.x;
RSQR R2.w, |R2.w|;
RCPR R2.w, R2.w;
MULR R2.w, {1}.x, R2.w;
MADR R2.w, refraction_index.x, R1.w, -R2.w;
MULR R3.xyz, R2.w, R1.xyzx;
MADR R3.xyz, refraction_index.x, R2.xyzx, R3.xyzx;
MULR R3.xyz, R3.xyzx, H0.x;
TEX R3.xyz, R3.xyzx, TEX0, CUBE;
DP3R R1.w, R1.xyzx, R2.xyzx;
DP3R R2.x, R2.xyzx, R1.xyzx;
MULR R1.xyz, {2}.x, R1.xyzx;
MULR R1.xyz, R1.xyzx, R1.w;
MADR R1.xyz, R0.w, R0.xyzx, -R1.xyzx;
ADDR R2.x, {1}.x, -R2.x;
POWR R2.x, R2.x, fresnel_power.x;
TEX R0.xyz, R1.xyzx, TEX0, CUBE;
ADDX H0.xyz, R0.xyzx, -R3.xyzx;
MOVR R0.x, fresnel_bias.x;
MADR R0.x, R2.x, fresnel_scale.x, R0.x;
MADX H0.xyz, R0.x, H0.xyzx, R3.xyzx;
MOVH o[COLH].xyz, H0.xyzx;
MOVH o[COLH].w, {1}.x;
END
# 34 instructions, 4 R-regs, 1 H-regs.
# End of program