#include "Angel.h"

#pragma comment(lib, "glew32.lib")

const vec3 WHITE(1.0, 1.0, 1.0);
const vec3 BLACK(0.0, 0.0, 0.0);
const vec3 RED(1.0, 0.0, 0.0);
const vec3 GREEN(0.0, 1.0, 0.0);
const vec3 BLUE(0.0, 0.0, 1.0);

const int CIRCLE_NUM_POINTS = 100;
const int ELLIPSE_NUM_POINTS = 100;

const int TRIANGLE_NUM_POINTS = 3;
const int SQUARE_NUM = 16;
const int SQUARE_NUM_POINTS = 4 * SQUARE_NUM;
const int LINE_NUM_POINTS = 2;

const int TOTAL_NUM_POINTS = TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS + LINE_NUM_POINTS +
                             CIRCLE_NUM_POINTS + ELLIPSE_NUM_POINTS;

// 锟斤拷锟斤拷锟斤拷锟斤拷蔚锟矫匡拷锟斤拷嵌锟�
double getTriangleAngle(int point) {
    return 2 * M_PI / 3 * point;
}

// 锟斤拷锟斤拷锟斤拷锟斤拷蔚锟矫匡拷锟斤拷嵌锟�
double getSquareAngle(int point) {
    return M_PI / 4 + (M_PI / 2 * point);
}

void generateTrianglePoints(vec2 vertices[], vec3 colors[], int startVertexIndex) {
    double scale = 0.2;
    vec2 center(0.0, 0.8);

    for (int i = 0; i < 3; ++i) {
        double currentAngle = getTriangleAngle(i);
        vertices[startVertexIndex + i] = vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
    }

    colors[startVertexIndex] = RED;
    colors[startVertexIndex + 1] = GREEN;
    colors[startVertexIndex + 2] = BLUE;
}

void generateSquarePoints(vec2 vertices[], vec3 colors[], int squareNumber, int startVertexIndex) {
    double scale = 0.60;
    double scaleDecrease = 0.04;
    vec2 center(0.0, -0.15);
    int vertexIndex = startVertexIndex;

    for (int i = 0; i < squareNumber; ++i) {
        vec3 currentColor;
        currentColor = i % 2 ? WHITE : BLACK;
        for (int j = 0; j < 4; ++j) {
            double currentAngle = getSquareAngle(j);
            vertices[vertexIndex] = vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
            colors[vertexIndex] = currentColor;
            vertexIndex++;
        }
        scale -= scaleDecrease;
    }
}

void generateLinePoints(vec2 vertices[], vec3 colors[], int startVertexIndex) {
    vertices[startVertexIndex] = vec2(-1.0, -1.0);
    vertices[startVertexIndex + 1] = vec2(1.0, 1.0);

    colors[startVertexIndex] = WHITE;
    colors[startVertexIndex + 1] = BLUE;
}


// 锟斤拷锟皆诧拷系牡锟�
vec2 getEllipseVertex(vec2 center, double scale, double verticalScale, double angle) {
    vec2 vertex(sin(angle), cos(angle));
    vertex *= scale;
    vertex += center;
    vertex.y *= verticalScale;
    return vertex;
}

// 锟斤拷锟捷角讹拷锟斤拷锟斤拷锟斤拷色
float generateAngleColor(double angle) {
    return 1.0 / (2 * M_PI) * angle;
}

void generateEllipsePoints(vec2 vertices[], vec3 colors[], int startVertexIndex, int numPoints,
                           vec2 center, double scale, double verticalScale) {
    double angleIncrement = (2 * M_PI) / numPoints;
    double currentAngle = M_PI / 2;

    for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
        vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);

        if (verticalScale == 1.0) {
            colors[i] = vec3(generateAngleColor(currentAngle), 0.0, 0.0);
        } else {
            colors[i] = GREEN;
        }
        currentAngle += angleIncrement;
    }
}


void init() {
    vec2 vertices[TOTAL_NUM_POINTS];
    vec3 colors[TOTAL_NUM_POINTS];

    // 锟斤拷锟缴革拷锟斤拷锟斤拷状锟较的碉拷
    generateTrianglePoints(vertices, colors, 0);
    generateSquarePoints(vertices, colors, SQUARE_NUM, TRIANGLE_NUM_POINTS);
    generateLinePoints(vertices, colors, TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS);

    /*锟斤拷锟斤拷圆锟轿猴拷锟斤拷圆锟较的碉拷锟斤拷锟缴�*/
    vec2 circleCenter(0, 0.45);
    generateEllipsePoints(vertices, colors, TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS + LINE_NUM_POINTS,
                          CIRCLE_NUM_POINTS, circleCenter, 0.26, 1.0);
    generateEllipsePoints(vertices, colors, TOTAL_NUM_POINTS - ELLIPSE_NUM_POINTS,
                          ELLIPSE_NUM_POINTS, vec2(0, 2), 0.08, 0.25);

    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
    GLuint vao[1];
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    // 锟斤拷锟斤拷锟斤拷锟斤拷始锟斤拷锟斤拷锟姐缓锟斤拷锟斤拷锟�
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);

    // 锟街憋拷锟饺★拷锟斤拷锟�
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    // 锟斤拷取锟斤拷色锟斤拷锟斤拷使锟斤拷
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // 锟接讹拷锟斤拷锟斤拷色锟斤拷锟叫筹拷始锟斤拷锟斤拷锟斤拷锟轿伙拷锟�
    GLuint pLocation = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(pLocation);
    glVertexAttribPointer(pLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    // 锟斤拷片元锟斤拷色锟斤拷锟叫筹拷始锟斤拷锟斤拷锟斤拷锟斤拷锟缴�
    GLuint cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

    // 锟斤拷色锟斤拷锟斤拷
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void) {
    // 锟斤拷锟斤拷锟斤拷
    glClear(GL_COLOR_BUFFER_BIT);

    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_POINTS);
    // 锟斤拷锟狡讹拷锟斤拷锟斤拷锟斤拷锟�
    for (int i = 0; i < SQUARE_NUM; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, TRIANGLE_NUM_POINTS + (i * 4), 4);
    }
    // 锟斤拷锟斤拷锟斤拷
    glDrawArrays(GL_LINES, TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS, LINE_NUM_POINTS);

    // 锟斤拷锟斤拷圆
    glDrawArrays(GL_TRIANGLE_FAN, TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS + LINE_NUM_POINTS,
                 CIRCLE_NUM_POINTS);
    // 锟斤拷锟斤拷锟斤拷圆
    glDrawArrays(GL_TRIANGLE_FAN, TOTAL_NUM_POINTS - ELLIPSE_NUM_POINTS,
                 ELLIPSE_NUM_POINTS);

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(800, 800);

    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutCreateWindow("Magic Eyed Robot without hands");

    glewExperimental = GL_TRUE;
    glewInit();

    init();
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
