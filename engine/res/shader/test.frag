#version 460

in vec2 fsUv;
  
out vec3 outColor;
  
void main() {
	outColor = vec3(fsUv.x, fsUv.y, 0);
}  