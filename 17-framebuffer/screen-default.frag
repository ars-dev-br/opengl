#version 140

in vec2 fragTexcoord;
out vec4 outColor;
uniform sampler2D tex;

void main() {
    outColor = texture(tex, fragTexcoord);
}
