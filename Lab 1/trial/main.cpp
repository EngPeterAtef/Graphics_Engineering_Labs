#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

GLuint load_shader(const string &path, GLenum shader_type)
{
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Failed to open file" << endl;
        return 0;
    }
    string source = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    const char *source_c = source.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source_c, nullptr);
    glCompileShader(shader);
    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
        string error_log(max_length, ' ');
        glGetShaderInfoLog(shader, max_length, &max_length, &error_log[0]);
        cerr << "Failed to compile shader" << endl;
        cerr << error_log << endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int main()
{
    if (!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }
    // to use openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        cerr << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // tell that the load for created window it's important if we have multiple windows
    if (!gladLoadGL(glfwGetProcAddress))
    {
        cerr << "Failed to load openGL" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    } // parameter bta3 al function de hoa function

    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//to set the configuration : ya3ni da al color ali hyt7t lma a3ml clear

    GLuint program = glCreateProgram();
    GLuint vs = load_shader("./assets/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = load_shader("./assets/simple.frag", GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    // m4 m7tgen dol tani 3la al shader
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array); // 1 vertex array

    while (!glfwWindowShouldClose(window))
    {
        float time = (float)glfwGetTime();
        glClearColor(0.5 * sinf(time) + 0.5,
                        0.5 * sinf(time + 1) + 0.5,
                        0.5 * sinf(time + 2) + 0.5,
                        0.5); // to set the configuration : ya3ni da al color ali hyt7t lma a3ml clear
        glClear(GL_COLOR_BUFFER_BIT); // order clear the back buffer
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 vertices (3 points)

        glfwSwapBuffers(window); // bugib al back buffer lma a3ml clear
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
