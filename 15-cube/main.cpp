#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#define GLSL(x) "#version 140\n" #x

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
using namespace std;

struct vertex_t {
    GLfloat x, y, z;
    GLfloat r, g, b;
    GLfloat u, v;
};

vertex_t vertices[] = {
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },

    {  -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },

    {  -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },

    {   0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },

    {  -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    {  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },

    {  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0 }
};

/* Function called by glfw when an error happens. */
static void error_callback(int error, const char* description);

/* Returns all the content of a file.  Only use this for files you trust not to
 * be too big.  The returned pointer must be freed after its use. */
static char* file_contents(const char* filename);

/* Compiles a shader and returns its handle. */
static GLuint compile_shader(GLenum shader_type, const char* filename);

/* Links a program using an array of shaders with size n. */
static GLuint link_program(GLuint* shaders, int n,
                           const char* fragDataLocation);

int main() {
    glfwInit();
    glfwSetErrorCallback(&error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint shaders[2] = {
        compile_shader(GL_VERTEX_SHADER, "vertex.glsl"),
        compile_shader(GL_FRAGMENT_SHADER, "frag.glsl")
    };

    GLuint program = link_program(shaders, 2, "outColor");
    glUseProgram(program);

    GLint position = glGetAttribLocation(program, "position");
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
    glEnableVertexAttribArray(position);

    GLint color = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                          (void*)(3 * sizeof(float)));

    GLint texcoord = glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                          (void*)(6 * sizeof(float)));

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int width, height;
    unsigned char* image = SOIL_load_image("../sample.png", &width, &height,
                                           0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint modelLocation = glGetUniformLocation(program, "model");
    GLint viewLocation = glGetUniformLocation(program, "view");
    GLint projLocation = glGetUniformLocation(program, "proj");

    glm::mat4 view = glm::lookAt(
        glm::vec3(1.5f, 1.5f, 1.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

    int lastSecond = glfwGetTime();
    int frames = 0;
    while(!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::rotate(glm::mat4(), time * 2,
                                      glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);

        frames++;
        if (lastSecond != (int) time) {
            fprintf(stdout, "%d\n", frames);
            frames = 0;
            lastSecond = time;
        }

        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    glDeleteTextures(1, &tex);
    glDeleteProgram(program);
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();

    return 0;
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "%d: %s\n", error, description);
}

static char* file_contents(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        fprintf(stderr, "Failed to open file \"%s\"", filename);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* contents = new char[file_size + 1];

    fread(contents, sizeof(char), file_size, file);
    return contents;
}

static GLuint compile_shader(GLenum shader_type, const char* filename) {
    GLuint shader = glCreateShader(shader_type);
    const char* source = file_contents(filename);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char error_buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, error_buffer);
        fprintf(stderr, "%s", error_buffer);
    }

    delete [] source;
    return shader;
}

static GLuint link_program(GLuint* shaders, int n,
                           const char* fragDataLocation) {
    GLuint program = glCreateProgram();

    for (int i = 0; i < n; i++) {
        glAttachShader(program, shaders[i]);
    }

    glBindFragDataLocation(program, 0, fragDataLocation);
    glLinkProgram(program);

    return program;
}
