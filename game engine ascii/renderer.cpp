#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <string>
#include <chrono> 
#include <bits/stdc++.h> 
#include <unistd.h>

using namespace std;
string black = "⬛";
string white = "  ";
vector<vector<string>> render;
string frame;
std::vector<int> AspectRatio;
vector<vector<int>> lines;
vector<vector<int>> triangles;
vector<vector<int>> rects;
int deltaTime = 0;
int Maxfps = 1;
int msCap = 1000/Maxfps;
int setPixel(int x,int y,string design){
    string color = black;
    render[y][x] = color;
}

vector<float> BoundBoxTri(float x0,float y0,float x1,float y1,float x2,float y2){
    vector<float> Xs{x0,x1,x2};
    vector<float> Ys{y0,y1,y2};
    sort(Xs.begin(), Xs.end());
    sort(Ys.begin(), Ys.end()); 
    float up = Ys[0];
    float down = Ys[Ys.size()-1];
    float right = Xs[Xs.size()-1];
    float left = Xs[0];
    vector<float> out{left,up,right,down};
    return out;
}
float TriArea(float x1,float y1,float x2,float y2,float x3,float y3){
    return abs(0.5 * (((x2-x1)*(y3-y1))-((x3-x1)*(y2-y1))));
}
int drawLine(int x0,int y0,int x1, int y1, string color){
    int dx, dy, p, x, y;

	dx=x1-x0;
	dy=y1-y0;

	x=x0;
	y=y0;

	p=2*dy-dx;

	while(x<x1)
	{
		if(p>=0)
		{
			setPixel(x,y,color);
			y=y+1;
			p=p+2*dy-2*dx;
		}
		else
		{
			setPixel(x,y,color);
			p=p+2*dy;
		}
		x=x+1;
	}
}
int drawTri(float x1,float  y1,float  x2,float  y2,float  x3, float y3, string color){
    vector<float> bounds = BoundBoxTri(x1, y1, x2, y2, x3, y3);
    float fullArea = TriArea(x1, y1, x2, y2, x3, y3);
    for(int y = floor(bounds[1]); y< floor(bounds[3]); y++){
        for(int x = floor(bounds[0]); x< floor(bounds[2]); x++){
            float tri1 = TriArea(x1, y1, x2, y2, x,y);
            float tri2 = TriArea(x1, y1, x, y, x3,y3);
            float tri3 = TriArea(x, y, x2, y2, x3,y3);

            if (tri1 + tri2 + tri3 == fullArea){
                setPixel(x,y,color);
            }
        }
    }
    return 0;
}
int drawRect(int x1,int y1,int x2,int y2, string color){
    vector<float> bound = BoundBoxTri(x1,y1,x2,y2,x1,y1);
    for(int y = floor(bound[1]); y< floor(bound[3]); y++){
        for(int x = floor(bound[0]); x< floor(bound[2]); x++){
            setPixel(x,y,color);
        }
    }
    return 0;
}
int renderFrame(){
    //clear
    for(int i = 0; i<AspectRatio[0]; i++){
        vector<string> x;
        for(int i2 = 0; i2 < AspectRatio[1]; i2++){
            x.push_back(white);
        }
        render.push_back(x);
    }
    //lines
    for(int i = 0; i<lines.size(); i++){
        vector<int> line = lines[i];
        string color = white;
        if(line[4] == 1){
            color = black;
        }
        drawLine(line[0],line[1],line[2],line[3],color);
    }
    //triangles
    for(int i = 0; i<triangles.size(); i++){
        vector<int> tri = triangles[i];
        string color = white;
        if(tri[tri.size()-1] == 1){
            color = black;
        }
        drawTri(tri[0],tri[1],tri[2],tri[3],tri[4],tri[5],color);
    }
    //rects
    for(int i = 0; i<rects.size(); i++){
        vector<int> rect = rects[i];
        string color = white;
        if(rect[rect.size()-1] == 1){
            color = black;
        }
        drawRect(rect[0],rect[1],rect[2],rect[3],color);
    }




    for (int i = 0; i < render.size(); i++)
    {
        vector<string> row = render[i];
        for (int i2 = 0; i2 < row.size(); i2++){
            frame += row[i2];
        }
        frame += "\n";
    }
    frame += "\n";
    
    return 0;
}
int timeMs(){
    using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int display(){
    while (true)
    {
        int start = timeMs();
        renderFrame();
        cout << string( 100, '\n' ); // clear screen
        cout<<frame;
        int end = timeMs();
        deltaTime = end-start;
        int timeToWait = msCap - deltaTime;
        cout.flush();
        usleep(timeToWait*1000);
    }
    
    return 0;
}
int main(){
    AspectRatio.push_back (20);
    AspectRatio.push_back (20);
    lines.push_back(vector<int>{1,1,20,15});
    triangles.push_back(vector<int>{0,0,10,5,5,0});
    rects.push_back(vector<int>{10,10,20,20});
    display();
    return 0;
}