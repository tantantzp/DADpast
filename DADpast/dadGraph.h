#ifndef DADGRAPH
#define DADGRAPH
   
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "qimage.h"
#include "qmessagebox.h"
#include <vector>
#include "Node.h"
#include "myVector.h"

using namespace std;
using namespace cv;

class DADPath
{
public:
	vector<CvPoint> Path;
	double energy;
	double totalDif;
	double K;
	DADPath(){
	    energy = 0;
		totalDif = 0;
	}
	double getK(){
		double size= Path.size();
		K = totalDif / size;
		return K;
	}
};


class DADGraph
{
public:
	GNode** graph;

	IplImage* isrc;
	IplImage* itarget;

	IplImage* isrcMask;
	IplImage* itargetMask;
    IplImage* itrimap;
	CvPoint maskup, maskdown;

	CvPoint cutLineP1, cutLineP2;
	CvPoint startP1, startP2;
	CvPoint endP1, endP2;

	int gheight, gwidth;

	vector<CvPoint> bestPath;
	vector<CvPoint> startPoint;
	vector<CvPoint> endPoint;

public:
	DADGraph(IplImage* _isrc, IplImage* _itar, IplImage* itri)
	{
		isrc = _isrc;
		itarget= _itar;
		itrimap = itri;
		getMask();
		gheight = abs(maskdown.x - maskup.x);
		gwidth = abs(maskdown.y - maskup.y);

		initGraph();
	}

	void getMask(){
		isrcMask = cvCreateImage(cvSize(isrc->width, isrc->height), IPL_DEPTH_8U, isrc->nChannels);
		itargetMask = cvCreateImage(cvSize(itarget->width, itarget->height), IPL_DEPTH_8U, itarget->nChannels);
	
		int width = itrimap->width ;
		int height = itrimap->height ;

		int mleft = itrimap->width;
		int mright = 0;
		int mtop = height;
		int mbutton = 0;

		for( int i = 0; i < height /*&& i < y_max*/; i++){
			for(int j = 0; j < width /*&& j < x_max*/; j++){
				if( CV_IMAGE_ELEM(itrimap,char,i,j) == 100){
					if(i < mtop) mtop = i;
					if(i > mbutton) mbutton = i;
					if(j < mleft) mleft = j;
					if(j > mright) mright = j;

					CV_IMAGE_ELEM(isrcMask, char, i, 3*j) = CV_IMAGE_ELEM(isrc, char, i, 3*j);
					CV_IMAGE_ELEM(isrcMask, char, i, 3*j+1) = CV_IMAGE_ELEM(isrc, char, i, 3*j+1);
					CV_IMAGE_ELEM(isrcMask, char, i, 3*j+2) = CV_IMAGE_ELEM(isrc, char, i, 3*j+2);
			    
					CV_IMAGE_ELEM(itargetMask, char, i, 3*j) = CV_IMAGE_ELEM(itarget, char, i, 3*j);
					CV_IMAGE_ELEM(itargetMask, char, i, 3*j+1) = CV_IMAGE_ELEM(itarget, char, i, 3*j+1);
					CV_IMAGE_ELEM(itargetMask, char, i, 3*j+2) = CV_IMAGE_ELEM(itarget, char, i, 3*j+2);
				}
				else
				{
					CV_IMAGE_ELEM(isrcMask, char, i, 3*j) = 0;
					CV_IMAGE_ELEM(isrcMask, char, i, 3*j+1) = 0;
					CV_IMAGE_ELEM(isrcMask, char, i, 3*j+2) = 0;
			    
					CV_IMAGE_ELEM(itargetMask, char, i, 3*j) = 0;
					CV_IMAGE_ELEM(itargetMask, char, i, 3*j+1) = 0;
					CV_IMAGE_ELEM(itargetMask, char, i, 3*j+2) = 0;
				}
			}
		}
		maskup.x = mtop, maskup.y = mleft;
		maskdown.x = mbutton, maskdown.y = mright;

    }


	void  initGraph()
	{
		graph = new GNode*[gheight];
		for(int i = 0; i < gheight; i++){
			graph[i] = new GNode[gwidth];
    	}

		for(int i = 0; i < gheight; i++){
			for(int j = 0; j < gwidth; j++){
				int x = i + maskup.x;
				int y = j + maskup.y;
				if( CV_IMAGE_ELEM(itrimap,char,x,y) == 100){
					graph[i][j].set(isrcMask, itargetMask, x, y, true);
				}
				else
				{
					graph[i][j].set(NULL, NULL, x, y, false);
				}
			}
		}
		
		int mid = (maskup.x + maskdown.x) / 2;
		int i = mid;
		int j;
		getEdge(i, cutLineP1, cutLineP2);
		getEdge(i - 1, startP1, startP2);
		getEdge(i + 1, endP1, endP2);

		cout << cutLineP1.x <<' '<<cutLineP1.y << ',' <<cutLineP2.x<<' '<<cutLineP2.y<<endl;
		cout << startP1.x <<' '<<startP1.y << ',' <<startP2.x<<' '<<startP2.y<<endl;
		cout << endP1.x <<' '<< endP1.y << ',' <<endP2.x<<' '<<endP2.y<<endl;

		for(j =  cutLineP1.y; j <  cutLineP2.y; j++)
		{
			CV_IMAGE_ELEM(itrimap, char, i, j) = 0;
            int x = i - maskup.x, y = j - maskup.y;
			graph[x][y].inmask = false;
		}
		i = startP1.x;
		for(j = startP1.y; j < startP2.y - 1; j++)
		{
			CvPoint a;
			a.x = i - maskup.x, a.y = j - maskup.y;
			startPoint.push_back(a);
		}
		i = endP1.x;
		for(j = endP1.y; j < endP2.y - 1; j++)
		{
			CvPoint a;
			a.x = i - maskup.x, a.y = j - maskup.y;
			endPoint.push_back(a);
		}

    }

	DADPath getPath()
	{
		double K = 0.0;
		double minEnergy = INT_MAX;
		DADPath  minPath;
		minPath.energy = INT_MAX - 100;
		cout << "getPath"<< endl;
		while(minPath.energy < minEnergy)
		{
			minEnergy = minPath.energy;
			cout <<"minEnergy "<< minEnergy<<endl;
			for(int i = 0; i < startPoint.size(); i ++)
			{
				for(int i = 0; i < gheight; i++){
					for(int j = 0; j < gwidth; j++){
						graph[i][j].length = 0;
						graph[i][j].visited = false;
						graph[i][j].expanded = false;
						graph[i][j].parent = NULL;
					}
				}
				DADPath path = dijkstra(startPoint[i], K);
				if(path.energy < minPath.energy)
				{
					minPath.Path = path.Path;
					minPath.energy = path.energy;
					minPath.totalDif = path.totalDif;
					cout <<"get a shorter path:"<< minPath.energy <<" at K :" << K << endl;
				}

			}
            K = minPath.getK();

		}
		cout <<"get the path with min energy" << endl;
        return minPath;
	}

	DADPath dijkstra(CvPoint start, double K)
	{
	    myVector<GNode*> node_vec;
		graph[start.x][start.y].length = (graph[start.x][start.y].difference - K)*(graph[start.x][start.y].difference - K);
		
		node_vec.insert(&graph[start.x][start.y]);

		while(node_vec.size > 0)
		{
			//cout <<"size:"<< node_vec.size << endl;
			GNode* minNode = node_vec[0];
			int minI = 0;
			for(int i = 1; i < node_vec.size; i ++)
			{
				if(node_vec[i]->length < minNode->length)
				{
				    minNode = node_vec[i];
					minI = i;
				}
			}
			int x = minNode->point.x - maskup.x;
			int y = minNode->point.y - maskup.y;
			graph[x][y].expanded = true;

			node_vec.remove(minI);
			
			int dir[4][2] = { {-1, 0}, {0, -1}, {1,0}, {0,1}};
		    for(int i = 0; i < 4; i ++)
			{
				int nx = x + dir[i][0];
				int ny = y + dir[i][1];
				if(nx > 0  && nx < gheight && y > 0 && y < gwidth)
				if(graph[nx][ny].inmask == true && graph[nx][ny].expanded == false)
				{
					if(graph[nx][ny].visited == false)
					{
						graph[nx][ny].length = graph[x][y].length +  (graph[nx][ny].difference - K)*(graph[nx][ny].difference - K);
						graph[nx][ny].parent = &graph[x][y];
						graph[nx][ny].visited = true;
                        node_vec.insert(&graph[nx][ny]);
					}
					else{
						double nlen = graph[x][y].length + (graph[nx][ny].difference - K)*(graph[nx][ny].difference - K);
						if(nlen < graph[nx][ny].length)
						{
							graph[nx][ny].length = nlen;
							graph[nx][ny].parent = &graph[x][y];
						}
					}
				}
			}
		}

		int minI = 0;
		double minLength = DBL_MAX;
		for(int i = 0; i < endPoint.size(); i ++)
		{
			if(graph[endPoint[i].x][endPoint[i].y].length > 0)
			{
				double lenth = graph[endPoint[i].x][endPoint[i].y].length;
				if(lenth < minLength)
				{
					minI = i;
					minLength = lenth;
				}
			}

		}
		DADPath path;
		GNode* pathNode = &graph[endPoint[minI].x][endPoint[minI].y];
		
		path.totalDif += pathNode -> difference;
		path.Path.push_back(pathNode->point);
		path.energy = pathNode->length ;

		while(pathNode -> parent != NULL)
		{
			pathNode = pathNode->parent;
			path.totalDif += pathNode -> difference;
            path.Path.push_back(pathNode->point);
		}

		int cutX = cutLineP1.x;
		int maxY, minY;
		int startY = path.Path[path.Path.size() - 1].y;
		int endY = path.Path[0].y;
		if(startY > endY) {
		    maxY = startY;
			minY = endY;
		}
		else
		{
		    maxY = endY;
			minY = startY;
		}
		for(int y = minY; y < maxY; y ++)
		{
             CvPoint tPoint;
		     tPoint.x = cutX;
			 tPoint.y = y;
			 path.totalDif += graph[cutX - maskup.x][y - maskup.y].difference;
             path.Path.push_back(tPoint);
		}

		return path;
	}


	void showGraph()
	{
		for(int i = 0; i < gheight; i++){
			for(int j = 0; j < gwidth; j++){
				cout << graph[i][j].difference << ' '; 
			}
			cout << endl;
		}
		
		

    }





	void getEdge(int i, CvPoint& point1, CvPoint& point2)
	{
		int j;
		bool inmask = false;
		int change = 0;
		int width = itrimap->width;

		point1.x = i, point1.y = 0;
		point2.x = i, point2.y = 0;

		for(j = 0; j < width; j++){		
			 if(!inmask && CV_IMAGE_ELEM(itrimap,char,i,j) == 100){
				 inmask = true;
				 change++;
				 if(change == 3){
					 point1.y = j;
				 }
			 }
			 else if(inmask &&  CV_IMAGE_ELEM(itrimap,char,i,j) != 100){
				 inmask = false;
				 change++;
				 if(change == 4){
					 point2.y = j;
					 break;
				 }
			 }
	  }
   }

};


#endif