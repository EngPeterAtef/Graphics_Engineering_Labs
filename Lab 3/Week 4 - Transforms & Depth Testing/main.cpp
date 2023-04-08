#include <iostream>
#include <glad/gl.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Color {
    uint8_t r, g, b, a;
};

struct Vertex {
    glm::vec3 position;
    Color color;
};

GLuint load_shader(const std::string& path, GLenum shader_type) {
    std::ifstream file(path);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCodeCStr = sourceCode.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::cerr << "ERROR IN " << path << std::endl;
        std::cerr << logStr << std::endl;
        delete[] logStr;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

int main() {
    
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);

    if(!window){
        std::cerr << "Failed to create Window\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress)){
        std::cerr << "Failed to load OpenGL\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    GLuint program = glCreateProgram();
    GLuint vs = load_shader("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = load_shader("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    Vertex vertices[4] = {
        {{-0.5, -0.5, 0.0},  {255, 0, 0, 255}},
        {{ 0.5, -0.5, 0.0},  {0, 255, 0, 255}},
        {{ 0.5,  0.5, 0.0},  {0, 0, 255, 255}},
        {{-0.5,  0.5, 0.0},  {255, 255, 0, 255}},
    };

    uint16_t elements[6] = { 0, 1, 2, 2, 3, 0 };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), vertices, GL_STATIC_DRAW);

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint16_t), elements, GL_STATIC_DRAW);


    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    GLint position_loc = 0; //glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, false, sizeof(Vertex), 0);

    GLint color_loc = 1; //glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

    glBindVertexArray(0);

    GLint mvp_loc = glGetUniformLocation(program, "MVP");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);

    while(!glfwWindowShouldClose(window)){
        float time = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vertex_array);

        glm::mat4 P = glm::perspective(glm::pi<float>()*0.5f, 640.0f/480.0f, 0.01f, 1000.0f);
        glm::mat4 V = glm::lookAt(
            glm::vec3(2.0f * cosf(time), 1.0f, 2.0f * sinf(time)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        for(int i = -2; i <= 2; i++){
            glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)i));

            glm::mat4 MVP = P * V * M;

            glUniformMatrix4fv(mvp_loc, 1, false, &MVP[0][0]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &element_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
