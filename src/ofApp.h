#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"
#include <utility>
#include <vector>
#include <string>

typedef struct {
	int i, j;
	int cost;
	int color;
} edge;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		//윈도우 창에 사용되는 변수 및 함수
		ofxWinMenu* winmenu;
		void menu_func(string menuname, bool bChecked);
		HWND hwnd;
		float winWidth, winHeight;


		vector<pair <int, int>> vtx;	//vertex들의 x,y 좌표를 저장할 vector container
		int leftvtx, rightvtx,leftmostvtx,rightmostvtx, topvtx, bottomvtx;	//vertex를 놓을 위치를 나타내기 위해 정의한 변수	
		float heightunit; //vertex를 놓을 위치를 나타내기 위해 정의한 변수	
		ofTrueTypeFont thickletter;	//윈도우 창에 굵은 글씨를 쓰기 위해 정의한 ofTrueTypeFont 객체
		ofTrueTypeFont thinletter;	//윈도우 창에 얇은 글씨를 쓰기 위해 정의한 ofTrueTypeFont 객체
		bool input_flag;	//현재 파일을 입력받은 상태인지 저장할 flag
		bool isKruskal;		//Kruskal algorithm이 실행중인지 저장할 flag
		bool isPrim;		//Prim algorithm이 실행중인지 저장할 flag
		bool pause_flag;	//cycle이 형성되어 재생이 멈춘 상태를 나타낼 flag
		bool pause_mode;	//pause_mode=true이면 cycle 발생 시 재생을 멈추는 pause_mode 켜기

		int adjmat[8][8];	//파일로부터 입력받은 adjacent matrix를 받을 2d array
		edge edges[8][8];	//edge type 구조체의 2d array
		bool visited[8][8];	//Prim's algorithm에서 이미 방문한 vertex를 저장하기 위한 2d array
		int num_of_vtx;		//vtx의 개수를 저장할 변수. 이 코드에서는 8로 고정됨
		int pausex, pausey;	//화면이 일시정지되었을 때 cycle이 발생한 edge의 양 끝을 저장할 변수

		int now_vtx;	//Prim's algorithm에서 현재 방문한 vertex를 저장할 변수
		int isvtx_included[8];	//Prim's algorithm에서 이미 방문한 vertex를 기억하기 위한 배열
		void edgedraw(int i, int j, int color_flag);	//edge를 그려주는 함수
		int vtx_parent[8];	//Kruskal's algorithm에서 disjoint set을 사용하기 위해 선언한 배열
		edge* edge_heap;	//두 알고리즘에서 Priority Queue(Minheap)를 사용해 시간 및 공간복잡도 감소
		int num_of_heap;	//heap에 들어간 edge element의 개수
		int mstedge_count;	//현재 MST의 구성 edge로서 포함된 edge의 개수. num_of_vtx-1이 되면 탐색 종료
		bool readFile();	//파일로부터 입력받기 위한 함수
		void freeMemory();	//한 알고리즘에서 다른 알고리즘으로 전환하거나 다른 입력파일을 받을 때 각 변수들을 할당해제 해주거나 변수를 초기화. edge들의 색도 초기화해줌
		void insert_minheap(edge element);	//heap에 edge를 넣는 함수
		edge delete_minheap();	//heap에서 edge 하나를 빼서 리턴하는 함수
		int set_find(int i);	//vertex i가 포함된 set을 확인하기 위한 함수.(parent를 탐색)
		void set_union(int i,int j);	//i의 set과 j의 set을 통합하는 함수
		void Kruskal();	//Kruskal algorithm을 적용하기 위한 전처리를 수행하는 함수
		void Kruskaldraw();	//본격적으로 Kruskal algorithm에 따라 edge를 그려주는 함수
		void Prim();	//Prim algorithm을 적용하기 위한 전처리를 수행하는 함수
		void Primdraw();	//본격적으로 Prim algorithm에 따라 edge를 그려주는 함수
};
