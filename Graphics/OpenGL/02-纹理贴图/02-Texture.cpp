﻿// 02-Texture.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb/stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    //glfwInit函数来初始化GLFW
    glfwInit();
    //glfwWindowHint函数来配置GLFW，第一个参数代表选项的名称，可以从很多以GLFW_开头的枚举值中选择；第二个参数接受一个整型，用来设置这个选项的值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //OpenGL版本是3.3，因此主版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //次版本号为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //使用的是核心模式(Core-profile)

    //创建glfw窗口设置当前上下文设置窗口大小变化回调，glfwCreateWindow函数前两个参数是窗口的宽和高。第三个参数表示这个窗口的名称
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);  //通知GLFW将窗口的上下文设置为当前线程的主上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //注册framebuffer_size_callback函数

    // 加载系统相关的OpenGL函数指针地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("Shaders/shader.vert", "Shaders/shader.frag");

    //顶点数组，以标准化设备坐标的形式定义
    float vertices[] = {
        //位置              颜色               纹理坐标
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
    };

    //索引数组
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    //创建顶点缓冲对象 顶点数组对象 索引缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);  //生成VBO对象
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);  //绑定VAO

    //绑定顶点缓冲区 copy顶点数据到顶点缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //把用户定义的数据复制到当前绑定缓冲

    //绑定顶点索引缓冲区 copy顶点索引数据到顶点索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点属性指针 解释顶点数据 顶点索引获取顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  //解析顶点数据
    glEnableVertexAttribArray(0);  //以顶点属性位置值作为参数，启用顶点属性

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //纹理坐标的顶点属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //生成纹理
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);  //第一个参数是生成纹理的数量，然后存储在第二个参数的数组中
    glBindTexture(GL_TEXTURE_2D, texture1);  //绑定纹理
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //设置环绕方式，第一个参数是纹理目标，第二个参数是设置的选项与应用的纹理轴，最后一个参数是环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //为放大和缩小指定过滤方式，这里都是线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  //将图像上下颠倒

    //stbi_load加载图片，首先接受一个图像文件的位置作为输入，再用图像的宽度、高度和颜色通道的个数填充后面三个变量
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        //glTexImage2D生成纹理，第一个参数是纹理目标，第二个是纹理的多级渐远纹理的级别，第三个是纹理的储存格式，第四个和第五个是纹理的宽度和高度，第六个总为0，第七第八个是源图的格式和数据类型，最后一个参数是真正的图像数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  //为当前绑定的纹理自动生成所有需要的多级渐远纹理
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);  //释放图像的内存

    glGenTextures(1, &texture2);  //第一个参数是生成纹理的数量，然后存储在第二个参数的数组中
    glBindTexture(GL_TEXTURE_2D, texture2);  //绑定纹理
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //设置环绕方式，第一个参数是纹理目标，第二个参数是设置的选项与应用的纹理轴，最后一个参数是环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //为放大和缩小指定过滤方式，这里都是线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //stbi_load加载图片，首先接受一个图像文件的位置作为输入，再用图像的宽度、高度和颜色通道的个数填充后面三个变量
    data = stbi_load("QQ图片2.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        //glTexImage2D生成纹理，第一个参数是纹理目标，第二个是纹理的多级渐远纹理的级别，第三个是纹理的储存格式，第四个和第五个是纹理的宽度和高度，第六个总为0，第七第八个是源图的格式和数据类型，最后一个参数是真正的图像数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  //为当前绑定的纹理自动生成所有需要的多级渐远纹理
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);  //释放图像的内存

    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置每个着色器采样器属于哪个纹理单元
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置

    //解绑VBO
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //解绑VAO
    //glBindVertexArray(0);

    //线框模式，第一个参数表示将其应用到所有的三角形的正面和背面，第二个参数表示用线来绘制
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染循环，在GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))
    {
        // 每一帧判断退出按键是否按下
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);   //清空颜色缓冲       

        //glBindTexture(GL_TEXTURE_2D, texture);  //把纹理赋值给片段着色器的采样器
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();   //使用着色器程序
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);   //解除绑定

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    //glfwGetKey返回这个按键是否正在被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 回调函数，在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // glViewport函数来设置窗口的维度，前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, width, height);
}