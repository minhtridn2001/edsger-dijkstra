# edsger-dijkstra
Implement edsger dijkstra algorithm - C programming UNIX
SAMPLE INPUT:
9
1 2 7
1 3 9
1 6 14
2 3 10
2 4 15
3 4 11
3 6 2
5 6 9
4 5 6
1
5
1 2 10
2 4 10
3 2 2
4 3 2
5 6 8
2
0
SAMPLE OUTPUT:
Result from node 1
Node 5: dist= 20, previous= 6
Node 4: dist= 20, previous= 3
Node 6: dist= 11, previous= 3
Node 3: dist= 9, previous= 1
Node 2: dist= 7, previous= 1
Node 1: dist= 0, previous= -1
Result from node 2
Node 1: dist= 10, previous= 2
Node 4: dist= 4, previous= 3
Node 3: dist= 2, previous= 2
Node 2: dist= 0, previous= -1
Node 6: dist= 100000, previous= -1
Node 5: dist= 100000, previous= -1
