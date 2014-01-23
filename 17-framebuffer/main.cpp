#include "renderer.hpp"

static void error_callback(int error, const char* description);

int main() {
    Renderer renderer;
    renderer.init(&error_callback);

    InputHandler inputHandler = renderer.getInputHandler();

    int lastSecond = glfwGetTime();
    int frames = 0;
    while(!renderer.shouldClose()) {
        renderer.render();

        double time = glfwGetTime();
        frames++;

        if (lastSecond != (int) time) {
            fprintf(stdout, "%d\n", frames);
            frames = 0;
            lastSecond = time;
        }

        inputHandler.poll();
    }

    return 0;
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "%d: %s\n", error, description);
}
