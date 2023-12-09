#version 330 core
layout (location = 3) in vec3 aPos;
layout (location = 4) in vec3 aNormal;
layout (location = 5) in vec2 aTex;
out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normMat;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	normal = normMat * aNormal;
	texCoord = aTex;
}