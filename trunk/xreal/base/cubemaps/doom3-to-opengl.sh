echo "converting $1 ..."

convert $1_forward.tga -flip -rotate 90 $1_px.tga
convert $1_back.tga -flip -rotate -90 $1_nx.tga

convert $1_left.tga -flip $1_py.tga
convert $1_right.tga -flop $1_ny.tga

convert $1_up.tga -flip -rotate 90 $1_pz.tga
convert $1_down.tga -flop -rotate -90 $1_nz.tga
