// g++ -Wall -std=c++11 -lGL -lGLEW -lglfw -lSOIL main.cpp -o water

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <iostream> // for debugging

GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
};

GLfloat vertices[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

const GLchar* vertexSource =
    "#version 140\n"
    "\n"
    "in vec2 position;\n"
    "in vec2 texcoord;\n"
    "in vec3 color;\n"
    "out vec3 fragColor;\n"
    "out vec2 fragTexcoord;\n"
    "\n"
    "void main() {\n"
    "    fragTexcoord = texcoord;\n"
    "    fragColor = color;\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "}\n";

const GLchar* fragSource =
    "#version 140\n"
    "\n"
    "in vec3 fragColor;\n"
    "in vec2 fragTexcoord;\n"
    "out vec4 outColor;\n"
    "uniform sampler2D tex;\n"
    "uniform float time;\n"
    "\n"
    "void main() {\n"
    "    if (fragTexcoord.y < 0.5) {\n"
    "        outColor = texture(tex, fragTexcoord);\n"
    "    } else {\n"
    "        vec2 coord;\n"
    "        coord.x = fragTexcoord.x + 0.005 * sin(time * 3 + fragTexcoord.y * 50);\n"
    "        coord.y = 1.0 - fragTexcoord.y;\n"
    "        outColor = texture(tex, coord) * vec4(0.8, 0.8, 0.8, 1);\n"
    "    }\n"
    "}\n";

static void err_callback(int error, const char* description) {
    std::cout << error << ": " << description << std::endl;
}

static void init_glfw() {
    glfwInit();
    glfwSetErrorCallback(err_callback);

    // Should be using 3.2, but Sandy Bridge does not support it.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

static void init_glew() {
    glewExperimental = GL_TRUE;
    glewInit();
}

static GLFWwindow* create_window(int width, int height, const char* title) {
    GLFWwindow* w = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(w);

    return w;
}

static GLuint compile_shader(GLenum type, const GLchar* source) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cerr << buffer << std::endl;
    }

    return shader;
}

static GLuint link_program(GLuint vertexShader, GLuint fragShader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);

    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);

    return program;
}

int main() {
    init_glfw();
    GLFWwindow* window = create_window(800, 600, "OpenGL");
    init_glew();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
                 GL_STATIC_DRAW);

    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragSource);

    GLuint program = link_program(vertexShader, fragmentShader);
    glUseProgram(program);

    GLint posAttrib = glGetAttribLocation(program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          7 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint colorAttrib = glGetAttribLocation(program, "color");
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE,
                          7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(colorAttrib);

    GLint texAttrib = glGetAttribLocation(program, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                          7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

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

    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(program, "time"), glfwGetTime());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glDeleteTextures(1, &tex);

    glDeleteProgram(program);

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
}
