/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2006 Robert Beckebans <trebor_7@users.sourceforge.net>

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
#include "qbsp.h"

/*
============
EmitShader
============
*/
int EmitShader(const char *shader)
{
	int             i;
	shaderInfo_t   *si;

	if(!shader)
	{
		shader = "noshader";
	}

	for(i = 0; i < numShaders; i++)
	{
		if(!Q_stricmp(shader, dshaders[i].shader))
		{
			return i;
		}
	}

	if(i == MAX_MAP_SHADERS)
	{
		Error("MAX_MAP_SHADERS");
	}
	numShaders++;
	strcpy(dshaders[i].shader, shader);

	si = ShaderInfoForShader(shader);
	dshaders[i].surfaceFlags = si->surfaceFlags;
	dshaders[i].contentFlags = si->contents;

	return i;
}


/*
============
EmitPlanes

There is no oportunity to discard planes, because all of the original
brushes will be saved in the map.
============
*/
void EmitPlanes(void)
{
	int             i;
	dplane_t       *dp;
	plane_t        *mp;

	mp = mapplanes;
	for(i = 0; i < nummapplanes; i++, mp++)
	{
		dp = &dplanes[numplanes];
		VectorCopy(mp->normal, dp->normal);
		dp->dist = mp->dist;
		numplanes++;
	}
}



/*
==================
EmitLeaf
==================
*/
void EmitLeaf(node_t * node)
{
	dleaf_t        *leaf_p;
	bspbrush_t     *b;
	drawSurfRef_t  *dsr;

	// emit a leaf
	if(numleafs >= MAX_MAP_LEAFS)
		Error("MAX_MAP_LEAFS");

	leaf_p = &dleafs[numleafs];
	numleafs++;

	leaf_p->cluster = node->cluster;
	leaf_p->area = node->area;

	//
	// write bounding box info
	//  
	VectorCopy(node->mins, leaf_p->mins);
	VectorCopy(node->maxs, leaf_p->maxs);

	//
	// write the leafbrushes
	//
	leaf_p->firstLeafBrush = numleafbrushes;
	for(b = node->brushlist; b; b = b->next)
	{
		if(numleafbrushes >= MAX_MAP_LEAFBRUSHES)
		{
			Error("MAX_MAP_LEAFBRUSHES");
		}
		dleafbrushes[numleafbrushes] = b->original->outputNumber;
		numleafbrushes++;
	}
	leaf_p->numLeafBrushes = numleafbrushes - leaf_p->firstLeafBrush;

	//
	// write the surfaces visible in this leaf
	//
	if(node->opaque)
	{
		return;					// no leaffaces in solids
	}

	// add the drawSurfRef_t drawsurfs
	leaf_p->firstLeafSurface = numleafsurfaces;
	for(dsr = node->drawSurfReferences; dsr; dsr = dsr->nextRef)
	{
		if(numleafsurfaces >= MAX_MAP_LEAFFACES)
			Error("MAX_MAP_LEAFFACES");
		dleafsurfaces[numleafsurfaces] = dsr->outputNumber;
		numleafsurfaces++;
	}


	leaf_p->numLeafSurfaces = numleafsurfaces - leaf_p->firstLeafSurface;
}


/*
============
EmitDrawNode_r
============
*/
int EmitDrawNode_r(node_t * node)
{
	dnode_t        *n;
	int             i;

	if(node->planenum == PLANENUM_LEAF)
	{
		EmitLeaf(node);
		return -numleafs;
	}

	// emit a node  
	if(numnodes == MAX_MAP_NODES)
		Error("MAX_MAP_NODES");
	n = &dnodes[numnodes];
	numnodes++;

	VectorCopy(node->mins, n->mins);
	VectorCopy(node->maxs, n->maxs);

	if(node->planenum & 1)
		Error("WriteDrawNodes_r: odd planenum");
	n->planeNum = node->planenum;

	//
	// recursively output the other nodes
	//  
	for(i = 0; i < 2; i++)
	{
		if(node->children[i]->planenum == PLANENUM_LEAF)
		{
			n->children[i] = -(numleafs + 1);
			EmitLeaf(node->children[i]);
		}
		else
		{
			n->children[i] = numnodes;
			EmitDrawNode_r(node->children[i]);
		}
	}

	return n - dnodes;
}

//=========================================================



/*
============
SetModelNumbers
============
*/
void SetModelNumbers(void)
{
	int             i;
	entity_t       *ent;
	int             models;
	char            value[10];
	const char     *model;

	models = 1;
	for(i = 1; i < num_entities; i++)
	{
		ent = &entities[i];
		
		model = ValueForKey(ent, "model");
		
		if(ent->brushes || ent->patches || (!ent->brushes && !ent->patches && model[0] != '\0'))
		{
			sprintf(value, "*%i", models);
			models++;
			SetKeyValue(&entities[i], "model", value);
		}
	}

}


//===========================================================

/*
==================
BeginBSPFile
==================
*/
void BeginBSPFile(void)
{
	// these values may actually be initialized
	// if the file existed when loaded, so clear them explicitly
	nummodels = 0;
	numnodes = 0;
	numbrushsides = 0;
	numleafsurfaces = 0;
	numleafbrushes = 0;

	// leave leaf 0 as an error, because leafs are referenced as
	// negative number nodes
	numleafs = 1;
}


/*
============
EndBSPFile
============
*/
void EndBSPFile(void)
{
	char            path[1024];

	EmitPlanes();
	UnparseEntities();

	// write the map
	sprintf(path, "%s.bsp", source);
	_printf("Writing %s\n", path);
	WriteBSPFile(path);
}


//===========================================================

/*
============
EmitBrushes
============
*/
void EmitBrushes(bspbrush_t * brushes)
{
	int             j;
	dbrush_t       *db;
	bspbrush_t     *b;
	dbrushside_t   *cp;

	for(b = brushes; b; b = b->next)
	{
		if(numbrushes == MAX_MAP_BRUSHES)
		{
			Error("MAX_MAP_BRUSHES");
		}
		b->outputNumber = numbrushes;
		db = &dbrushes[numbrushes];
		numbrushes++;

		db->shaderNum = EmitShader(b->contentShader->shader);
		db->firstSide = numbrushsides;

		// don't emit any generated backSide sides
		db->numSides = 0;
		for(j = 0; j < b->numsides; j++)
		{
			if(b->sides[j].backSide)
			{
				continue;
			}
			if(numbrushsides == MAX_MAP_BRUSHSIDES)
			{
				Error("MAX_MAP_BRUSHSIDES ");
			}
			cp = &dbrushsides[numbrushsides];
			db->numSides++;
			numbrushsides++;
			cp->planeNum = b->sides[j].planenum;
			cp->shaderNum = EmitShader(b->sides[j].shaderInfo->shader);
		}
	}

}


/*
==================
BeginModel
==================
*/
void BeginModel(entity_t * e)
{
	dmodel_t       *mod;
	
	qprintf("--- BeginModel ---\n");

	if(nummodels == MAX_MAP_MODELS)
	{
		Error("MAX_MAP_MODELS");
	}
	mod = &dmodels[nummodels];

	mod->firstSurface = numDrawSurfaces;
	mod->firstBrush = numbrushes;
	
	EmitBrushes(e->brushes);
}




/*
==================
EndModel
==================
*/
void EndModel(entity_t * e, node_t * headnode)
{
	dmodel_t       *mod;
	vec3_t          mins, maxs;
	bspbrush_t     *b;
	parseMesh_t    *p;
	mapDrawSurface_t *ds;
	const char     *model;
	int             i, j;

	qprintf("--- EndModel ---\n");

	mod = &dmodels[nummodels];
	
	// calculate the AABB
	ClearBounds(mins, maxs);
	for(b = e->brushes; b; b = b->next)
	{
		if(!b->numsides)
		{
			continue;			// not a real brush (origin brush, etc)
		}
		AddPointToBounds(b->mins, mins, maxs);
		AddPointToBounds(b->maxs, mins, maxs);
	}

	for(p = e->patches; p; p = p->next)
	{
		for(i = 0; i < p->mesh.width * p->mesh.height; i++)
		{
			AddPointToBounds(p->mesh.verts[i].xyz, mins, maxs);
		}
	}

	model = ValueForKey(e, "model");
	if(!e->brushes && !e->patches && model[0] != '\0')
	{
		//qprintf("calculating bbox from draw surfaces...\n");
		
		for(i = e->firstDrawSurf; i < numMapDrawSurfs; i++)
		{
			ds = &mapDrawSurfs[i];
			
			if(!ds->numVerts)
			{
				continue;			// leftover from a surface subdivision
			}
			
			for(j = 0; j < ds->numVerts; j++)
			{
				AddPointToBounds(ds->verts[j].xyz, mins, maxs);
			}
		}
	}

	VectorCopy(mins, mod->mins);
	VectorCopy(maxs, mod->maxs);
	
	EmitDrawNode_r(headnode);
	mod->numSurfaces = numDrawSurfaces - mod->firstSurface;
	mod->numBrushes = numbrushes - mod->firstBrush;

	nummodels++;
}
