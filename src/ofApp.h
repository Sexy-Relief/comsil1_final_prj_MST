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
		
		//������ â�� ���Ǵ� ���� �� �Լ�
		ofxWinMenu* winmenu;
		void menu_func(string menuname, bool bChecked);
		HWND hwnd;
		float winWidth, winHeight;


		vector<pair <int, int>> vtx;	//vertex���� x,y ��ǥ�� ������ vector container
		int leftvtx, rightvtx,leftmostvtx,rightmostvtx, topvtx, bottomvtx;	//vertex�� ���� ��ġ�� ��Ÿ���� ���� ������ ����	
		float heightunit; //vertex�� ���� ��ġ�� ��Ÿ���� ���� ������ ����	
		ofTrueTypeFont thickletter;	//������ â�� ���� �۾��� ���� ���� ������ ofTrueTypeFont ��ü
		ofTrueTypeFont thinletter;	//������ â�� ���� �۾��� ���� ���� ������ ofTrueTypeFont ��ü
		bool input_flag;	//���� ������ �Է¹��� �������� ������ flag
		bool isKruskal;		//Kruskal algorithm�� ���������� ������ flag
		bool isPrim;		//Prim algorithm�� ���������� ������ flag
		bool pause_flag;	//cycle�� �����Ǿ� ����� ���� ���¸� ��Ÿ�� flag
		bool pause_mode;	//pause_mode=true�̸� cycle �߻� �� ����� ���ߴ� pause_mode �ѱ�

		int adjmat[8][8];	//���Ϸκ��� �Է¹��� adjacent matrix�� ���� 2d array
		edge edges[8][8];	//edge type ����ü�� 2d array
		bool visited[8][8];	//Prim's algorithm���� �̹� �湮�� vertex�� �����ϱ� ���� 2d array
		int num_of_vtx;		//vtx�� ������ ������ ����. �� �ڵ忡���� 8�� ������
		int pausex, pausey;	//ȭ���� �Ͻ������Ǿ��� �� cycle�� �߻��� edge�� �� ���� ������ ����

		int now_vtx;	//Prim's algorithm���� ���� �湮�� vertex�� ������ ����
		int isvtx_included[8];	//Prim's algorithm���� �̹� �湮�� vertex�� ����ϱ� ���� �迭
		void edgedraw(int i, int j, int color_flag);	//edge�� �׷��ִ� �Լ�
		int vtx_parent[8];	//Kruskal's algorithm���� disjoint set�� ����ϱ� ���� ������ �迭
		edge* edge_heap;	//�� �˰��򿡼� Priority Queue(Minheap)�� ����� �ð� �� �������⵵ ����
		int num_of_heap;	//heap�� �� edge element�� ����
		int mstedge_count;	//���� MST�� ���� edge�μ� ���Ե� edge�� ����. num_of_vtx-1�� �Ǹ� Ž�� ����
		bool readFile();	//���Ϸκ��� �Է¹ޱ� ���� �Լ�
		void freeMemory();	//�� �˰��򿡼� �ٸ� �˰������� ��ȯ�ϰų� �ٸ� �Է������� ���� �� �� �������� �Ҵ����� ���ְų� ������ �ʱ�ȭ. edge���� ���� �ʱ�ȭ����
		void insert_minheap(edge element);	//heap�� edge�� �ִ� �Լ�
		edge delete_minheap();	//heap���� edge �ϳ��� ���� �����ϴ� �Լ�
		int set_find(int i);	//vertex i�� ���Ե� set�� Ȯ���ϱ� ���� �Լ�.(parent�� Ž��)
		void set_union(int i,int j);	//i�� set�� j�� set�� �����ϴ� �Լ�
		void Kruskal();	//Kruskal algorithm�� �����ϱ� ���� ��ó���� �����ϴ� �Լ�
		void Kruskaldraw();	//���������� Kruskal algorithm�� ���� edge�� �׷��ִ� �Լ�
		void Prim();	//Prim algorithm�� �����ϱ� ���� ��ó���� �����ϴ� �Լ�
		void Primdraw();	//���������� Prim algorithm�� ���� edge�� �׷��ִ� �Լ�
};
