#include "ofApp.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(2);
	ofBackground(255);
	winHeight = ofGetHeight();
	winWidth = ofGetWidth();
	srand((unsigned int)time(NULL));
	num_of_vtx = 8;

	//window â ���� vertex���� ǥ�õ� ��ġ�� ǥ���ϱ� ���� ����
	leftvtx = winWidth * 7 / 20;
	leftmostvtx = winWidth * 3 / 20;
	rightvtx = winWidth * 13 / 20;
	rightmostvtx = winWidth * 17 / 20;
	topvtx = winHeight * 3 / 20;
	bottomvtx = winHeight * 17 / 20;
	heightunit = (bottomvtx - topvtx) / 3;

	num_of_heap = 0;	//heap ���Ұ��� �ʱ�ȭ

	//�� vertex���� ��ǥ�� ����Ǵ� vector vtx�� ��� ����
	vtx.push_back(make_pair(leftvtx, topvtx));
	vtx.push_back(make_pair(rightvtx, topvtx));
	vtx.push_back(make_pair(leftmostvtx, topvtx + heightunit));
	vtx.push_back(make_pair(rightmostvtx, topvtx + heightunit));
	vtx.push_back(make_pair(leftmostvtx, topvtx + 2 * heightunit));
	vtx.push_back(make_pair(rightmostvtx, topvtx + 2 * heightunit));
	vtx.push_back(make_pair(leftvtx, topvtx + 3 * heightunit));
	vtx.push_back(make_pair(rightvtx, topvtx + 3 * heightunit));

	//���ڸ� ���� ���� ofTrueTypeFont ��ü �ʱ�ȭ
	thickletter.loadFont("verdana.ttf", 20, true, true);
	thinletter.loadFont("verdana.ttf", 14, true, true);

	ofSetEscapeQuitsApp(false);	//ESCŰ�� ��� Exit �޴��� ���ؼ��� �����ϵ��� �����Ѵ�

	//�ʱ� flag���� ��� false�� �ʱ�ȭ�Ѵ�.
	input_flag = false;	
	isKruskal = false;
	isPrim = false;
	pause_flag = false;
	pause_mode = false;

	ofSetWindowTitle("Minimum Spanning Tree");
	//ȭ�� �߾ӿ� ������â�� ���
	ofSetWindowPosition((ofGetScreenWidth() - winWidth) / 2, (ofGetScreenHeight() - winHeight) / 2);

	/*ofxWinMenu �ν��Ͻ��� ������ â�� ��ܿ� �� �޴��� ���� ���� �����쿡 ���� ����.
	menu_func���� �޴� ���� �� �����ϴ� �۾� ����*/
	hwnd = WindowFromDC(wglGetCurrentDC());
	winmenu = new ofxWinMenu(this, hwnd);
	winmenu->CreateMenuFunction(&ofApp::menu_func);
	HMENU hmenu = winmenu->CreateWindowMenu();

	//File menu ����
	HMENU submenu = winmenu->AddPopupMenu(hmenu, "File");
	winmenu->AddPopupItem(submenu, "open", false, false);
	winmenu->AddPopupSeparator(submenu);
	winmenu->AddPopupItem(submenu, "Exit", false, false);
	//Algorithm menu ����
	submenu = winmenu->AddPopupMenu(hmenu, "Algorithm");
	//�˰��� ���� �ٸ� �۾��� �����ϹǷ� 3���� ��ư ����,auto check�� ���� ����
	winmenu->AddPopupItem(submenu, "Kruskal's", false, false);
	winmenu->AddPopupItem(submenu, "Prim's", false, false);
	winmenu->AddPopupItem(submenu, "Reset", false, false);
	//Others menu����
	submenu = winmenu->AddPopupMenu(hmenu, "Others");
	winmenu->AddPopupItem(submenu, "about", false, false);

	//�޴� ����
	winmenu->SetWindowMenu();
}

void ofApp::menu_func(string menuname, bool bChecked) {
	//open menu ���� �� readFile�� ȣ���� ���� �Է�
	if (menuname == "open")
		readFile();
	//Exit menu ���� �� ���� �ʱ�ȭ�� �Ҵ������� �����ϴ� freeMemory ȣ�� �� ����
	if (menuname == "Exit") {
		freeMemory();
		ofExit();
	}
	//Kruskal menu ���� �� ���� ���Է��̸� ��ȭâ ����, ���� �Է� ���¸� Kruskal ȣ��
	if (menuname == "Kruskal's") {
		if (!input_flag) {
			ofSystemAlertDialog("Input file is not opened. Open txt file first");
		}
		else {
			Kruskal();
		}
	}
	//Prim menu ���� �� ���� ���Է��̸� ��ȭâ ����, ���� �Է� ���¸� Prim ȣ��
	if (menuname == "Prim's") {
		if (!input_flag) {
			ofSystemAlertDialog("Input file is not opened. Open txt file first");
		}
		else {
			Prim();
		}
	}
	//Reset menu ���� �� ���� ���Է��̸� ��ȭâ ����, ���� �Է� ���¸� freeMemory ȣ���� edge�� ���� ���� �� �����ʱ�ȭ
	if (menuname == "Reset") {
		if (!input_flag) {
			ofSystemAlertDialog("Input file is not opened.");
		}
		else {
			freeMemory();
		}
	}
	//about menu ���� �� ���α׷� ���� �˸�
	if (menuname == "about")
		ofSystemAlertDialog("20181650 AHN DO HYUN\nProgram showing the method for finding minimum spanning tree visually");
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::edgedraw(int i,int j,int color_flag) {
	//���ڷ� �޴� color_flag�� ���� ���� �ٸ��� ĥ���ش�.
	if (color_flag == 0)
		ofSetColor(235);
	else if (color_flag == 1)
		ofSetColor(0, 0, 255);
	else if (color_flag == -1)
		ofSetColor(255, 0, 0);
	char numstr[10];	//cost ǥ�ø� ���� character buffer
	sprintf(numstr, "%d", adjmat[i][j]);
	//�� vertex�� ����� ��ġ�� ���� edge�� ���۵ǰ� ������ ��ġ�� ������ ������ ���ü� ����
	if (vtx[i].first == vtx[j].first) {
		ofDrawLine(vtx[i].first, vtx[i].second + 40, vtx[j].first, vtx[j].second - 40);
		ofSetColor(0);
		thinletter.drawString(numstr, vtx[i].first + (vtx[j].first - vtx[i].first) * 3 / 10 - 7, vtx[i].second + 40 + (vtx[j].second - vtx[i].second - 80) * 3 / 10 + 7);
	}
	else if (vtx[i].second == vtx[j].second) {
		ofDrawLine(vtx[i].first + 40, vtx[i].second, vtx[j].first - 40, vtx[j].second);
		ofSetColor(0);
		thinletter.drawString(numstr, vtx[i].first + 40 + (vtx[j].first - vtx[i].first - 80) * 3 / 10 - 7, vtx[i].second + (vtx[j].second - vtx[i].second) * 3 / 10 + 7);
	}
	else if (i % 2 == 1) {
		ofDrawLine(vtx[i].first - 40, vtx[i].second + 40, vtx[j].first + 40, vtx[j].second - 40);
		ofSetColor(0);
		thinletter.drawString(numstr, vtx[i].first - 40 + (vtx[j].first - vtx[i].first + 80) * 3 / 10 - 7, vtx[i].second + 40 + (vtx[j].second - vtx[i].second - 80) * 3 / 10 + 7);
	}
	else {
		ofDrawLine(vtx[i].first + 40, vtx[i].second + 40, vtx[j].first - 40, vtx[j].second - 40);
		ofSetColor(0);
		thinletter.drawString(numstr, vtx[i].first + 40 + (vtx[j].first - vtx[i].first - 80) * 3 / 10 - 7, vtx[i].second + 40 + (vtx[j].second - vtx[i].second - 80) * 3 / 10 + 7);
	}
}

void ofApp::draw(){
	ofSetColor(0);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetLineWidth(4);
	char str[20];	//�� ǥ�ø� ���� character buffer
	if (input_flag) {
		ofNoFill();
		//�Է¹��� �׷����� edge �׸���
		for (int i = 0; i < 8;i++) {
			for (int j = i; j < 8; j++) {
				if(edges[i][j].cost)
					edgedraw(edges[i][j].i, edges[i][j].j, edges[i][j].color);
			}
		}
		//�Է¹��� �׷����� vertex �׸���
		for (int i = 0; i < 8; i++) {
			ofDrawRectangle(vtx[i].first, vtx[i].second, 80, 80);
			sprintf(str, "%d", i);
			thickletter.drawString(str, vtx[i].first - 8, vtx[i].second + 8);
		}
		if (pause_mode) {
			sprintf(str, "Pause mode on");
			thickletter.drawString(str, 15, winHeight - 50);
		}
		if (isKruskal&&!pause_flag) {
			Kruskaldraw();
		}
		else if (isPrim&&!pause_flag) {
			Primdraw();
		}

	}
}

bool ofApp::readFile() {
	//�̹� �ٸ� file�� ���� ���¶�� freeMemory ���� �� input_flag�� �ʱ�ȭ��Ű�� ���� �Է��� �����Ѵ�.
	if (input_flag) {
		freeMemory();
		input_flag = 0;
	}
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select file(txt file containing 8*8 int array)");
	if (openFileResult.bSuccess) {
		string filename = openFileResult.getName();
		ofFile file(filename);
		if (!file.exists()) {
			cout << "file does not exits!" << '\n';
			return false;
		}
		else {
			cout << "file input success!" << '\n';
			input_flag = true;
		}
		// 8*8 size�� int �迭�� ���� txt file�� �Է����� �޾�, �װ��� �״�� adjmat�� �����Ѵ�.
		ofBuffer buf(file);
		vector<string> each_line;	//�� line�� ���� vector
		for (auto line : buf.getLines()) {
			each_line.push_back(line);
		}
		for (int i = 0; i < 8; i++) {
			vector<string> temp = ofSplitString(each_line[i]," ");	//�� line�� ���� �������� ������ �� ���ڵ��� atoi�� ��ȯ, adjmat�� ����
			for (int j = 0; j < 8; j++) {
				adjmat[i][j] = atoi(temp[j].c_str());
			}
			temp.clear();
		}
		//edge���� ������ edges array�� �ʱ�ȭ
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
					edges[i][j].i = i;
					edges[i][j].j = j;
					edges[i][j].cost = adjmat[i][j];
					edges[i][j].color = 0;
				
			}
		}
	}
}
//--------------------------------------------------------------
void ofApp::freeMemory() {
	pause_flag = 0;	//�Ͻ����� �� ���·� reset�ϰų� �˰��� ���� �� ���� �Է� �� �Ͻ����� ����
	
	//��� edge���� ���� ���󺹱�
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8;j++) {
			edges[i][j].color = 0;
		}
	}
	//Kruskal �Ǵ� Prim �˰����� ������ ���¸� �� flag�� clear�ϰ� �Ҵ��� heap�� delete ���ָ� heap�� ���� ���� ���� �ʱ�ȭ
	if (isKruskal) {
		isKruskal = false;
		num_of_heap = 0;
		delete[] edge_heap;
	}
	else if (isPrim) {
		isPrim=false;
		num_of_heap = 0;
		delete[] edge_heap;
	}
}

//edge��� ������ heap�� �����ϴ� �Լ�
void ofApp::insert_minheap(edge element) {
	//complete binary tree���� ���� ���� item�� �ΰ� parent�� ���� �ö󰡸� parent�� �� ������ �����ϰ� �� ��ġ�� ����
	num_of_heap++;
	int temp = num_of_heap;
	while (temp > 1 && edge_heap[temp / 2].cost > element.cost) {
		edge_heap[temp] = edge_heap[temp / 2];
		temp /= 2;
	}
	edge_heap[temp] = element;
}

//heap���� edge�� �ϳ� �����ϴ� �Լ�
edge ofApp::delete_minheap() {
	//minheap���� root ���Ҹ� �����ϰ� ���� ������ ���Ҹ� root�� �ű� �� child �� �� ���� ���Ҹ� ���� ��ġ�� �ٲٴ� ū ���� ������ ����.
	edge retelem = edge_heap[1];
	edge element = edge_heap[num_of_heap];
	num_of_heap--;
	int parent = 1, child = 2;
	while (child <= num_of_heap) {
		if (child<num_of_heap && edge_heap[child].cost>edge_heap[child + 1].cost)
			child++;
		if (element.cost > edge_heap[child].cost) {
			edge_heap[parent] = edge_heap[child];
			parent = child;
			child = parent * 2;
		}
		else break;
	}
	edge_heap[parent] = element;
	return retelem;
}

//disjoint set���� �θ� ã�� ���� �Լ�.
int ofApp::set_find(int i) {
	//vtx_parent���� ������ �ش� vertex�� set�� root���� �ǹ��ϸ� �� ���밪�� child�� �����̴�.
	//���� parent���� ������ �� ������ parent���� ���� �� root���� index�� �����Ѵ�.
	while (vtx_parent[i]>=0) {
		i = vtx_parent[i];
	}
	return i;
}
void ofApp::set_union(int i, int j) {
	//i�� j�� ���� set�̸� �״�� �����Ѵ�.
	int iset = set_find(i);
	int jset = set_find(j);
	if (iset == jset)
		return;
	
	int temp = vtx_parent[iset] + vtx_parent[jset]; //�� ���� root index�� ����� ���� ���� child�� ������ �̸� ����

	//�� ���� set�� root�� ū set�� root�� child�� �����.
	if (vtx_parent[iset] < vtx_parent[jset]) {
		vtx_parent[jset] = iset;
		vtx_parent[iset] = temp;
	}
	else {
		vtx_parent[iset] = jset;
		vtx_parent[jset] = temp;
	}
}
void ofApp::Kruskal() {
	//�� �������� �ʱ�ȭ
	freeMemory();
	mstedge_count = 0;
	for (int i = 0; i < 8; i++) {
		vtx_parent[i] = -1;
	}

	edge_heap = new edge[65];	//heap �Ҵ�

	//cost�� �ִ� edge�� heap�� ������
	edge tmp;
	for (int i = 0; i < 8; i++) {
		for (int j = i; j < 8; j++) {
			if (adjmat[i][j]) {
				tmp.i = i;
				tmp.j = j;
				tmp.cost = adjmat[i][j];
				insert_minheap(tmp);
			}
		}
	}
	isKruskal = true;	//Kruskal flag set
}
void ofApp::Kruskaldraw() {
	edge temp;
	//num_of_vtx-1��ŭ mstedge_count�� �����ߴٸ�  �̹� MST�� �ϼ��Ǿ�����, 
	//��� �׷��� �� Alertâ�� ���� ���� �ѹ� �� ����������
	if (mstedge_count == num_of_vtx - 1) {
		mstedge_count++;
	}
	else if (mstedge_count == num_of_vtx) {
		ofSystemAlertDialog("MST finding Complete!");
		mstedge_count++;
	}


	if (mstedge_count < num_of_vtx - 1) {
		//�� ���� num_of_heap�� 0�� �Ǿ��ٴ� ���� MST�� �������� �ʴ´ٴ� ���̴�.
		//���� ��� ĥ�� �� Alertâ�� ���� ���� �ѹ� �� �ݺ�����
		if (num_of_heap == 0) {
			num_of_heap = -1;
			return;
		}
		else if (num_of_heap == -1) {
			num_of_heap = 0;
			ofSystemAlertDialog("MST does not exists");
			freeMemory();
			return;
		}

		//heap���� edge�� ������ cycle�� ������ ������ �� edge�� Ǫ�������� ĥ�ϰ�, ����� ���������� ĥ��/
		temp = delete_minheap();
		if (set_find(temp.i) != set_find(temp.j)) {
			set_union(temp.i, temp.j);
			mstedge_count++;
			edges[temp.i][temp.j].color = 1;	//�ش� edge�� ���� Ǫ���� ��ĥ
		}
		else {
			edges[temp.i][temp.j].color = -1;	//�ش� edge�� ���� �Ӱ� ��ĥ

			//pause_mode�� ���������� pause_flag�� set
			if (pause_mode) {
				pausex = temp.i;
				pausey = temp.j;
				pause_flag = true;
			}
		}

	}
}
void ofApp::Prim() {
	//���� ������ �ʱ�ȭ
	freeMemory();
	mstedge_count = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			visited[i][j] = 0;
		}
	}
	for (int i = 0; i < 8; i++) {
		isvtx_included[i] = 0;
	}

	edge_heap = new edge[65];	//heap �Ҵ�
	now_vtx = rand() % 8;	//�����ϴ� vertex�� �������� ����.
	isvtx_included[now_vtx] = 1;	//���� vertex�� �湮 ���� true�� ����

	//���� vertex�� �̾��� edge�� ��� heap�� ����
	for (int i = 0; i < 8; i++) {
		if (edges[now_vtx][i].cost) {
			insert_minheap(edges[now_vtx][i]);
		}
	}
	isPrim = true;	//Prim flag set
}
	
void ofApp::Primdraw() {
	edge temp;
	//msetedge_count�� num_of_vtx-1�� �Ǿ��ٴ� ���� MST Ž���� �����ٴ� ���̴�.
	//���� ��� ĥ�� �� Alertâ�� ���� ���� �ѹ� �� �ݺ�����
	if (mstedge_count == num_of_vtx - 1) {
		mstedge_count++;
	}
	else if (mstedge_count == num_of_vtx) {
		ofSystemAlertDialog("MST finding Complete!");
		mstedge_count++;
	}

	if (mstedge_count < num_of_vtx - 1) {
		while (1) {
			//num_of_heap�� 0�� �Ǿ��ٴ� ���� MST�� �������� �ʴ´ٴ� ���̴�.
			//���� ��� ĥ�� �� Alertâ�� ���� ���� �ѹ� �� �ݺ�����
			if (num_of_heap == 0) {
				num_of_heap = -1;
				return;
			}
			else if (num_of_heap == -1) {
				num_of_heap = 0;
				ofSystemAlertDialog("MST does not exists");
				freeMemory();
				return;
			}
			//�� edge 2d array �ڷᱸ�������� undirected graph�� ���Ͽ� ���� edge�� �ι� ����ǹǷ�, 
			//�̹� �湮�� edge�� heap���� ���´ٸ� ������ �ٽ� delete�ϱ� ���� while ���� ����ߴ�.
			temp = delete_minheap();
			if (!visited[temp.i][temp.j]) {
				break;	
			}
		}

		//temp.i�� temp.j�� �������� ���̱� ���� �ٸ� �ӽ� ������ �������
		int temp1, temp2;
		if (temp.i > temp.j) {
			temp1 = temp.j;
			temp2 = temp.i;
		}
		else {
			temp1 = temp.i;
			temp2 = temp.j;
		}

		//���� ��ȿ�� edge�� ���� �湮�� ����� ������
		visited[temp1][temp2] = visited[temp2][temp1] = 1;

		//edge�� ����� �� vertex �� �湮���� ���� vertex�� �����Ѵٸ�, �� edge�� Ǫ���� ĥ���� ��,
		//�� vertex�� nowvtx�� �������ְ�, nowvtx�� ���Ͽ� �ٽ� ����� edge �� cost�� �����ϴ� edge�� heap�� ����
		if ((isvtx_included[temp1] && !isvtx_included[temp2])||
			(!isvtx_included[temp1] && isvtx_included[temp2])) {
			mstedge_count++;	//�ش� edge�� MST�� �߰�
			edges[temp1][temp2].color = 1;
			if (isvtx_included[temp1] && !isvtx_included[temp2]) {
				now_vtx = temp2;
				isvtx_included[temp2] = 1;
			}
			else if (!isvtx_included[temp1] && isvtx_included[temp2]) {
				now_vtx = temp1;
				isvtx_included[temp1] = 1;
			}

			for (int i = 0; i < 8; i++) {
				if (edges[now_vtx][i].cost) {
					insert_minheap(edges[now_vtx][i]);
				}
			}
		}
		//���� �� vertex�� �� �湮�� vertex��� �� edge�� �Ӱ� ĥ�Ѵ�.
		else if (isvtx_included[temp1] && isvtx_included[temp2]) {
			edges[temp1][temp2].color = -1;
			if (pause_mode) {
				pausex = temp1;
				pausey = temp2;
				pause_flag = true;
			}

		}

	}
}
void ofApp::keyPressed(int key){
	//pause_mode���� cycle ���� �� ȭ���� ���ߴµ�, �����̽��ٸ� ���� �ٽ� ����� �� �ִ�.
	if (key == ' ') {
		if (pause_mode&&pause_flag) {
			pause_flag = false;
			edges[pausex][pausey].color = 0;
		}
	}
	//pŰ�� ���� pause mode�� �Ѱ� �� �� �ִ�.
	if (key == 'p' || key == 'P') {
		pause_mode = !pause_mode;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
