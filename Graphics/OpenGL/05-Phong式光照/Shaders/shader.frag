#version 330 core
    out vec4 FragColor;

    in vec3 Normal;
    in vec3 FragPos;

    struct Material{
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;  //影响镜面高光的散射/半径
    };

    struct Light{
        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform Material material;
    uniform Light light;

    void main()
    {
       //环境光
       vec3 ambient = material.ambient * light.ambient;

       //漫反射
       vec3 norm = normalize(Normal);
       vec3 lightDir = normalize(lightPos - FragPos);  //光照方向
       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffuse = light.diffuse * (diff * material.diffuse);

       //镜面光
       vec3 viewDir = normalize(viewPos - FragPos);  //视线方向
       vec3 reflectDir = reflect(-lightDir, norm);  //反射方向
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
       vec3 specular = light.specular * (spec * material.specular);
       
       vec3 result = ambient + diffuse + specular;
       FragColor = vec4(result, 1.0);
    }