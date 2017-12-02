#include "mesh.h"
#include <sstream>
#include <fstream>
#include <iosfwd>
#include <algorithm>
#include <GL/gl.h>
#include <math.h>
#include <algorithm>

#define PI 3.1415926
float AngleToRadian = PI / 180;

My_Mesh::My_Mesh() {
    vTranslation[0] = Theta[0] = 0;
    vTranslation[1] = Theta[1] = 0;
    vTranslation[2] = Theta[2] = 0;
    Theta[0] = 45;
}

My_Mesh::~My_Mesh() {}

void My_Mesh::normal_to_color(float nx, float ny, float nz, float &r, float &g, float &b) {
    r = float(std::min(std::max(0.5 * (nx + 1.0), 0.0), 1.0));
    g = float(std::min(std::max(0.5 * (ny + 1.0), 0.0), 1.0));
    b = float(std::min(std::max(0.5 * (nz + 1.0), 0.0), 1.0));
};

const VtList &My_Mesh::get_vts() {
    return this->m_vt_list_;
};

void My_Mesh::clear_data() {
    m_vertices_.clear();
    m_normals_.clear();
    m_faces_.clear();
    m_color_list_.clear();
    m_vt_list_.clear();
};

void My_Mesh::get_boundingbox(point3f &min_p, point3f &max_p) const {
    min_p = this->m_min_box_;
    max_p = this->m_max_box_;
};

const STLVectorf &My_Mesh::get_colors() {
    return this->m_color_list_;
};

const VertexList &My_Mesh::get_vertices() {
    return this->m_vertices_;
};

const NormalList &My_Mesh::get_normals() {
    return this->m_normals_;
};

const FaceList &My_Mesh::get_faces() {
    return this->m_faces_;
};

int My_Mesh::num_faces() {
    return this->m_faces_.size() / 3;
};

int My_Mesh::num_vertices() {
    return this->m_vertices_.size() / 3;
};

const point3f &My_Mesh::get_center() {
    return this->m_center_;
};

// @TODO 此段代码较通用，为避免冗余，将它抽成此函数
// 传入z轴，按cos和sin生成x，y坐标
void My_Mesh::genXY(int num_samples, float step, float z) {
    // 对360度完成num_samples次采样
    for (int i = 0; i < num_samples; i++) {
        // iRadian: 当前i采样点的弧度
        float iRadian = i * step * AngleToRadian;
        float x = cos(iRadian);
        float y = sin(iRadian);

        // 生成一圈的顶点
        m_vertices_.push_back(x);
        m_vertices_.push_back(y);
        m_vertices_.push_back(z);

        m_normals_.push_back(x);
        m_normals_.push_back(y);
        m_normals_.push_back(0);

        // 法线由里向外
        float r, g, b;
        My_Mesh::normal_to_color(x, y, z, r, g, b);

        // 这里采用法线来生成颜色，可以自定义自己的颜色生成方式
        m_color_list_.push_back(r);
        m_color_list_.push_back(g);
        m_color_list_.push_back(b);
    }
}

// @TODO 生成圆盘
void My_Mesh::generate_disk(int num_division) {
    this->clear_data();
    this->m_center_ = point3f(0, 0, 0);
    this->m_min_box_ = point3f(-1, -1, 0);
    this->m_max_box_ = point3f(1, 1, 0);

    int num_samples = num_division;
    float step = 1.0 * 360 / num_samples;

    // @TODO 圆盘Z轴为0(即二维平面)，按cos和sin生成x，y坐标
    float z = 0;
    genXY(num_samples, step, z);

    // 生成三角面片
    for (int i = 0; i < num_samples; i++) {
        // 生成三角面片
        m_faces_.push_back(i);
        m_faces_.push_back((i + 1) % num_samples);
        m_faces_.push_back(num_samples);

        float iRadian = i * step * AngleToRadian;
        float x = (cos(iRadian) + 1) / 2;
        float y = (sin(iRadian) + 1) / 2;
        // 纹理坐标
        m_vt_list_.push_back(x);
        m_vt_list_.push_back(y);

        iRadian = (i + 1) * step * AngleToRadian;
        x = (cos(iRadian) + 1) / 2;
        y = (sin(iRadian) + 1) / 2;
        // 纹理坐标
        m_vt_list_.push_back(x);
        m_vt_list_.push_back(y);

        // 纹理坐标
        m_vt_list_.push_back(0.5);
        m_vt_list_.push_back(0.5);
    }

    m_faces_.push_back(0);
    m_faces_.push_back(0);
    m_faces_.push_back(0);
};

// @TODO 生成圆锥体
void My_Mesh::generate_cone(int num_division, float height) {
    this->clear_data();
    this->m_center_ = point3f(0, 0, height / 2);
    this->m_min_box_ = point3f(-1, -1, 0);
    this->m_max_box_ = point3f(1, 1, height);

    int num_samples = num_division;
    float step = 1.0 * 360 / num_samples;

    // @TODO 圆锥体底面部分
    // 圆盘Z轴为0(即二维平面)，按cos和sin生成x，y坐标
    float z = 0;
    genXY(num_samples, step, z);

    // @TODO 圆锥体顶点部分
    m_vertices_.push_back(0);
    m_vertices_.push_back(0);
    m_vertices_.push_back(height);

    m_normals_.push_back(0);
    m_normals_.push_back(0);
    m_normals_.push_back(1);

    // 法线由里向外
    float r, g, b;
    My_Mesh::normal_to_color(0, 0, 1, r, g, b);

    // 采用法线来生成颜色
    m_color_list_.push_back(r);
    m_color_list_.push_back(g);
    m_color_list_.push_back(b);

    for (int i = 0; i < num_samples; i++) {
        // 生成三角面片
        m_faces_.push_back(num_samples);
        m_faces_.push_back(i % num_samples);
        m_faces_.push_back((i + 1) % num_samples);

        // 纹理坐标
        // @TODO 裁剪三角形纹理，避开边界白线
        m_vt_list_.push_back(0.5);
        m_vt_list_.push_back(.98);

        m_vt_list_.push_back(.99 * i / num_samples);
        m_vt_list_.push_back(0.0);

        m_vt_list_.push_back(.99 * (i + 1) / num_samples);
        m_vt_list_.push_back(0.0);
    }
};

// 生成圆柱体
void My_Mesh::generate_cylinder(int num_division, float height) {
    this->clear_data();
    this->m_center_ = point3f(0, 0, 0);
    this->m_min_box_ = point3f(-1, -1, -height);
    this->m_max_box_ = point3f(1, 1, height);

    int num_samples = num_division;
    float step = 1.0 * 360 / num_samples;

    // @TODO 圆柱体Z轴向上，按cos和sin生成x，y坐标
    float z = -height;
    genXY(num_samples, step, z);

    z = height;
    genXY(num_samples, step, z);

    // 生成三角面片
    for (int i = 0; i < num_samples; i++) {
        // 由四个顶点生成2个三角面片
        m_faces_.push_back(i);
        m_faces_.push_back((i + 1) % num_samples);
        m_faces_.push_back(i + num_samples);

        m_faces_.push_back(i + num_samples);
        m_faces_.push_back((i + 1) % num_samples);
        m_faces_.push_back((i + num_samples + 1) % (num_samples) + num_samples);

        // 纹理坐标
        // 将0-360度映射到UV坐标的0-1

        m_vt_list_.push_back(1.0 * i / num_samples);
        m_vt_list_.push_back(0.0);

        m_vt_list_.push_back(1.0 * (i + 1) / num_samples);
        m_vt_list_.push_back(0.0);

        m_vt_list_.push_back(1.0 * i / num_samples);
        m_vt_list_.push_back(1.0);


        m_vt_list_.push_back(1.0 * i / num_samples);
        m_vt_list_.push_back(1.0);

        m_vt_list_.push_back(1.0 * (i + 1) / num_samples);
        m_vt_list_.push_back(0.0);

        m_vt_list_.push_back(1.0 * (i + 1) / num_samples);
        m_vt_list_.push_back(1.0);
    }
};


void My_Mesh::set_texture_file(std::string s) {
    this->texture_file_name = s;
};

std::string My_Mesh::get_texture_file() {
    return this->texture_file_name;
};

void My_Mesh::set_translate(float x, float y, float z) {
    vTranslation[0] = x;
    vTranslation[1] = y;
    vTranslation[2] = z;
};

void My_Mesh::get_translate(float &x, float &y, float &z) {
    x = vTranslation[0];
    y = vTranslation[1];
    z = vTranslation[2];
};

void My_Mesh::set_theta(float x, float y, float z) {
    Theta[0] = x;
    Theta[1] = y;
    Theta[2] = z;
};

void My_Mesh::get_theta(float &x, float &y, float &z) {
    x = Theta[0];
    y = Theta[1];
    z = Theta[2];
};

void My_Mesh::set_theta_step(float x, float y, float z) {
    Theta_step[0] = x;
    Theta_step[1] = y;
    Theta_step[2] = z;
};

void My_Mesh::add_theta_step() {
    Theta[0] = Theta[0] + Theta_step[0];
    Theta[1] = Theta[1] + Theta_step[1];
    Theta[2] = Theta[2] + Theta_step[2];
};
