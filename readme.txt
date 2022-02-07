Finding MST for 8 vertex graph 
1. 프로젝트를 빌드 및 실행시키면 윈도우 창이 뜹니다.
2. 알고리즘을 적용하기 전 File->open에서 txt 파일을 입력받습니다.(정수끼리 space로 구분된 8*8 adjacent matrix가 저장되어 있는 txt파일)
3. 실행시키면 화면에 8개의 vertex와 그 edge들이 표시됩니다.
4. 이후 Algorithm 탭에서 Kruskal's 또는 Prim's 탭을 선택합니다.
5. 해당 알고리즘으로 MST를 탐색하는 과정이 표시됩니다. 붉은 선은 Cycle이 만들어지기 때문에 포함되지 않는 경로, 푸른 선은 MST에 포함되는 경로입니다.
6. File-Exit 메뉴를 통해 프로그램을 종료할 수 있습니다.

--Algorithm 탭의 reset 메뉴를 이용해 다시 입력만 받은 상태로 초기화할 수 있습니다.
--p키를 누르면 pause mode가 적용되어 cycle이 만들어지는 붉은 선의 경로가 발견될 때마다 화면을 일시 정지하고,
  스페이스바를 이용해 다시 재생시킬 수 있습니다. 다시 p를 눌러 모드를 해제할 수 있습니다.
--Prim's algorithm의 경우 랜덤한 vertex에서 시작하기 때문에 시작한 vertex에 연결된 edge가 없다면 바로 탐색이 종료됩니다.