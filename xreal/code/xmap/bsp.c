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


char            source[1024];
char            tempsource[1024];
char            name[1024];

vec_t           microvolume = 1.0;
qboolean        glview;
qboolean        nodetail;
qboolean        fulldetail;
qboolean        onlyents;
qboolean        onlytextures;
qboolean        nowater;
qboolean        nofill;
qboolean        noopt;
qboolean        leaktest;
qboolean        verboseentities;
qboolean        noCurveBrushes;
qboolean        fakemap;
qboolean        notjunc;
qboolean        nomerge;
qboolean        nofog;
qboolean        nosubdivide;
qboolean        testExpand;
qboolean        showseams;

char            outbase[32];

/*
============
ProcessWorldModel
============
*/
void ProcessWorldModel(void)
{
	entity_t       *e;
	tree_t         *tree;
	bspFace_t      *faces;
	qboolean        leaked;
	
	e = &entities[0];
	e->firstDrawSurf = 0;		//numMapDrawSurfs;

	BeginModel(e);

	// check for patches with adjacent edges that need to LOD together
	PatchMapDrawSurfs(e);

	// build an initial bsp tree using all of the sides
	// of all of the structural brushes
	faces = MakeStructuralBspFaceList(entities[0].brushes);
	tree = FaceBSP(faces);
	MakeTreePortals(tree);
	FilterStructuralBrushesIntoTree(e, tree);

	// see if the bsp is completely enclosed
	if(FloodEntities(tree))
	{
		// rebuild a better bsp tree using only the
		// sides that are visible from the inside
		FillOutside(tree->headnode);

		// chop the sides to the convex hull of
		// their visible fragments, giving us the smallest
		// polygons 
		ClipSidesIntoTree(e, tree);

		faces = MakeVisibleBspFaceList(entities[0].brushes);
		FreeTree(tree);
		tree = FaceBSP(faces);
		MakeTreePortals(tree);
		FilterStructuralBrushesIntoTree(e, tree);
		leaked = qfalse;
	}
	else
	{
		Sys_Printf("**********************\n");
		Sys_Printf("******* leaked *******\n");
		Sys_Printf("**********************\n");
		LeakFile(tree);
		if(leaktest)
		{
			Sys_Printf("--- MAP LEAKED, ABORTING LEAKTEST ---\n");
			exit(0);
		}
		leaked = qtrue;

		// chop the sides to the convex hull of
		// their visible fragments, giving us the smallest
		// polygons 
		ClipSidesIntoTree(e, tree);
	}

	// save out information for visibility processing
	NumberClusters(tree);
	
	if(!leaked)
	{
		WritePortalFile(tree);
	}
	
	if(drawFlag)
	{
		// draw portals in new window
		DrawTree(tree);
	}
	
	if(glview)
	{
		// dump the portals for debugging
		WriteGLView(tree, source);
	}
	FloodAreas(tree);

	// add references to the detail brushes
	FilterDetailBrushesIntoTree(e, tree);
	
	// create drawsurfs for triangle models
	AddTriangleModels();

	// drawsurfs that cross fog boundaries will need to
	// be split along the bound
	if(!nofog)
	{
		FogDrawSurfs();			// may fragment drawsurfs
	}

	// subdivide each drawsurf as required by shader tesselation
	if(!nosubdivide)
	{
		SubdivideDrawSurfs(e, tree);
	}

	// merge together all common shaders on the same plane and remove 
	// all colinear points, so extra tjunctions won't be generated
	if(!nomerge)
	{
		MergeSides(e, tree);	// !@# testing
	}

	// add in any vertexes required to fix tjunctions
	if(!notjunc)
	{
		FixTJunctions(e);
	}

	// allocate lightmaps for faces and patches
	AllocateLightmaps(e);

	// add references to the final drawsurfs in the apropriate clusters
	FilterDrawsurfsIntoTree(e, tree);

	EndModel(e, tree->headnode);

	FreeTree(tree);
}

/*
============
ProcessSubModel
============
*/
void ProcessSubModel(int entityNum)
{
	entity_t       *e;
	tree_t         *tree;
	bspBrush_t     *b, *bc;
	node_t         *node;
	
	e = &entities[entityNum];
	e->firstDrawSurf = numMapDrawSurfs;

	BeginModel(e);

	PatchMapDrawSurfs(e);

	// just put all the brushes in an empty leaf
	// FIXME: patches?
	node = AllocNode();
	node->planenum = PLANENUM_LEAF;
	for(b = e->brushes; b; b = b->next)
	{
		bc = CopyBrush(b);
		bc->next = node->brushlist;
		node->brushlist = bc;
	}

	tree = AllocTree();
	tree->headnode = node;

	ClipSidesIntoTree(e, tree);
	
	// create drawsurfs for triangle models
	AddTriangleModel(e, qfalse);

	// subdivide each drawsurf as required by shader tesselation or fog
	if(!nosubdivide)
	{
		SubdivideDrawSurfs(e, tree);
	}

	// merge together all common shaders on the same plane and remove 
	// all colinear points, so extra tjunctions won't be generated
	if(!nomerge)
	{
		MergeSides(e, tree);	// !@# testing
	}

	// add in any vertexes required to fix tjunctions
	if(!notjunc)
	{
		FixTJunctions(e);
	}

	// allocate lightmaps for faces and patches
	AllocateLightmaps(e);

	// add references to the final drawsurfs in the apropriate clusters
	FilterDrawsurfsIntoTree(e, tree);

	EndModel(e, node);

	FreeTree(tree);
}


/*
============
ProcessModels
============
*/
void ProcessModels(void)
{
	int             i;
	entity_t       *entity;
	const char     *classname;
	const char     *model;

	BeginBSPFile();

	for(i = 0; i < numEntities; i++)
	{
		entity = &entities[i];
		
		classname = ValueForKey(entity, "classname");
		model = ValueForKey(entity, "model");
		
		if(entity->brushes || entity->patches || (!entity->brushes && !entity->patches && model[0] != '\0' && Q_stricmp("misc_model", classname)))
		{
			Sys_FPrintf(SYS_VRB, "############### model %i ###############\n", numModels);
			
			if(i == 0)
				ProcessWorldModel();
			else
				ProcessSubModel(i);
		}
	}
}

/*
============
Bspinfo
============
*/
void Bspinfo(int count, char **fileNames)
{
	int             i;
	char            source[1024];
	int             size;
	FILE           *f;

	if(count < 1)
	{
		Sys_Printf("No files to dump info for.\n");
		return;
	}

	for(i = 0; i < count; i++)
	{
		Sys_Printf("---------------------\n");
		strcpy(source, fileNames[i]);
		DefaultExtension(source, ".bsp");
		f = fopen(source, "rb");
		if(f)
		{
			size = Q_filelength(f);
			fclose(f);
		}
		else
			size = 0;
		Sys_Printf("%s: %i\n", source, size);

		LoadBSPFile(source);
		PrintBSPFileSizes();
		Sys_Printf("---------------------\n");
	}
}


/*
============
OnlyEnts
============
*/
void OnlyEnts(void)
{
	char            out[1024];

	sprintf(out, "%s.bsp", source);
	LoadBSPFile(out);
	numEntities = 0;

	LoadMapFile(name);
	SetModelNumbers();

	UnparseEntities();

	WriteBSPFile(out);
}


/*
============
OnlyTextures
============
*/
void OnlyTextures(void)
{								// FIXME!!!
	char            out[1024];
	int             i;

	Error("-onlytextures isn't working now...");

	sprintf(out, "%s.bsp", source);

	LoadMapFile(name);

	LoadBSPFile(out);

	// replace all the drawsurface shader names
	for(i = 0; i < numDrawSurfaces; i++)
	{
	}

	WriteBSPFile(out);
}


int BspMain(int argc, char **argv)
{
	int             i;
	double          start, end;
	char            path[1024];

	Sys_Printf("---- bsp ----\n");

	tempsource[0] = '\0';

	for(i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-tempname"))
		{
			strcpy(tempsource, argv[++i]);
		}
		else if(!strcmp(argv[i], "-threads"))
		{
			numthreads = atoi(argv[i + 1]);
			i++;
		}
		else if(!strcmp(argv[i], "-glview"))
		{
			glview = qtrue;
		}
		else if(!strcmp(argv[i], "-v"))
		{
			Sys_Printf("verbose = true\n");
			verbose = qtrue;
		}
		else if(!strcmp(argv[i], "-draw"))
		{
			Sys_Printf("drawflag = true\n");
			drawFlag = qtrue;
		}
		else if(!strcmp(argv[i], "-nowater"))
		{
			Sys_Printf("nowater = true\n");
			nowater = qtrue;
		}
		else if(!strcmp(argv[i], "-noopt"))
		{
			Sys_Printf("noopt = true\n");
			noopt = qtrue;
		}
		else if(!strcmp(argv[i], "-nofill"))
		{
			Sys_Printf("nofill = true\n");
			nofill = qtrue;
		}
		else if(!strcmp(argv[i], "-nodetail"))
		{
			Sys_Printf("nodetail = true\n");
			nodetail = qtrue;
		}
		else if(!strcmp(argv[i], "-fulldetail"))
		{
			Sys_Printf("fulldetail = true\n");
			fulldetail = qtrue;
		}
		else if(!strcmp(argv[i], "-onlyents"))
		{
			Sys_Printf("onlyents = true\n");
			onlyents = qtrue;
		}
		else if(!strcmp(argv[i], "-onlytextures"))
		{
			Sys_Printf("onlytextures = true\n");	// FIXME: make work again!
			onlytextures = qtrue;
		}
		else if(!strcmp(argv[i], "-micro"))
		{
			microvolume = atof(argv[i + 1]);
			Sys_Printf("microvolume = %f\n", microvolume);
			i++;
		}
		else if(!strcmp(argv[i], "-nofog"))
		{
			Sys_Printf("nofog = true\n");
			nofog = qtrue;
		}
		else if(!strcmp(argv[i], "-nosubdivide"))
		{
			Sys_Printf("nosubdivide = true\n");
			nosubdivide = qtrue;
		}
		else if(!strcmp(argv[i], "-leaktest"))
		{
			Sys_Printf("leaktest = true\n");
			leaktest = qtrue;
		}
		else if(!strcmp(argv[i], "-verboseentities"))
		{
			Sys_Printf("verboseentities = true\n");
			verboseentities = qtrue;
		}
		else if(!strcmp(argv[i], "-nocurves"))
		{
			noCurveBrushes = qtrue;
			Sys_Printf("no curve brushes\n");
		}
		else if(!strcmp(argv[i], "-notjunc"))
		{
			notjunc = qtrue;
			Sys_Printf("no tjunction fixing\n");
		}
		else if(!strcmp(argv[i], "-expand"))
		{
			testExpand = qtrue;
			Sys_Printf("Writing expanded.map.\n");
		}
		else if(!strcmp(argv[i], "-showseams"))
		{
			showseams = qtrue;
			Sys_Printf("Showing seams on terrain.\n");
		}
		else if(!strcmp(argv[i], "-tmpout"))
		{
			strcpy(outbase, "/tmp");
		}
		else if(!strcmp(argv[i], "-fakemap"))
		{
			fakemap = qtrue;
			Sys_Printf("will generate fakemap.map\n");
		}
		else if(!strcmp(argv[i], "-samplesize"))
		{
			samplesize = atoi(argv[i + 1]);
			if(samplesize < 1)
				samplesize = 1;
			i++;
			Sys_Printf("lightmap sample size is %dx%d units\n", samplesize, samplesize);
		}
		else if(!strcmp(argv[i], "-connect"))
		{
			Broadcast_Setup(argv[++i]);
		}
		else if(argv[i][0] == '-')
			Error("Unknown option \"%s\"", argv[i]);
		else
			break;
	}
	
	if(i != argc - 1)
	{
		Sys_Printf("usage: xmap -map2bsp [-<switch> [-<switch> ...]] <mapname.map>\n"
				"\n"
				"Switches:\n"
				"   v              = verbose output\n"
				"   threads <X>    = set number of threads to X\n"
				"   nocurves       = don't emit bezier surfaces\n"
				//"   optimize       = enable optimization\n"
				//"   breadthfirst   = breadth first bsp building\n"
				//"   nobrushmerge   = don't merge brushes\n"
				//"   noliquids                            = don't write liquids to map\n"
				//"   freetree                             = free the bsp tree\n"
				//"   nocsg                                = disables brush chopping\n"
				//"   forcesidesvisible                    = force all sides to be visible\n"
				//"   grapplereach                         = calculate grapple reachabilities\n"
				//"   glview     = output a GL view\n"
				"   draw       = enables drawing\n"
				//"   noweld     = disables weld\n"
				//"   noshare    = disables sharing\n"
				"   notjunc    = disables juncs\n"
				"   nowater    = disables water brushes\n"
				//"   noprune    = disables node prunes\n"
				//"   nomerge    = disables face merging\n"
				//"   nosubdiv   = disables subdeviding\n"
				//"   nodetail   = disables detail brushes\n"
				//"   fulldetail = enables full detail\n"
				"   onlyents   = only compile entities with bsp\n"
				//"   micro <volume>\n"
				//"              = sets the micro volume to the given float\n"
				"   leaktest   = perform a leak test\n"
				//"   chop <subdivide_size>\n"
				//"              = sets the subdivide size to the given float\n"
				"   TODO list all options\n");
		exit(0);
	}

	start = I_FloatTime();

	ThreadSetDefault();
	//numthreads = 1;       // multiple threads aren't helping because of heavy malloc use
	SetQdirFromPath(argv[i]);

	strcpy(source, ExpandArg(argv[i]));
	StripExtension(source);

	// delete portal and line files
	sprintf(path, "%s.prt", source);
	remove(path);
	sprintf(path, "%s.lin", source);
	remove(path);

	strcpy(name, ExpandArg(argv[i]));
	if(strcmp(name + strlen(name) - 4, ".reg"))
	{
		// if we are doing a full map, delete the last saved region map
		sprintf(path, "%s.reg", source);
		remove(path);

		DefaultExtension(name, ".map");	// might be .reg
	}

	// if onlyents, just grab the entites and resave
	if(onlyents)
	{
		OnlyEnts();
		return 0;
	}

	// if onlytextures, just grab the textures and resave
	if(onlytextures)
	{
		OnlyTextures();
		return 0;
	}

	// start from scratch
	LoadShaderInfo();

	// load original file from temp spot in case it was renamed by the editor on the way in
	if(strlen(tempsource) > 0)
	{
		LoadMapFile(tempsource);
	}
	else
	{
		LoadMapFile(name);
	}
	
	ProcessModels();
	
	SetModelNumbers();

	EndBSPFile();

	end = I_FloatTime();
	Sys_Printf("%5.0f seconds elapsed\n", end - start);

	// remove temp name if appropriate
	if(strlen(tempsource) > 0)
	{
		remove(tempsource);
	}
	
	// shut down connection
	Broadcast_Shutdown();

	return 0;
}
