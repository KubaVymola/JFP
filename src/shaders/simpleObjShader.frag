#version 330 core

in vec3 FragPos;
in vec3 Normal;
in float logz;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    float ambientStrength = 0.3;
    float specularStrength = 0.3;

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max((dot(norm, lightDir) + 1.0) / 2.0, 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 31);
    vec3 specular = specularStrength * spec * lightColor;

    // vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

    gl_FragDepth = logz;
}
