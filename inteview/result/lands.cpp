#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <stack>
#include <vector>
#include <set>

//g++ test.cpp -std=c++11

class Solution {
    public int numIslands(char[][] grid) {
        if (grid == null || grid.length == 0) return 0;
        int row = grid.length, columns = grid[0].length, count = 0;
        for (int i = 0; i < row; i++) {//遍历所有点
            for (int j = 0; j < columns; j++) {
                if (grid[i][j] == '1') {
                    dfs(grid, i, j, row, columns);//dfs遍历所有连接的点
                    count++;//记录岛屿数量
                }
            }
        }
        return count;
    }

    private void dfs(char[][] grid, int i, int j, int row, int columns) {
        if (i < 0 || j < 0 || i >= row || j >= columns || grid[i][j] == '0') return;//基线条件
        grid[i][j] = '0';//遍历过的点置 0
        dfs(grid, i + 1, j, row, columns);
        dfs(grid, i, j + 1, row, columns);
        dfs(grid, i - 1, j, row, columns);
        dfs(grid, i, j - 1, row, columns);
    }
}


class Solution {
    public int numIslands(char[][] grid) {
        if (grid == null || grid.length == 0) return 0;
        int row = grid.length, columns = grid[0].length, count = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < columns; j++) {//遍历所有节点
                if (grid[i][j] == '1') {
                    bfs(grid, i, j, row, columns);
                    count++;//记录岛屿数量
                }
            }
        }
        return count;
    }

    private void bfs(char[][] grid, int i, int j, int row, int columns) {
        Queue<Integer> loc = new LinkedList<>();//队列暂存值为 1 的点
        loc.add(i * columns + j);//暂存该点位置，也可以用一个[i,j]数组表示，不过占用空间也会大一倍
        while (!loc.isEmpty()) {
            int id = loc.remove();//取出位置
            int r = id / columns, c = id % columns;//分解位置得到索引
            if (r - 1 >= 0 && grid[r - 1][c] == '1') {
                loc.add((r - 1) * columns + c);
                grid[r - 1][c] = '0';
            }
            if (r + 1 < row && grid[r + 1][c] == '1') {
                loc.add((r + 1) * columns + c);
                grid[r + 1][c] = '0';
            }
            if (c - 1 >= 0 && grid[r][c - 1] == '1') {
                loc.add(r * columns + c - 1);
                grid[r][c - 1] = '0';
            }
            if (c + 1 < columns && grid[r][c + 1] == '1') {
                loc.add(r * columns + c + 1);
                grid[r][c + 1] = '0';
            }
        }
    }
}



using namespace std;

int main(int argc, char** argv)
{

}