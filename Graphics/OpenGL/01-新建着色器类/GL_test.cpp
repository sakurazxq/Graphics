// GL_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <glad/glad.h>  //在最前面
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";

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

    //// build and compile our shader program
    //// 创建编译顶点着色器
    //int vertexShader = glCreateShader(GL_VERTEX_SHADER);  //创建着色器，把需要创建的着色器类型以参数形式提供给glCreateShader
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  //把这个着色器源码附加到着色器对象上，第二参数指定了传递的源码字符串数量
    //glCompileShader(vertexShader);  //编译
    //// 检查编译是否成功
    //int success;  //表示是否成功编译
    //char infoLog[512];  //存储错误消息的容器
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);  //检查编译是否成功
    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  //获取错误消息
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}
    ////创建编译片段着色器
    //int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    //// check for shader compile errors
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}
    ////创建shader程序 连接顶点片段着色器 连接shader程序，着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本
    //int shaderProgram = glCreateProgram();  //创建一个程序，并返回新创建程序对象的ID引用
    //glAttachShader(shaderProgram, vertexShader);  //把着色器附加到程序上
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);  //链接
    //// 检测链接着色器程序是否失败
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);  //检测链接着色器程序是否失败
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);  //获取错误消息
    //    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    //}
    ////链接完成着shader程序之后删除着色器程序
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    Shader ourShader("Shaders/shader.vert", "Shaders/shader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    //顶点数组，以标准化设备坐标的形式定义
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f // top left 
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
    //第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；
    //用一个简单的sizeof计算出顶点数据大小。第三个参数是希望发送的实际数据。第四个参数指定了显卡如何管理给定的数据，GL_STATIC_DRAW 表示数据不会或几乎不会改变，
    //GL_DYNAMIC_DRAW：数据会被改变很多，GL_STREAM_DRAW ：数据每次绘制时都会改变

    //绑定顶点索引缓冲区 copy顶点索引数据到顶点索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点属性指针 解释顶点数据 顶点索引获取顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //解析顶点数据
    glEnableVertexAttribArray(0);  //以顶点属性位置值作为参数，启用顶点属性

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //解绑VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //解绑VAO
    glBindVertexArray(0);

    //线框模式，第一个参数表示将其应用到所有的三角形的正面和背面，第二个参数表示用线来绘制
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染循环，在GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))  //glfwWindowShouldClose函数在每次循环开始前检查一次GLFW是否被要求退出，如果是的话返回true,然后渲染循环便结束
    {
        // 每一帧判断退出按键是否按下
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);   //清空颜色缓冲

        ourShader.use();   //使用着色器程序

        //ourShader.setFloat("someUniform", 1.0f);

        //float timeValue = glfwGetTime();  //获取运行的秒数
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;  //颜色渐变
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");  //用glGetUniformLocation查询uniform ourColor的位置值
        //glUniform4f(vertexColorLocation, greenValue, 0.0f, 0.0f, 1.0f);  //设置uniform值

        glBindVertexArray(VAO);  //绑定VAO
        //glDrawArrays(GL_TRIANGLES, 0, 3); //需要绑定顶点数据，glDrawArrays函数，使用当前激活的着色器，之前定义的顶点属性配置，
        //和VBO的顶点数据（通过VAO间接绑定）来绘制图元。第一个参数是绘制的OpenGL图元的类型，第二个参数指定了顶点数组的起始索引，最后一个参数指定绘制多少个顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //glDrawElements指明从索引缓冲渲染，第一个参数是绘制的图元的类型，第二个参数是我们打算绘制顶点的个数，
        //第三个参数是索引的类型，最后一个参数指定EBO中的偏移量
        glBindVertexArray(0);   //解除绑定

        glfwSwapBuffers(window);  //glfwSwapBuffers函数会交换颜色缓冲，用双缓冲渲染窗口应用程序，前缓冲保存着最终输出的图像，
        //它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。用双缓冲绘图时可以避免图像闪烁的问题
        glfwPollEvents();  //glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfwTerminate正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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