#version 140

in vec3 position;
in vec3 color;
in vec2 texcoord;

out vec2 fragTexcoord;
out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    fragColor = color;
    fragTexcoord = texcoord;
    gl_Position = proj * view * model * vec4(position, 1.0);
}
