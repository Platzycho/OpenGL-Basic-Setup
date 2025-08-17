#version 330 core
out vec4 outColor;

in vec3 fragColor;
in vec2 texCoords;

uniform sampler2D textureSampler;
uniform bool useTexture;

void main() {
    if(useTexture)
    {
        outColor = texture(textureSampler, texCoords);
    } else {
        outColor = vec4(fragColor, 1.0);
    }
    
}