#include "Angel.h"
#include "mat.h"
#include<vector>
#include <fstream>

#pragma comment(lib, "glew32.lib")

typedef struct vIndex {
    unsigned int a, b, c;

    vIndex(int ia, int ib, int ic) : a(ia), b(ib), c(ic) {}
} vec3i;

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;
int AXIS;

const int TRANSFORM_SCALE = 0;
const int TRANSFORM_ROTATE = 1;
const int TRANSFORM_TRANSLATE = 2;

const double DELTA_DELTA = 0.1;
const double DEFAULT_DELTA = 0.3;

double scaleDelta = DEFAULT_DELTA;
double rotateDelta = DEFAULT_DELTA;
double translateDelta = DEFAULT_DELTA;

vec3 scaleTheta(1.0, 1.0, 1.0);
vec3 rotateTheta(0.0, 0.0, 0.0);
vec3 translateTheta(0.0, 0.0, 0.0);

GLint matrixLocation;
int currentTransform = TRANSFORM_TRANSLATE;
int mainWindow;

int nVertices = 0;
int nFaces = 0;
int nEdges = 0;
std::vector<vec3> vertices;
std::vector<vec3i> faces;

std::vector<vec3> points;   //传入着色器的绘制点
std::vector<vec3> colors;   //传入着色器的颜色

const int NUM_VERTICES = 8;
const vec3 vertex_colors[NUM_VERTICES] = {
        vec3(1.0, 1.0, 1.0),  // White
        vec3(1.0, 1.0, 0.0),  // Yellow
        vec3(0.0, 1.0, 0.0),  // Green
        vec3(0.0, 1.0, 1.0),  // Cyan
        vec3(1.0, 0.0, 1.0),  // Magenta
        vec3(1.0, 0.0, 0.0),  // Red
        vec3(0.0, 0.0, 0.0),  // Black
        vec3(0.0, 0.0, 1.0)   // Blue
};

void read_off(const std::string filename) {
    if (filename.empty()) {
        return;
    }

    std::ifstream fin;
    fin.open(filename);

    // @TODO: 修改此函数读取OFF文件中三维模型的信息
    std::string offMsg;
    fin >> offMsg >> nVertices >> nFaces >> nEdges;

    for (int i = 0; i < nVertices; i++) {
        double x, y, z;
        fin >> x >> y >> z;
        vertices.push_back(vec3((GLfloat) x, (GLfloat) y, (GLfloat) z));
    }

    for (int i = 0; i < nFaces; i++) {
        int n, x, y, z;
        fin >> n >> x >> y >> z;
        faces.push_back(vec3i(x, y, z));
    }

    fin.close();
}

// 存储每个三角的各个顶点并为点赋色
void storeFacesPoints() {
    points.clear();
    colors.clear();

    // @TODO: 修改此函数在points和colors容器中存储每个三角面片的各个点和颜色信息
    for (int i = 0; i < nFaces; ++i) {
        int a = faces[i].a;
        int b = faces[i].b;
        int c = faces[i].c;

        points.push_back(vertices[a]);
        points.push_back(vertices[b]);
        points.push_back(vertices[c]);

        colors.push_back(vertex_colors[a]);
        colors.push_back(vertex_colors[b]);
        colors.push_back(vertex_colors[c]);
    }
}

void init() {
    storeFacesPoints();

    // 创建顶点数组对象
    GLuint vao[1];
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    // 创建并初始化顶点缓存对象
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3) + colors.size() * sizeof(vec3),
                 NULL, GL_STATIC_DRAW);

    // 分别读取数据
    glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(vec3), &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), colors.size() * sizeof(vec3), &colors[0]);

    // 读取着色器并使用
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // 从顶点着色器中初始化顶点的位置
    GLuint pLocation = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(pLocation);
    glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    // 从片元着色器中初始化顶点的颜色
    GLuint cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points.size() * sizeof(vec3)));
    // 获得矩阵存储位置
    matrixLocation = glGetUniformLocation(program, "matrix");

    // 黑色背景
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display() {
    // 清理窗口
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 生成变换矩阵
    mat4 m(1.0, 0.0, 0.0, 0.0,
           0.0, 1.0, 0.0, 0.0,
           0.0, 0.0, 1.0, 0.0,
           0.0, 0.0, 0.0, 1.0);

    // @TODO: 在此处修改函数，计算变换矩阵
    m = m * Translate(translateTheta[X_AXIS],
                      translateTheta[Y_AXIS],
                      translateTheta[Z_AXIS]);

    m = m * RotateX(rotateTheta[X_AXIS]);
    m = m * RotateY(rotateTheta[Y_AXIS]);
    m = m * RotateZ(rotateTheta[Z_AXIS]);

    m = m * Scale(scaleTheta[X_AXIS],
                  scaleTheta[Y_AXIS],
                  scaleTheta[Z_AXIS]);

    // 从指定位置中传入变换矩阵
    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, m);
    glDrawArrays(GL_TRIANGLES, 0, points.size());

    glutSwapBuffers();
}

// 通过Delta值更新Theta
void updateTheta(int axis, int sign) {
    switch (currentTransform) {
        case TRANSFORM_SCALE:
            scaleTheta[axis] += sign * scaleDelta;
            break;
        case TRANSFORM_ROTATE:
            rotateTheta[axis] += sign * rotateDelta;
            break;
        case TRANSFORM_TRANSLATE:
            translateTheta[axis] += sign * translateDelta;
            break;
    }
}

// 复原Theta和Delta
void resetTheta() {
    scaleTheta = vec3(1.0, 1.0, 1.0);
    rotateTheta = vec3(0.0, 0.0, 0.0);
    translateTheta = vec3(0.0, 0.0, 0.0);
    scaleDelta = DEFAULT_DELTA;
    rotateDelta = DEFAULT_DELTA;
    translateDelta = DEFAULT_DELTA;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            updateTheta(X_AXIS, 1);
            break;
        case 'a':
            updateTheta(X_AXIS, -1);
            break;
        case 'w':
            updateTheta(Y_AXIS, 1);
            break;
        case 's':
            updateTheta(Y_AXIS, -1);
            break;
        case 'e':
            updateTheta(Z_AXIS, 1);
            break;
        case 'd':
            updateTheta(Z_AXIS, -1);
            break;
        case 't':
            resetTheta();
            break;
        case 'x':
            AXIS = X_AXIS;
            break;
        case 'y':
            AXIS = Y_AXIS;
            break;
        case 'z':
            AXIS = Z_AXIS;
            break;
        case 033:
            // Esc按键
            exit(EXIT_SUCCESS);
            break;
    }
    glutPostWindowRedisplay(mainWindow);
}

// 顺时针旋转
void clockwise() {
    rotateTheta[AXIS] -= 1 * rotateDelta;
    glutPostWindowRedisplay(mainWindow);
}

// 逆时针旋转
void anticlockwise() {
    rotateTheta[AXIS] += 1 * rotateDelta;
    glutPostWindowRedisplay(mainWindow);
}

// 主窗口鼠标回调函数
void mainWindowMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 按下鼠标左键，指定当没有其他事件处理时，去调用clockwise()这个函数顺时针旋转
        glutIdleFunc(clockwise);
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // 按下鼠标右键，指定当没有其他事件处理时，去调用clockwise()这个函数顺时针旋转
        glutIdleFunc(anticlockwise);
    } else if (button == GLUT_LEFT_BUTTON ||
               button == GLUT_RIGHT_BUTTON
               && state == GLUT_UP) {
        // 释放鼠标左/右键，解除调用
        glutIdleFunc(NULL);
    }
}

void menuEvents(int menuChoice) {
    currentTransform = menuChoice;
}

void setupMenu() {
    glutCreateMenu(menuEvents);
    glutAddMenuEntry("Scale", TRANSFORM_SCALE);
    glutAddMenuEntry("Rotate", TRANSFORM_ROTATE);
    glutAddMenuEntry("Translate", TRANSFORM_TRANSLATE);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void printHelp() {
    printf("%s\n\n", "3D Transfomations");
    printf("Keyboard options:\n");
    printf("q: Increase x\n");
    printf("a: Decrease x\n");
    printf("w: Increase y\n");
    printf("s: Decrease y\n");
    printf("e: Increase z\n");
    printf("d: Decrease z\n");
    printf("x: Rotate by x\n");
    printf("y: Rotate by y\n");
    printf("z: Rotate by z\n");
    printf("t: Reset all transformations and deltas\n");
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    mainWindow = glutCreateWindow("3D Transformation");

    glewExperimental = GL_TRUE;
    glewInit();
    read_off("cube.off");
    init();
    setupMenu();
    glutDisplayFunc(display);
    // 在主窗口中指定函数mainWindowMouse，在鼠标按下或释放时将会被调用
    glutMouseFunc(mainWindowMouse);
    glutKeyboardFunc(keyboard);
    // 输出帮助信息
    printHelp();
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();

    return 0;
}


