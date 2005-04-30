/// ============================================================================
/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2005 Robert Beckebans <trebor_7@users.sourceforge.net>
Please see the file "AUTHORS" for a list of contributors

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/// ============================================================================
#ifndef MAP_LOCAL_H
#define MAP_LOCAL_H

/// includes ===================================================================
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
// system -------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <map>

// qrazor-fx ----------------------------------------------------------------
#include "files.h"

// xreal --------------------------------------------------------------------



class face_t;
class portal_t;


#define	MAX_POINTS_ON_WINDING	64

// you can define on_epsilon in the makefile as tighter
#ifndef	ON_EPSILON
#define	ON_EPSILON	0.1
#endif

class winding_c
{
public:
	winding_c(int points = 0);
	winding_c(const vec3_c &normal, vec_t dist);
	winding_c(const cplane_c &base);
	~winding_c();
	
	void		initFromPlane(const vec3_c &normal, vec_t dist);
	
	cplane_c	calcPlane() const;
	vec_t		calcArea() const;
	aabb_c		calcAABB() const;
	vec3_c		calcCenter() const;
	bool		isTiny() const;
	
	//! Reverse vertex order
	void		reverse();
	
	//! Splits winding by split plane
	void		clip(const cplane_c &split, winding_c **front, winding_c **back, vec_t epsilon = ON_EPSILON) const;
	//! Returns the fragment of in that is on the front side of the cliping plane.
	winding_c*	chop(const cplane_c &split) const;
	//! Changes the original if clipped
	bool		chopInPlace(const cplane_c &split, vec_t epsilon = ON_EPSILON);

	void		removeColinearPoints();
	
	//! Check for errors in this winding and abort if any
	void		check() const;
	
	plane_side_e	onPlaneSide(const cplane_c &p) const	{return onPlaneSide(p._normal, p._dist);}
	plane_side_e	onPlaneSide(const vec3_c &normal, vec_t dist) const;
	
	const std::vector<vec3_c>&	getPoints() const	{return _p;}
	
	//! Print to console
	void		print() const;
	
//	winding_c*	CopyWinding(winding_c *w);

private:
	std::vector<vec3_c>	_p;
};
typedef winding_c*			winding_p;
typedef std::vector<winding_p>		winding_v;
typedef winding_v::iterator		winding_i;
typedef winding_v::const_iterator	winding_ci;



class map_shader_c
{
public:
	map_shader_c(const std::string &name);

	inline const char*	getName() const			{return _name.c_str();}
	//TODO
	
private:
	std::string	_name;
	
	//TODO surface flags
	
	//TODO content flags
};

class map_brushside_c
{
public:
	inline map_brushside_c()
	{
		_plane		= NULL;
		
		_tex_mat.identity();
		
		_winding	= NULL;
		
		_shader		= NULL;
		
		_visible	= false;
		_tested		= false;
		_bevel		= false;
		_culled		= false;
	}
	
	void			translate(const vec3_c& v);

	void			setPlane(cplane_c *p)		{_plane = p;}
	const cplane_c*		getPlane() const 		{return _plane;}
	
	void			setWinding(winding_p w)		{_winding = w;}
	winding_p		getWinding() const		{return _winding;}
	
	void			setShader(const std::string& s);
	
	bool			isVisible() const		{return _visible;}
	void			isVisible(bool b)		{_visible = b;}
	
	bool			isTested() const		{return _tested;}
	void			isTested(bool b)		{_tested = b;}
	
	bool			isBevel() const			{return _bevel;}
	void			isBevel(bool b)			{_bevel = b;}
	
	bool			isCulled() const		{return _culled;}
	void			isCulled(bool b)		{_culled = b;}

private:
//	vec3_c			_plane_pts[3];
//	cplane_c		_plane_equation;
	cplane_c*		_plane;
	
	matrix_c		_tex_mat;
	
	winding_c*		_winding;
//	side_t*			_original;		// bspbrush_t sides will reference the mapbrush_t sides
	
	map_shader_c*		_shader;
	
	bool			_visible;		// choose visble planes first
	bool			_tested;		// this plane allready checked as a split
	bool			_bevel;			// don't ever use for bsp splitting
	bool			_culled;
	
	//TODO content flags
	
	//TODO compile flags
};

//typedef boost::shared_ptr<map_brushside_c>	map_brushside_p;
typedef map_brushside_c*			map_brushside_p;
typedef std::vector<map_brushside_p>		map_brushside_v;
typedef map_brushside_v::iterator		map_brushside_i;
typedef map_brushside_v::const_iterator		map_brushside_ci;


class map_brush_c
{
public:
	inline map_brush_c(int entity_num)
	{
		_entity_num	= entity_num;
	}
	
	void			addSide(map_brushside_p side)	{_sides.push_back(side);}
	void			translate(const vec3_c &v);
	//! Returns false if the brush doesn't enclose a valid volume.
	bool			createWindings();
	bool			calcAABB();

	int			getEntityNum() const	{return _entity_num;}
	const aabb_c&		getAABB() const		{return _aabb;}
	
private:
	int			_entity_num;
	int			_brush_num;

	int			_contents;
	
	aabb_c			_aabb;

	map_brushside_v		_sides;
};
//typedef boost::shared_ptr<map_brush_c>	map_brush_p;
typedef map_brush_c*			map_brush_p;
typedef std::vector<map_brush_p>	map_brush_v;
typedef map_brush_v::iterator		map_brush_i;
typedef map_brush_v::const_iterator	map_brush_ci;


class map_patch_c
{
public:
	//TODO
};


class map_entity_c
{
	friend void	UnparseEntities();
public:
	void		finish();
	void		adjustBrushesForOrigin();

	void		addBrush(map_brush_p b)		{_brushes.push_back(b);}
	const map_brush_v&	getBrushes() const	{return _brushes;}

	void 		setKeyValue(const std::string &key, const std::string &value);
	// will return "" if not present
	const char*	getValueForKey(const std::string &key) const;
	vec_t		getFloatForKey(const std::string &key) const;
	void 		getVector3ForKey(const std::string &key, vec3_c &v) const;
	void		hasKey(const std::string &key) const;
	
	void		print() const;

private:
	vec3_c					_origin;
	
	map_brush_v				_brushes;
	std::vector<map_patch_c*>		_patches;
	
	std::map<std::string, std::string>	_epairs;

	// only valid for func_areaportals
	int					_areaportalnum;
	int					_portalareas[2];
};

/*
#define	PLANENUM_LEAF		-1

#define	MAXEDGES		20

class face_t
{
public:
	face_t*			next;		// on node

	// the chain of faces off of a node can be merged or split,
	// but each face_t along the way will remain in the chain
	// until the entire tree is freed
	face_t*			merged;	// if set, this face isn't valid anymore
	face_t*			split[2];	// if set, this face isn't valid anymore

	portal_t*		portal;
	int			texinfo;
	int			planenum;
	int			contents;	// faces in different contents can't merge
	int			outputnumber;
	winding_c*		w;
	int			numpoints;
	bool			badstartvert;	// tjunctions cannot be fixed without a midpoint vertex
	int			vertexnums[MAXEDGES];
};



class bspbrush_t
{
public:
	bspbrush_t*		next;
	
	vec3_t			mins, maxs;
	int			side, testside;		// side of node during construction
	map_brush_p		original;
	
	map_brushside_v		sides;			// variably sized
};



#define	MAX_NODE_BRUSHES	8
class node_t
{
public:
	// both leafs and nodes
	int			planenum;	// -1 = leaf node
	node_t*			parent;
	vec3_t			mins, maxs;	// valid after portalization
	bspbrush_t*		volume;		// one for each leaf/node

	// nodes only
	bool			detail_seperator;	// a detail brush caused the split
	map_brushside_p		side;			// the side that created the node
	node_t*			children[2];
	face_t*			faces;

	// leafs only
	bspbrush_t*		brushlist;	// fragments of all brushes in this leaf
	int			contents;	// OR of all brush contents
	int			occupied;	// 1 or greater can reach entity
	map_entity_c*		occupant;	// for leak file testing
	int			cluster;	// for portalfile writing
	int			area;		// for areaportals
	portal_t*		portals;	// also on nodes during construction
};

class portal_t
{
public:
	cplane_c		plane;
	node_t*			onnode;		// NULL = outside box
	node_t*			nodes[2];		// [0] = front side of plane
	portal_t*		next[2];
	winding_c*		winding;

	bool			sidefound;		// false if ->side hasn't been checked
	map_brushside_p		side;			// NULL = non-visible
	face_t*			face[2];		// output face in bsp file
};

class tree_t
{
public:
	node_t*			headnode;
	node_t			outside_node;
	vec3_t			mins, maxs;
};

*/

extern std::vector<map_entity_c*>	map_entities;
extern map_brush_v			map_brushes;
extern map_brushside_v			map_brushsides;
extern std::vector<cplane_c*>		map_planes;


//
// map_bspfile.cxx
//
extern std::vector<bsp_dentdata_t>	dentdata;

extern std::vector<bsp_dshader_t>	dshaders;

extern std::vector<bsp_dplane_t>	dplanes;

extern std::vector<bsp_dnode_t>		dnodes;

extern std::vector<bsp_dleaf_t>		dleafs;

extern std::vector<bsp_dleafsurface_t>	dleafsurfaces;

extern std::vector<bsp_dleafbrush_t>	dleafbrushes;

extern std::vector<bsp_dmodel_t>	dmodels;

extern std::vector<bsp_dbrush_t>	dbrushes;

extern std::vector<bsp_dbrushside_t>	dbrushsides;

extern std::vector<bsp_dvertex_t>	dvertexes;

extern std::vector<bsp_dindex_t>	dindexes;

extern std::vector<bsp_dsurface_t>	dsurfaces;

extern std::vector<bsp_dvisdata_t>	dvisdata;

//extern	int		dareas_num;
//extern	bsp_darea_t	dareas[MAX_BSP_AREAS];

//extern int			dareaportals_num;
//extern bsp_dareaportal_t	dareaportals[MAX_BSP_AREAPORTALS];




//extern byte			dpop[256];

//void	DecompressVis(byte *in, byte *decompressed);
//int	CompressVis(byte *vis, byte *dest);


void		LoadBSPFile(const std::string &filename);
void		WriteBSPFile(const std::string &filename);
void		PrintBSPFileSizes();

//
// map_entity.cxx
//
//void		ParseEntities();
void		UnparseEntities();


//
// map_map.cxx
//
void		LoadMapFile(const std::string &filename);


//
// map_shader.cxx
//
void		Map_InitShaders();
void		Map_ShutdownShaders();

map_shader_c*	Map_FindShader(const std::string &name);

void		Map_ShaderList_f();
void		Map_ShaderCacheList_f();
void		Map_ShaderSearch_f();


#endif // MAP_LOCAL_H
