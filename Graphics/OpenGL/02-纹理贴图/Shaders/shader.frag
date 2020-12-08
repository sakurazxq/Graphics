#version 330 core
    out vec4 FragColor;

    in vec3 ourColor;
    in vec2 TexCoord;

    uniform sampler2D texture1;
    uniform sampler2D texture2;

    void main()
    {
        //mix是插值函数
       FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4);  //texture采样纹理的颜色，第一个参数是纹理采样器，第二个参数是对应的纹理坐标
    }