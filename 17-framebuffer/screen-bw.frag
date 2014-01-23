#version 140

in vec2 fragTexcoord;
out vec4 outColor;
uniform sampler2D tex;

void main() {
    outColor = texture(tex, fragTexcoord);
    float avg = 0.2126 * outColor.r + 0.7152 * outColor.g + 0.0722 * outColor.b;
    outColor = vec4(avg, avg, avg, 1.0);
}
