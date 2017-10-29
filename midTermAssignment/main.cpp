/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - 已实现功能如下：
 * - 1) 绘制棋盘格和各种形状方块
 * - 2) 键盘左/右/下键控制方块的移动，上键旋转方块
 * - 3) 随机生成方块并赋上不同的颜色
 * - 4) 方块的自动向下移动
 * - 5) 方块之间的碰撞检测
 * - 6) 棋盘格中每一行填充满之后自动消除
 * - 7) 其他
 *
 * ============================================================
 * 致谢：参考代码来自于西蒙弗雷泽大学CMPT361-Assignment
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
using namespace std;

int rotation = 0;        // 控制当前窗口中的方块旋转
vec2 tile[4];            // 表示当前窗口中的方块
bool gameOver = false;   // 游戏结束控制变量
unsigned int FALL_TIME = 400;
int randm;

// 窗口大小
int xsize = 400;
int ysize = 720;

// 当前方块的位置（以棋盘格的左下角为原点的坐标系）
vec2 tilePos = vec2(4, 19);

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[10][20];

// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
// 棋盘格 10*20*2 = 400 个三角形，每个三角形3个点，3*400=1200个点的颜色值需要记录
vec4 boardColors[1200];

GLuint locXSize;
GLuint locYSize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

// 一个二维数组表示该形状所有可能出现的方块和方向
vec2 rotationOShape[1][4] = {
        {vec2(-1, -1), vec2(-1, 0), vec2(0, 0), vec2(0, -1)}
};
vec2 allRotationIShape[2][4] = {
        {vec2(-2, 0), vec2(-1, 0), vec2(0, 0), vec2(1, 0)},
        {vec2(0, -2), vec2(0, -1), vec2(0, 0), vec2(0, 1)}
};
vec2 allRotationSShape[2][4] = {
        {vec2(0, 0), vec2(1, 0), vec2(0, -1), vec2(-1, -1)},
        {vec2(0, 1), vec2(0, 0), vec2(1, -1), vec2(1, 0)}
};
vec2 allRotationZShape[2][4] = {
        {vec2(-1, 0), vec2(0, 0), vec2(0, -1), vec2(1, -1)},
        {vec2(0, -1), vec2(0, 0), vec2(1, 0),  vec2(1, 1)}
};
vec2 allRotationLShape[4][4] = {
        {vec2(0, 0),  vec2(-1, 0), vec2(1, 0),  vec2(-1, -1)},
        {vec2(0, 1),  vec2(0, 0),  vec2(0, -1), vec2(1, -1)},
        {vec2(1, 1),  vec2(-1, 0), vec2(0, 0),  vec2(1, 0)},
        {vec2(-1, 1), vec2(0, 1),  vec2(0, 0),  vec2(0, -1)}
};
vec2 allRotationJShape[4][4] = {
        {vec2(-1, 0),  vec2(0, 0),  vec2(1, 0), vec2(1, -1)},
        {vec2(0, -1),  vec2(0, 0),  vec2(0, 1), vec2(1, 1)},
        {vec2(-1, 1),  vec2(-1, 0), vec2(0, 0), vec2(1, 0)},
        {vec2(-1, -1), vec2(0, -1), vec2(0, 0), vec2(0, 1)}
};
vec2 allRotationTShape[4][4] = {
        {vec2(-1, 0), vec2(0, 0), vec2(1, 0), vec2(0, -1)},
        {vec2(0, -1), vec2(0, 0), vec2(0, 1), vec2(1, 0)},
        {vec2(-1, 0), vec2(0, 0), vec2(1, 0), vec2(0, 1)},
        {vec2(0, -1), vec2(0, 0), vec2(0, 1), vec2(-1, 0)}
};

// 绘制窗口的颜色变量
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
vec4 cyan = vec4(0.0, 1.0, 1.0, 1.0);
vec4 magenta = vec4(1.0, 0.0, 1.0, 1.0);

// 修改棋盘格在pos位置的颜色为color，并且更新对应的VBO
void changeCellColor(vec2 pos, vec4 color) {
    // 每个格子是个正方形，包含两个三角形，总共6个顶点，并在特定的位置赋上适当的颜色
    for (int i = 0; i < 6; i++) {
        boardColors[(int) (6 * (10 * pos.y + pos.x) + i)] = color;
    }

    vec4 newColors[6] = {color, color, color, color, color, color};

    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

    // 计算偏移量，在适当的位置赋上颜色
    int offset = 6 * sizeof(vec4) * (int) (10 * pos.y + pos.x);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newColors), newColors);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 当前方块移动或者旋转时，更新VBO
void updateTile() {
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

    // 每个方块包含四个格子
    for (int i = 0; i < 4; i++) {
        // 计算格子的坐标值
        GLfloat x = tilePos.x + tile[i].x;
        GLfloat y = tilePos.y + tile[i].y;

        vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
        vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
        vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
        vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

        // 每个格子包含两个三角形，所以有6个顶点坐标
        vec4 newPoints[6] = {p1, p2, p3, p2, p3, p4};
        glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newPoints);
    }
    glBindVertexArray(0);
}

// @TODO: 随机选择下一个下落方块形状
// @TODO: 给新方块赋上对应的颜色
// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断，没有足够的空间来生成新的方块。
void newTile() {
    if (!gameOver) {
        // 将新方块放于棋盘格的最上行中间位置并设置默认的旋转方向
        tilePos = vec2(4, 19);
        rotation = 0;
        randm = rand() % 7;

        for (int i = 0; i < 4; i++) {
            switch (randm) {
                case 0:
                    tile[i] = rotationOShape[0][i];
                    break;
                case 1:
                    tile[i] = allRotationIShape[0][i];
                    break;
                case 2:
                    tile[i] = allRotationSShape[0][i];
                    break;
                case 3:
                    tile[i] = allRotationZShape[0][i];
                    break;
                case 4:
                    tile[i] = allRotationLShape[0][i];
                    break;
                case 5:
                    tile[i] = allRotationJShape[0][i];
                    break;
                case 6:
                    tile[i] = allRotationTShape[0][i];
                    break;
                default:
                    break;
            }
        }
    }

    updateTile();

    // 给新方块赋上颜色
    vec4 newColors[24];
    for (int i = 0; i < 24; i++) {
        switch (randm) {
            case 0:
                newColors[i] = orange;
                break;
            case 1:
                newColors[i] = red;
                break;
            case 2:
                newColors[i] = blue;
                break;
            case 3:
                newColors[i] = green;
                break;
            case 4:
                newColors[i] = yellow;
                break;
            case 5:
                newColors[i] = cyan;
                break;
            case 6:
                newColors[i] = magenta;
                break;
            default:
                break;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newColors), newColors);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

// 游戏和OpenGL初始化
void init() {
    // 初始化棋盘格，包含64个顶点坐标（总共32条线），并且每个顶点一个颜色值
    vec4 gridPoints[64];
    vec4 gridColors[64];

    // 纵向线
    for (int i = 0; i < 11; i++) {
        gridPoints[2 * i] = vec4(33.0 + 33.0 * i, 33.0, 0, 1);
        gridPoints[2 * i + 1] = vec4(33.0 + 33.0 * i, 693.0, 0, 1);

    }
    // 水平线
    for (int i = 0; i < 21; i++) {
        gridPoints[22 + 2 * i] = vec4(33.0, 33.0 + 33.0 * i, 0, 1);
        gridPoints[22 + 2 * i + 1] = vec4(363.0, 33.0 + 33.0 * i, 0, 1);
    }

    // 将所有线赋成白色
    for (int i = 0; i < 64; i++) {
        gridColors[i] = white;
    }

    // 初始化棋盘格，并将没有被填充的格子设置成黑色
    vec4 boardPoints[1200];
    for (int i = 0; i < 1200; i++) {
        boardColors[i] = black;
    }

    // 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            vec4 p1 = vec4(33.0 + j * 33.0, 33.0 + i * 33.0, .5, 1);
            vec4 p2 = vec4(33.0 + j * 33.0, 66.0 + i * 33.0, .5, 1);
            vec4 p3 = vec4(66.0 + j * 33.0, 33.0 + i * 33.0, .5, 1);
            vec4 p4 = vec4(66.0 + j * 33.0, 66.0 + i * 33.0, .5, 1);

            boardPoints[6 * (10 * i + j)] = p1;
            boardPoints[6 * (10 * i + j) + 1] = p2;
            boardPoints[6 * (10 * i + j) + 2] = p3;
            boardPoints[6 * (10 * i + j) + 3] = p2;
            boardPoints[6 * (10 * i + j) + 4] = p3;
            boardPoints[6 * (10 * i + j) + 5] = p4;
        }
    }

    // 将棋盘格所有位置的填充与否都设置为false（没有被填充）
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            board[i][j] = false;
        }
    }

    // 载入着色器
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    locXSize = glGetUniformLocation(program, "xsize");
    locYSize = glGetUniformLocation(program, "ysize");

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    GLuint vColor = glGetAttribLocation(program, "vColor");

    glGenVertexArrays(3, &vaoIDs[0]);

    // 棋盘格顶点
    glBindVertexArray(vaoIDs[0]);
    glGenBuffers(2, vboIDs);

    // 棋盘格顶点位置
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
    glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec4), gridPoints, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // 棋盘格顶点颜色
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
    glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec4), gridColors, GL_STATIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);

    // 棋盘格每个格子
    glBindVertexArray(vaoIDs[1]);
    glGenBuffers(2, &vboIDs[2]);

    // 棋盘格每个格子顶点位置
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
    glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(vec4), boardPoints, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // 棋盘格每个格子顶点颜色
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
    glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);

    // 当前方块
    glBindVertexArray(vaoIDs[2]);
    glGenBuffers(2, &vboIDs[4]);

    // 当前方块顶点位置
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // 当前方块顶点颜色
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);

    glBindVertexArray(0);
    glClearColor(0, 0, 0, 0);

    // 游戏初始化
    newTile();
}

// @TODO: 碰撞检测
// 检查在cellPos位置的格子是否被填充或者是否在棋盘格的边界范围内。
bool checkValid(vec2 cellPos) {
    return (cellPos.x >= 0) && (cellPos.x < 10) && (cellPos.y >= 0) && (cellPos.y < 20) ?
           !board[(int)cellPos.x][(int)cellPos.y] : false;
}

// @TODO: 添加当前形状自身旋转切换
// 在棋盘上有足够空间的情况下旋转当前方块
void rotate() {
    // 计算得到下一个旋转方向
    int nextRotation = (rotation + 1) % 4;

    bool isRotatedValid = false;

    for (int i = 0; i < 4; i++) {
        // 检查当前旋转之后的位置的有效性
        if (checkValid(allRotationIShape[nextRotation % 2][i] + tilePos) &&
                checkValid(allRotationSShape[nextRotation % 2][i] + tilePos) &&
                checkValid(allRotationZShape[nextRotation % 2][i] + tilePos) &&
                checkValid(allRotationLShape[nextRotation][i] + tilePos) &&
                checkValid(allRotationJShape[nextRotation][i] + tilePos) &&
                checkValid(allRotationTShape[nextRotation][i] + tilePos)) {
            isRotatedValid = true;
        }
    }

    if (isRotatedValid) {
        // 更新旋转，将当前方块设置为旋转之后的方块
        rotation = nextRotation;

        for (int i = 0; i < 4; i++) {
            switch (randm) {
                case 0:
                    tile[i] = rotationOShape[0][i];
                    break;
                case 1:
                    tile[i] = allRotationIShape[rotation % 2][i];
                    break;
                case 2:
                    tile[i] = allRotationSShape[rotation % 2][i];
                    break;
                case 3:
                    tile[i] = allRotationZShape[rotation % 2][i];
                    break;
                case 4:
                    tile[i] = allRotationLShape[rotation][i];
                    break;
                case 5:
                    tile[i] = allRotationJShape[rotation][i];
                    break;
                case 6:
                    tile[i] = allRotationTShape[rotation][i];
                    break;
                default:
                    break;
            }
        }

        updateTile();
    }
}

// @TODO: 消除填满的行
void clearRow(int row) {
    for (int k = row; k < 19; k++) {
        for (int i = 0; i < 10; i++) {
            // 消除填满的行之后上方的方块下落一格，并相应改变颜色
            board[i][k] = board[i][k + 1];
            changeCellColor(vec2(i, k), boardColors[(6 * (10 * (k + 1) + i))]);
        }
    }
}

// 检查棋盘格在row行有没有被填充满
void checkFullRow(int row) {
    int cnt = 0;
    for (int i = 0; i < 10; i++) {
        if (!checkValid(vec2(i, row))) {
            cnt++;
        }
    }

    if (cnt == 10) {
        clearRow(row);
        checkFullRow(row);  // 重新检查当前行
    }
}

// @TODO: 游戏结束判定
// 检查棋盘格在col列有没有被填充满
void checkFullCol(int col) {
    if (!checkValid(vec2(col, 19))) {
        if (!checkValid(vec2(col, 18))) {
            if (!checkValid(vec2(col, 17))) {
                cout << "Game Over!" << endl;
                gameOver = true;
            }
        }
    }
}

// @TODO: 更新棋盘格对应位置顶点的颜色VBO
// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO
void setTile() {
    // 每个格子
    for (int i = 0; i < 4; i++) {
        // 获取格子在棋盘格上的坐标
        int x = (tile[i] + tilePos).x;
        int y = (tile[i] + tilePos).y;
        // 将格子对应在棋盘格上的位置设置为填充
        board[x][y] = true;
        // 并将相应位置的颜色修改
        switch (randm) {
            case 0:
                changeCellColor(vec2(x, y), orange);
                break;
            case 1:
                changeCellColor(vec2(x, y), red);
                break;
            case 2:
                changeCellColor(vec2(x, y), blue);
                break;
            case 3:
                changeCellColor(vec2(x, y), green);
                break;
            case 4:
                changeCellColor(vec2(x, y), yellow);
                break;
            case 5:
                changeCellColor(vec2(x, y), cyan);
                break;
            case 6:
                changeCellColor(vec2(x, y), magenta);
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < 20; i++) {
        checkFullRow(i);     // 放置后检查是否满行
    }

    for (int i = 0; i < 10; i++) {
        if (!gameOver) {
            checkFullCol(i); // 检查游戏是否结束
        }
    }
}

/* 给定位置(x,y)，移动方块。
 * 有效的移动值为(-1,0)，(1,0)，(0,-1)，
 * 分别对应于向左、向右和向下移动。
 * 如果移动成功，返回值为true，反之为false。
 */
bool moveTile(vec2 direction) {
    // 计算移动之后的方块的位置坐标
    vec2 newTilePos[4];
    for (int i = 0; i < 4; i++) {
        newTilePos[i] = tile[i] + tilePos + direction;
    }

    // 检查移动之后的有效性
    if (checkValid(newTilePos[0]) && checkValid(newTilePos[1]) &&
            checkValid(newTilePos[2]) && checkValid(newTilePos[3])) {
        // 有效：移动该方块
        tilePos.x = tilePos.x + direction.x;
        tilePos.y = tilePos.y + direction.y;

        updateTile();
        return true;
    }

    return false;
}

// 游戏渲染部分
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform1i(locXSize, xsize);
    glUniform1i(locYSize, ysize);

    glBindVertexArray(vaoIDs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 1200);   // 绘制棋盘格 (10*20*2 = 400 个三角形)

    glBindVertexArray(vaoIDs[2]);
    glDrawArrays(GL_TRIANGLES, 0, 24);     // 绘制当前方块 (8 个三角形)

    glBindVertexArray(vaoIDs[0]);
    glDrawArrays(GL_LINES, 0, 64);         // 绘制棋盘格的线

    glutSwapBuffers();
}

// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。
void reshape(GLsizei w, GLsizei h) {
    xsize = w;
    ysize = h;
    glViewport(0, 0, w, h);
}

// 键盘响应事件中的特殊按键响应
void special(int key, int x, int y) {
    if (!gameOver) {
        switch (key) {
            case GLUT_KEY_UP:    // 上方向键旋转方块
                rotate();
                break;
            case GLUT_KEY_DOWN:  // 下方向键向下移动方块
                if (!moveTile(vec2(0, -1))) {
                    setTile();
                    newTile();
                }
                break;
            case GLUT_KEY_LEFT:  // 左方向键向左移动方块
                moveTile(vec2(-1, 0));
                break;
            case GLUT_KEY_RIGHT: // 右方向键向右移动方块
                moveTile(vec2(1, 0));
                break;
            default:
                break;
        }
    }
}

// 重新启动游戏
void restart() {
    gameOver = false;
    init();
}

// 键盘响应时间中的普通按键响应
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 033:
        case 'q':       // ESC键 或 'q' 键退出游戏
            exit(EXIT_SUCCESS);
        case 'r':       // 'r' 键重启游戏
            restart();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// 空闲回调函数
void idle() {
    glutPostRedisplay();
}

// @TODO: 实现自动下落
void autoMoveDown(int x) {
    if (!moveTile(vec2(0, -1))) {
        setTile();
        newTile();
    }
    idle();
    glutTimerFunc(FALL_TIME, autoMoveDown, 1); // 定时器循环调用
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(xsize, ysize);
    glutInitWindowPosition(680, 178);
    glutInitContextVersion(3, 3);
    glewExperimental = GL_TRUE;
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Mid-Term-Skeleton-Code");

    srand((unsigned int) clock());
    glewInit();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(FALL_TIME, autoMoveDown, 1);  // 自动下落
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}