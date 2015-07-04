#include "StdAfx.h"

#include "vector2.h"
#include "triangulate.h"

#if 0
extern "C"
 {
 	// For windows
	#define ANSI_DECLARATORS
 
	#ifdef SINGLE
	#define REAL float
	#else /* not SINGLE */
	#define REAL double
	#endif /* not SINGLE */
 
	#include "triangle.h"
	};
#else
	#define ANSI_DECLARATORS

	#ifdef SINGLE
	#define REAL float
	#else /* not SINGLE */
	#define REAL double
	#endif /* not SINGLE */
	#include "triangle.h"
#endif

// Triangulate the polygon,
// Input : pts -- planar points
// Output: tris -- triangle index
void Triangulate_2d(std::vector<MATH::vector2d>& pts, std::vector<int>& tris)
{
	//////////////////////////////////////////////////////////////////////////
	//
	struct triangulateio in, out;

	/* Define input points. */
	in.numberofpoints = pts.size();
	in.pointlist = new REAL[in.numberofpoints * 2];
	int i = 0;
	for ( std::vector<MATH::vector2d>::const_iterator cit = pts.begin();
		cit != pts.end(); cit++, i++ )
	{
		in.pointlist[2*i] = cit->x;
		in.pointlist[2*i+1] = cit->y;
	}

	in.numberofpointattributes = 0;
	in.pointattributelist = NULL;

	in.pointmarkerlist = NULL;

	in.numberofsegments = in.numberofpoints;
	in.segmentlist = new int[in.numberofsegments*2];
	in.segmentmarkerlist = new int[in.numberofsegments];
	for (int i=0; i<in.numberofsegments; i++)
	{
		in.segmentlist[2*i] = i;
		in.segmentlist[2*i+1] = (i+1)%in.numberofpoints;

		in.segmentmarkerlist[i] = 0;
	}

	in.numberofholes = 0;
	in.numberofregions = 1;
	in.regionlist = new REAL[in.numberofregions * 4];
	in.regionlist[0] = 0;
	in.regionlist[1] = 0;
	in.regionlist[2] = 1.0; /* Regional attribute (for whole mesh). */
	in.regionlist[3] = 0.1; /* Area constraint that will not be used. */


	/* Make necessary initializations so that Triangle can return a */
	/* triangulation in `out'. */

	out.pointlist = (REAL *) NULL; /* Not needed if -N switch used. */
	/* Not needed if -N switch used or number of point attributes is zero: */
	out.pointattributelist = (REAL *) NULL;
	out.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	out.trianglelist = (int *) NULL; /* Not needed if -E switch used. */
	/* Not needed if -E switch used or number of triangle attributes is zero: */
	out.triangleattributelist = (REAL *) NULL;

	/* Needed only if segments are output (-p or -c) and -P not used: */
	out.segmentlist = (int *) NULL;
	/* Needed only if segments are output (-p or -c) and -P and -B not used: */
	out.segmentmarkerlist = (int *) NULL;

	//////////////////////////////////////////////////////////////////////////
	// Triangulate parameters

	/* Triangulate the points. Switches are chosen to read and write a */
	/* PSLG (p), preserve the convex hull (c), number everything from */
	/* zero (z), assign a regional attribute to each element (A), and */
	/* produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/* neighbor list (n), area (a). */

	double area = 3600; // area constraints

	char ch[100];
	sprintf_s(ch, "a%.3f", area);
	std::string p_area;
	//p_area.assign(ch);

	std::string para("pq25z");
	para = para + p_area;

	triangulate(const_cast<char*>(para.c_str()), &in, &out, NULL);

	pts.clear();
	for (int i=0; i<out.numberofpoints; i++)
	{
		pts.push_back(MATH::vector2d(out.pointlist[2*i], out.pointlist[2*i+1]));
	}

	tris.clear();
	for (int i=0; i<out.numberoftriangles; i++)
	{
		for (int j=0; j<3; j++)
			tris.push_back(out.trianglelist[3*i+j]);
	}

	//
	/* Free all allocated arrays, including those allocated by Triangle. */

	delete []in.pointlist;
	delete []in.segmentlist;
	delete []in.segmentmarkerlist;
	delete []in.regionlist;
	delete []out.pointlist;
	delete []out.pointattributelist;
	delete []out.pointmarkerlist;
	delete []out.trianglelist;
	delete []out.triangleattributelist;
	delete []out.segmentlist;
	delete []out.segmentmarkerlist;
} 

void Triangulate_2d(QVector<QPoint>& pts, QVector<int>& tris)
{
	//////////////////////////////////////////////////////////////////////////
	//
	struct triangulateio in, out;

	/* Define input points. */
	in.numberofpoints = pts.size();
	in.pointlist = new REAL[in.numberofpoints * 2];
	int i = 0;
	for ( QVector<QPoint>::const_iterator cit = pts.begin();
		cit != pts.end(); cit++, i++ )
	{
		in.pointlist[2*i] = cit->x();
		in.pointlist[2*i+1] = cit->y();
	}

	in.numberofpointattributes = 0;
	in.pointattributelist = NULL;

	in.pointmarkerlist = NULL;

	in.numberofsegments = in.numberofpoints;
	in.segmentlist = new int[in.numberofsegments*2];
	in.segmentmarkerlist = new int[in.numberofsegments];
	for (int i=0; i<in.numberofsegments; i++)
	{
		in.segmentlist[2*i] = i;
		in.segmentlist[2*i+1] = (i+1)%in.numberofpoints;

		in.segmentmarkerlist[i] = 0;
	}

	in.numberofholes = 0;
	in.numberofregions = 1;
	in.regionlist = new REAL[in.numberofregions * 4];
	in.regionlist[0] = 0;
	in.regionlist[1] = 0;
	in.regionlist[2] = 1.0; /* Regional attribute (for whole mesh). */
	in.regionlist[3] = 0.1; /* Area constraint that will not be used. */


	/* Make necessary initializations so that Triangle can return a */
	/* triangulation in `out'. */

	out.pointlist = (REAL *) NULL; /* Not needed if -N switch used. */
	/* Not needed if -N switch used or number of point attributes is zero: */
	out.pointattributelist = (REAL *) NULL;
	out.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	out.trianglelist = (int *) NULL; /* Not needed if -E switch used. */
	/* Not needed if -E switch used or number of triangle attributes is zero: */
	out.triangleattributelist = (REAL *) NULL;

	/* Needed only if segments are output (-p or -c) and -P not used: */
	out.segmentlist = (int *) NULL;
	/* Needed only if segments are output (-p or -c) and -P and -B not used: */
	out.segmentmarkerlist = (int *) NULL;

	//////////////////////////////////////////////////////////////////////////
	// Triangulate parameters

	/* Triangulate the points. Switches are chosen to read and write a */
	/* PSLG (p), preserve the convex hull (c), number everything from */
	/* zero (z), assign a regional attribute to each element (A), and */
	/* produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/* neighbor list (n), area (a). */

	double area = 3600; // area constraints

	char ch[100];
	sprintf_s(ch, "a%.3f", area);
	std::string p_area;
	//p_area.assign(ch);

	std::string para("pq25z");
	para = para + p_area;

	triangulate(const_cast<char*>(para.c_str()), &in, &out, NULL);

	pts.clear();
	for (int i=0; i<out.numberofpoints; i++)
	{
		pts.push_back(QPoint(out.pointlist[2*i], out.pointlist[2*i+1]));
	}

	tris.clear();
	for (int i=0; i<out.numberoftriangles; i++)
	{
		for (int j=0; j<3; j++)
			tris.push_back(out.trianglelist[3*i+j]);
	}

	//
	/* Free all allocated arrays, including those allocated by Triangle. */

	delete []in.pointlist;
	delete []in.segmentlist;
	delete []in.segmentmarkerlist;
	delete []in.regionlist;
	delete []out.pointlist;
	delete []out.pointattributelist;
	delete []out.pointmarkerlist;
	delete []out.trianglelist;
	delete []out.triangleattributelist;
	delete []out.segmentlist;
	delete []out.segmentmarkerlist;
} 
