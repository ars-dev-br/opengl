#ifndef RENDERER_H
#define RENDERER_HPP 1

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#include "data.hpp"
#include "input_handler.hpp"

using namespace std;

class Renderer {
private:
    GLFWwindow* window;

    GLuint cubeBuffer;
    GLuint screenBuffer;

    GLuint cubeVertexArray;
    GLuint screenVertexArray;

    GLuint cubeVertexShader;
    GLuint cubeFragShader;
    GLuint cubeProgram;

    GLuint screenVertexShader;
    GLuint screenFragShader;
    GLuint screenProgram;

    GLuint cubeTexture;

    GLuint frameBuffer;
    GLuint colorBuffer;
    GLuint depthStencilBuffer;

    GLuint loadTexture(const char* filename) {
        GLuint texture = 0;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        int w, h;
        unsigned char* image = SOIL_load_image("../sample.png", &w, &h, 0,
                                               SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture;
    }

    GLuint compileShader(GLenum shaderType, const char* filename) {
        FILE* file = fopen(filename, "r");
        if (file == nullptr) {
            fprintf(stderr, "Failed to open file \"%s\"\n", filename);
            return 0;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* source = new char[file_size + 1];
        memset(source, 0, file_size * sizeof(char) + 1);
        fread(source, sizeof(char), file_size, file);

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, (const char**) &source, nullptr);
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            char error_buffer[512];
            glGetShaderInfoLog(shader, 512, nullptr, error_buffer);
            fprintf(stderr, "On shader \"%s\": %s", filename, error_buffer);
        }

        delete [] source;
        return shader;
    }

    GLuint linkProgram(GLuint vertex, GLuint frag, const char* fragData) {
        GLuint program = glCreateProgram();

        glAttachShader(program, vertex);
        glAttachShader(program, frag);

        glBindFragDataLocation(program, 0, fragData);
        glLinkProgram(program);

        return program;
    }

    void openglInit(GLFWerrorfun errorCallback) {
        glfwInit();
        glfwSetErrorCallback(errorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        glewInit();

        glfwSwapInterval(0);
    }

    void cubeInit() {
        glGenBuffers(1, &cubeBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE), CUBE, GL_STATIC_DRAW);

        glGenVertexArrays(1, &cubeVertexArray);
        glBindVertexArray(cubeVertexArray);

        cubeVertexShader = compileShader(GL_VERTEX_SHADER, "cube.vert");
        cubeFragShader = compileShader(GL_FRAGMENT_SHADER, "cube.frag");
        cubeProgram = linkProgram(cubeVertexShader, cubeFragShader, "outColor");

        cubeTexture = loadTexture("../sample.png");

        GLint position = glGetAttribLocation(cubeProgram, "position");
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
                              8 * sizeof(float), 0);

        GLint color = glGetAttribLocation(cubeProgram, "color");
        glEnableVertexAttribArray(color);
        glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(3 * sizeof(float)));

        GLint texcoord = glGetAttribLocation(cubeProgram, "texcoord");
        glEnableVertexAttribArray(texcoord);
        glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE,
                              8 * sizeof(float), (void*)(6 * sizeof(float)));


    }

    void screenInit() {
        glGenBuffers(1, &screenBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, screenBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN), SCREEN, GL_STATIC_DRAW);

        glGenVertexArrays(1, &screenVertexArray);
        glBindVertexArray(screenVertexArray);

        screenVertexShader = compileShader(GL_VERTEX_SHADER, "screen.vert");
        screenFragShader = compileShader(GL_FRAGMENT_SHADER, "screen.frag");
        screenProgram = linkProgram(screenVertexShader, screenFragShader,
                                    "outColor");

        GLint position = glGetAttribLocation(screenProgram, "position");
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float), 0);

        GLint texcoord = glGetAttribLocation(screenProgram, "texcoord");
        glEnableVertexAttribArray(texcoord);
        glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(float), (void*)(2 * sizeof(float)));

    }

    void frameBufferInit() {
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, colorBuffer, 0);

        glGenRenderbuffers(1, &depthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, depthStencilBuffer);
    }

    void cubeRender() {
        float time = glfwGetTime();

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glBindVertexArray(cubeVertexArray);
        glUseProgram(cubeProgram);

        glm::mat4 model = glm::rotate(glm::mat4(), time * 2,
                                      glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 view = glm::lookAt(glm::vec3(2.5f, 2.5f, 1.5f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

        glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"), 1,
                           GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "proj"), 1,
                           GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1,
                           GL_FALSE, glm::value_ptr(model));

        glEnable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw main cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_STENCIL_TEST);
        {
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilMask(0xff);
            glDepthMask(GL_FALSE);
            glClear(GL_STENCIL_BUFFER_BIT);

            // Draw floor
            glDrawArrays(GL_TRIANGLES, 36, 6);

            glStencilFunc(GL_EQUAL, 1, 0xff);
            glStencilMask(0x00);
            glDepthMask(GL_TRUE);
            glUniform3f(glGetUniformLocation(cubeProgram, "override"),
                        0.35f, 0.35f, 0.35f);

            model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)),
                               glm::vec3(1, 1, -1));
            glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1,
                               GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glUniform3f(glGetUniformLocation(cubeProgram, "override"),
                        1.0f, 1.0f, 1.0f);
        }
        glDisable(GL_STENCIL_TEST);
    }

    void screenRender() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(screenVertexArray);
        glUseProgram(screenProgram);
        glDisable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

public:
    Renderer()
        : window(nullptr), cubeBuffer(0), screenBuffer(0), cubeVertexArray(0),
          screenVertexArray(0), cubeVertexShader(0), cubeFragShader(0),
          cubeProgram(0), screenVertexShader(0), screenFragShader(0),
          screenProgram(0), cubeTexture(0) {
    }

    ~Renderer() {
        glDeleteTextures(1, &cubeTexture);

        glDeleteFramebuffers(1, &frameBuffer);
        glDeleteTextures(1, &colorBuffer);
        glDeleteRenderbuffers(1, &depthStencilBuffer);

        glDeleteShader(cubeVertexShader);
        glDeleteShader(cubeFragShader);
        glDeleteProgram(cubeProgram);

        glDeleteShader(screenVertexShader);
        glDeleteShader(screenFragShader);
        glDeleteProgram(screenProgram);

        glDeleteVertexArrays(1, &cubeVertexArray);
        glDeleteVertexArrays(1, &screenVertexArray);

        glDeleteBuffers(1, &cubeBuffer);
        glDeleteBuffers(1, &screenBuffer);

        glfwDestroyWindow(window);
        window = nullptr;

        glfwTerminate();
    }

    void init(GLFWerrorfun errorCallback) {
        openglInit(errorCallback);

        cubeInit();
        screenInit();
        frameBufferInit();
    }

    void render() {
        cubeRender();
        screenRender();

        glfwSwapBuffers(window);
    }

    void close() {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }

    InputHandler getInputHandler() {
        return InputHandler(window);
    }
};

#endif
