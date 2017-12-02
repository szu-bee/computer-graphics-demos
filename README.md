# computer-graphics-demos

Put my computer graphics projects here.

### Usage
After Environment Configuration finished, just open a project in clion, and run it!

注：我的环境是Ubuntu16.04，OpenGL环境配置如下：

只需几行命令：
1. 安装基本编译环境
```
sudo apt install build-essential 
```

2. 安装 OpenGL Library、OpenGL Utilities

OpenGL Utilities 是一组建构于 OpenGL Library 之上的工具组，提供许多很方便的函式，使 OpenGL 更强大且更容易使用。
```
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
```

3. 安装 OpenGL Utility Toolkit、glew

OpenGL Utility Toolkit 是建立在 OpenGL Utilities 上面的工具箱，除了强化了 OpenGL Utilities 的不足之外，也增加了 OpenGL 对于视窗界面支援。
glew 是一个跨平台的 C++库，是一个 OpenGL 图形接口扩展库。
```
sudo apt install freeglut3-dev
```
先用 apt-cache search glew 命令找到 glew 的相关包，再安装列出的所有包。

4. 环境配置完成，使用 clion 开发（clion 是一种流行的编写 C/C++的 IDE，下载地址：
https://www.jetbrains.com/clion/），只需将文件复制都项目工程文件夹下，再修改
CMakeLists.txt 文件，等待自动加载完成即可。

test1 的 CMakeLists.txt 文件配置如下：
```
cmake_minimum_required(VERSION 3.7)
project(test1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ./Common/InitShader.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ./include)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${GLUT_LIBRARY})
```