#version 330 core

in vec3 fragPos;
in vec3 fragNormal;

out vec4 FragColor;

void main() {
    // Material properties
    vec3 ka = vec3(1.0);
    vec3 kd = vec3(1.0);
    vec3 ks = vec3(0.0);     // No specular
    float p = 0.0;           // Shininess doesn't matter
    vec3 ambientIntensity = vec3(0.2); // Ia

    // Light properties
    vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));
    vec3 lightDiffuse = vec3(1.0);
    vec3 lightSpecular = vec3(0.0); // No specular
    vec3 lightAmbient = vec3(0.0);  // Directional light has no ambient

    // Normal
    vec3 norm = normalize(fragNormal);

    // Ambient
    vec3 ambient = ka * ambientIntensity;

    // Diffuse
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = kd * lightDiffuse * diff;

    // Specular (omitted due to ks = 0)
    vec3 specular = vec3(0.0);

    // Final color
    vec3 result = ambient + diffuse + specular;

    // Optional: gamma correction
    vec3 gammaCorrected = pow(clamp(result, 0.0, 1.0), vec3(1.0 / 2.2));
    FragColor = vec4(gammaCorrected, 1.0);
}
