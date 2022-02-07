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

	//window 창 위에 vertex들이 표시될 위치를 표현하기 위한 변수
	leftvtx = winWidth * 7 / 20;
	leftmostvtx = winWidth * 3 / 20;
	rightvtx = winWidth * 13 / 20;
	rightmostvtx = winWidth * 17 / 20;
	topvtx = winHeight * 3 / 20;
	bottomvtx = winHeight * 17 / 20;
	heightunit = (bottomvtx - topvtx) / 3;

	num_of_heap = 0;	//heap 원소개수 초기화

	//각 vertex들의 좌표가 저장되는 vector vtx에 모두 삽입
	vtx.push_back(make_pair(leftvtx, topvtx));
	vtx.push_back(make_pair(rightvtx, topvtx));
	vtx.push_back(make_pair(leftmostvtx, topvtx + heightunit));
	vtx.push_back(make_pair(rightmostvtx, topvtx + heightunit));
	vtx.push_back(make_pair(leftmostvtx, topvtx + 2 * heightunit));
	vtx.push_back(make_pair(rightmostvtx, topvtx + 2 * heightunit));
	vtx.push_back(make_pair(leftvtx, topvtx + 3 * heightunit));
	vtx.push_back(make_pair(rightvtx, topvtx + 3 * heightunit));

	//글자를 쓰기 위해 ofTrueTypeFont 객체 초기화
	thickletter.loadFont("verdana.ttf", 20, true, true);
	thinletter.loadFont("verdana.ttf", 14, true, true);

	ofSetEscapeQuitsApp(false);	//ESC키를 잠궈 Exit 메뉴를 통해서만 종료하도록 유도한다

	//초기 flag들을 모두 false로 초기화한다.
	input_flag = false;	
	isKruskal = false;
	isPrim = false;
	pause_flag = false;
	pause_mode = false;

	ofSetWindowTitle("Minimum Spanning Tree");
	//화면 중앙에 윈도우창을 띄움
	ofSetWindowPosition((ofGetScreenWidth() - winWidth) / 2, (ofGetScreenHeight() - winHeight) / 2);

	/*ofxWinMenu 인스턴스로 윈도우 창의 상단에 들어갈 메뉴를 위해 현재 윈도우에 대해 생성.
	menu_func에서 메뉴 선택 시 수행하는 작업 정의*/
	hwnd = WindowFromDC(wglGetCurrentDC());
	winmenu = new ofxWinMenu(this, hwnd);
	winmenu->CreateMenuFunction(&ofApp::menu_func);
	HMENU hmenu = winmenu->CreateWindowMenu();

	//File menu 생성
	HMENU submenu = winmenu->AddPopupMenu(hmenu, "File");
	winmenu->AddPopupItem(submenu, "open", false, false);
	winmenu->AddPopupSeparator(submenu);
	winmenu->AddPopupItem(submenu, "Exit", false, false);
	//Algorithm menu 생성
	submenu = winmenu->AddPopupMenu(hmenu, "Algorithm");
	//알고리즘 별로 다른 작업을 수행하므로 3개의 버튼 생성,auto check는 하지 않음
	winmenu->AddPopupItem(submenu, "Kruskal's", false, false);
	winmenu->AddPopupItem(submenu, "Prim's", false, false);
	winmenu->AddPopupItem(submenu, "Reset", false, false);
	//Others menu생성
	submenu = winmenu->AddPopupMenu(hmenu, "Others");
	winmenu->AddPopupItem(submenu, "about", false, false);

	//메뉴 적용
	winmenu->SetWindowMenu();
}

void ofApp::menu_func(string menuname, bool bChecked) {
	//open menu 선택 시 readFile을 호출해 파일 입력
	if (menuname == "open")
		readFile();
	//Exit menu 선택 시 변수 초기화와 할당해제를 수행하는 freeMemory 호출 및 종료
	if (menuname == "Exit") {
		freeMemory();
		ofExit();
	}
	//Kruskal menu 선택 시 파일 미입력이면 대화창 띄우기, 파일 입력 상태면 Kruskal 호출
	if (menuname == "Kruskal's") {
		if (!input_flag) {
			ofSystemAlertDialog("Input file is not opened. Open txt file first");
		}
		else {
			Kruskal();
		}
	}
	//Prim menu 선택 시 파일 미입력이면 대화창 띄우기, 파일 입력 상태면 Prim 호출
	if (menuname == "Prim's") {
		if (!input_flag) {
			ofSystemAlertDialog("Input file is not opened. Open txt file first");
		}
		else {
			Prim();
		}
	}
	//Reset menu 선택 시 파일 미입력이면 대화창 띄우기, 파일 입력 상태면 freeMemory 호출해 edge의 색을 제거 및 변수초기화
	if (menuname == "Reset") {
		if (!input_flag) {
			ofSystemAlertDialog("Input file is not opened.");
		}
		else {
			freeMemory();
		}
	}
	//about menu 선택 시 프로그램 정보 알림
	if (menuname == "about")
		ofSystemAlertDialog("20181650 AHN DO HYUN\nProgram showing the method for finding minimum spanning tree visually");
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::edgedraw(int i,int j,int color_flag) {
	//인자로 받는 color_flag에 따라 색을 다르게 칠해준다.
	if (color_flag == 0)
		ofSetColor(235);
	else if (color_flag == 1)
		ofSetColor(0, 0, 255);
	else if (color_flag == -1)
		ofSetColor(255, 0, 0);
	char numstr[10];	//cost 표시를 위한 character buffer
	sprintf(numstr, "%d", adjmat[i][j]);
	//두 vertex의 상대적 위치에 따라 edge가 시작되고 끝나는 위치를 적절히 조절해 가시성 제고
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
	char str[20];	//글 표시를 위한 character buffer
	if (input_flag) {
		ofNoFill();
		//입력받은 그래프의 edge 그리기
		for (int i = 0; i < 8;i++) {
			for (int j = i; j < 8; j++) {
				if(edges[i][j].cost)
					edgedraw(edges[i][j].i, edges[i][j].j, edges[i][j].color);
			}
		}
		//입력받은 그래프의 vertex 그리기
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
	//이미 다른 file을 읽은 상태라면 freeMemory 실행 후 input_flag를 초기화시키고 파일 입력을 수행한다.
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
		// 8*8 size의 int 배열이 적힌 txt file을 입력으로 받아, 그것을 그대로 adjmat에 저장한다.
		ofBuffer buf(file);
		vector<string> each_line;	//각 line을 받을 vector
		for (auto line : buf.getLines()) {
			each_line.push_back(line);
		}
		for (int i = 0; i < 8; i++) {
			vector<string> temp = ofSplitString(each_line[i]," ");	//각 line에 대해 공백으로 나누어 그 숫자들을 atoi로 변환, adjmat에 저장
			for (int j = 0; j < 8; j++) {
				adjmat[i][j] = atoi(temp[j].c_str());
			}
			temp.clear();
		}
		//edge들을 저장할 edges array를 초기화
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
	pause_flag = 0;	//일시정지 된 상태로 reset하거나 알고리즘 변경 및 파일 입력 시 일시정지 해제
	
	//모든 edge들의 색을 원상복귀
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8;j++) {
			edges[i][j].color = 0;
		}
	}
	//Kruskal 또는 Prim 알고리즘이 동작한 상태면 그 flag를 clear하고 할당한 heap을 delete 해주며 heap의 원소 개수 또한 초기화
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

//edge들로 구성된 heap에 삽입하는 함수
void ofApp::insert_minheap(edge element) {
	//complete binary tree에서 제일 끝에 item을 두고 parent를 따라 올라가며 parent가 더 작으면 정지하고 그 위치에 삽입
	num_of_heap++;
	int temp = num_of_heap;
	while (temp > 1 && edge_heap[temp / 2].cost > element.cost) {
		edge_heap[temp] = edge_heap[temp / 2];
		temp /= 2;
	}
	edge_heap[temp] = element;
}

//heap에서 edge를 하나 삭제하는 함수
edge ofApp::delete_minheap() {
	//minheap에서 root 원소를 제거하고 가장 마지막 원소를 root로 옮긴 뒤 child 중 더 작은 원소를 따라 위치를 바꾸다 큰 값을 만나면 멈춤.
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

//disjoint set에서 부모를 찾기 위한 함수.
int ofApp::set_find(int i) {
	//vtx_parent값이 음수면 해당 vertex가 set의 root임을 의미하며 그 절대값은 child의 개수이다.
	//따라서 parent값이 음수가 될 때까지 parent값을 따라가 그 root값의 index를 리턴한다.
	while (vtx_parent[i]>=0) {
		i = vtx_parent[i];
	}
	return i;
}
void ofApp::set_union(int i, int j) {
	//i와 j가 같은 set이면 그대로 리턴한다.
	int iset = set_find(i);
	int jset = set_find(j);
	if (iset == jset)
		return;
	
	int temp = vtx_parent[iset] + vtx_parent[jset]; //두 값의 root index에 저장된 값을 더해 child의 개수를 미리 저장

	//더 작은 set의 root를 큰 set의 root의 child로 만든다.
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
	//각 변수값들 초기화
	freeMemory();
	mstedge_count = 0;
	for (int i = 0; i < 8; i++) {
		vtx_parent[i] = -1;
	}

	edge_heap = new edge[65];	//heap 할당

	//cost가 있는 edge를 heap에 삽입함
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
	//num_of_vtx-1만큼 mstedge_count가 증가했다면  이미 MST가 완성되었지만, 
	//모두 그려준 뒤 Alert창을 보기 위해 한번 더 증가시켜줌
	if (mstedge_count == num_of_vtx - 1) {
		mstedge_count++;
	}
	else if (mstedge_count == num_of_vtx) {
		ofSystemAlertDialog("MST finding Complete!");
		mstedge_count++;
	}


	if (mstedge_count < num_of_vtx - 1) {
		//이 또한 num_of_heap이 0이 되었다는 것은 MST가 존재하지 않는다는 뜻이다.
		//선을 모두 칠한 후 Alert창을 보기 위해 한번 더 반복해줌
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

		//heap에서 edge를 꺼내고 cycle을 만들지 않으면 그 edge를 푸른색으로 칠하고, 만들면 붉은색으로 칠함/
		temp = delete_minheap();
		if (set_find(temp.i) != set_find(temp.j)) {
			set_union(temp.i, temp.j);
			mstedge_count++;
			edges[temp.i][temp.j].color = 1;	//해당 edge의 색을 푸르게 색칠
		}
		else {
			edges[temp.i][temp.j].color = -1;	//해당 edge의 색을 붉게 색칠

			//pause_mode가 켜져있으면 pause_flag를 set
			if (pause_mode) {
				pausex = temp.i;
				pausey = temp.j;
				pause_flag = true;
			}
		}

	}
}
void ofApp::Prim() {
	//각종 변수값 초기화
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

	edge_heap = new edge[65];	//heap 할당
	now_vtx = rand() % 8;	//시작하는 vertex를 랜덤으로 설정.
	isvtx_included[now_vtx] = 1;	//시작 vertex의 방문 여부 true로 변경

	//현재 vertex와 이어진 edge를 모두 heap에 삽입
	for (int i = 0; i < 8; i++) {
		if (edges[now_vtx][i].cost) {
			insert_minheap(edges[now_vtx][i]);
		}
	}
	isPrim = true;	//Prim flag set
}
	
void ofApp::Primdraw() {
	edge temp;
	//msetedge_count가 num_of_vtx-1이 되었다는 것은 MST 탐색이 끝났다는 것이다.
	//선을 모두 칠한 후 Alert창을 보기 위해 한번 더 반복해줌
	if (mstedge_count == num_of_vtx - 1) {
		mstedge_count++;
	}
	else if (mstedge_count == num_of_vtx) {
		ofSystemAlertDialog("MST finding Complete!");
		mstedge_count++;
	}

	if (mstedge_count < num_of_vtx - 1) {
		while (1) {
			//num_of_heap이 0이 되었다는 것은 MST가 존재하지 않는다는 뜻이다.
			//선을 모두 칠한 후 Alert창을 보기 위해 한번 더 반복해줌
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
			//이 edge 2d array 자료구조에서는 undirected graph에 대하여 같은 edge가 두번 저장되므로, 
			//이미 방문한 edge를 heap에서 꺼냈다면 버리고 다시 delete하기 위해 while 문을 사용했다.
			temp = delete_minheap();
			if (!visited[temp.i][temp.j]) {
				break;	
			}
		}

		//temp.i와 temp.j의 가독성을 높이기 위해 다른 임시 변수를 만들어줌
		int temp1, temp2;
		if (temp.i > temp.j) {
			temp1 = temp.j;
			temp2 = temp.i;
		}
		else {
			temp1 = temp.i;
			temp2 = temp.j;
		}

		//꺼낸 유효한 edge에 대해 방문한 기록을 남겨줌
		visited[temp1][temp2] = visited[temp2][temp1] = 1;

		//edge에 연결된 두 vertex 중 방문하지 않은 vertex가 존재한다면, 그 edge를 푸르게 칠해준 뒤,
		//그 vertex로 nowvtx를 움직여주고, nowvtx에 대하여 다시 연결된 edge 중 cost가 존재하는 edge를 heap에 삽입
		if ((isvtx_included[temp1] && !isvtx_included[temp2])||
			(!isvtx_included[temp1] && isvtx_included[temp2])) {
			mstedge_count++;	//해당 edge를 MST에 추가
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
		//만약 두 vertex가 다 방문한 vertex라면 이 edge를 붉게 칠한다.
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
	//pause_mode에서 cycle 형성 시 화면이 멈추는데, 스페이스바를 통해 다시 재생할 수 있다.
	if (key == ' ') {
		if (pause_mode&&pause_flag) {
			pause_flag = false;
			edges[pausex][pausey].color = 0;
		}
	}
	//p키를 통해 pause mode를 켜고 끌 수 있다.
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
