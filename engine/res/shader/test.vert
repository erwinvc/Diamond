#version 460

layout (location = 0) in vec3 vsPosition;
//in vec3 vsNormal;
//in vec2 vsUv;
//in vec3 vsTangent;
//in vec3 vsBitangent;

out vec2 fsUv;

void main() {
    gl_Position = vec4(vsPosition, 1);
    fsUv = vec2((vsPosition.x+1.0)/2.0, (vsPosition.y+1.0)/2.0);
}

