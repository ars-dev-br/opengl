#version 140

in vec2 fragTexcoord;
in vec3 fragColor;

out vec4 outColor;

uniform sampler2D tex;

void main() {
    outColor = vec4(fragColor, 1.0) * texture(tex, fragTexcoord);
}
