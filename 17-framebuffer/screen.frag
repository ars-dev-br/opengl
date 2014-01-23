#version 140

in vec2 fragTexcoord;
out vec4 outColor;
uniform sampler2D tex;

const float blurSizeH = 1.0 / 600.0;
const float blurSizeV = 1.0 / 400.0;
void main() {
    vec4 sum = vec4(0.0);
    vec2 coord = fragTexcoord;
    for (int x = -4; x <= 4; x++) {
        for (int y = -4; y <= 4; y++) {
            sum += texture(tex, vec2(coord.x + x * blurSizeH,
                                     coord.y + y * blurSizeV)) / 81.0;
        }
    }

    outColor = sum;
}
