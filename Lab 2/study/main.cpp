#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

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

struct positions
{
    float x, y, z;
};

struct colors
{
    uint8_t r, g, b, a;
};

struct Vertex
{
    positions pos;
    colors col;
};

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
    // m4 m7tgen dol tani 3la al shader
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // triangle vertices
    // float position[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f
    //     };
    // float vertices[] = {
    //     -0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,
    //     /////////////////////
    //     0.5f,
    //     -0.5f,
    //     0.0f,
    //     0.0f,
    //     0.0f,
    //     1.0f,
    //     1.0f,
    //     /////////////////////
    //     0.5f,
    //     0.5f,
    //     0.0f,
    //     0.0f,
    //     1.0f,
    //     1.0f,
    //     1.0f,
    // };
    // vertex for triangle
    // Vertex vertices[] = {
    //     {{-0.5f, -0.5f, 0.0f}, {255, 0, 0, 255}},
    //     {{0.5f, -0.5f, 0.0f}, {0, 0, 255, 255}},
    //     {{0.5f, 0.5f, 0.0f}, {0, 255, 0, 255}}};
    // Vertex for square
    // Vertex vertices[] = {
    //     {{-0.5f, -0.5f, 0.0f}, {255, 0, 0, 255}},
    //     {{0.5f, -0.5f, 0.0f}, {0, 0, 255, 255}},
    //     {{0.5f, 0.5f, 0.0f}, {0, 255, 0, 255}},
    //     {{-0.5f, 0.5f, 0.0f}, {0, 255, 255, 255}},
    //     {{-0.5f, -0.5f, 0.0f}, {255, 0, 0, 255}},
    //     {{0.5f, 0.5f, 0.0f}, {0, 255, 0, 255}}
    //     };
    Vertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {0, 255, 255, 255}},
        {{0.5f, -0.5f, 0.0f}, {0, 0, 255, 255}},
        {{-0.5f, 0.5f, 0.0f}, {0, 255, 0, 255}},
        {{0.5f, 0.5f, 0.0f}, {255, 0, 0, 255}},
    };

    uint16_t elements[] = {
        0, 1, 2, // kda the vertex 0,1,2 will be connected to make the first triangle
        3, 1, 2  // the vertex 2,3,0 will be connected to make the second triangle
    };

    GLuint vertex_buffer;
    // initialize the vertex buffer and put the name (id) of the vertex buffer in the vertex_buffer variable as it's just unsigned int
    glGenBuffers(1, &vertex_buffer); // 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, size of the data , the data of the whole data , GL_STATIC_DRAW or GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // GL_STATIC_DRAW : the data will most likely not change at all or very rarely.

    // this buffer is used to store the elements of the vertex
    GLuint element_buffer;
    // initialize the vertex buffer and put the name (id) of the vertex buffer in the vertex_buffer variable as it's just unsigned int
    glGenBuffers(1, &element_buffer); // create 1 buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, size of the data , the data of the whole data , GL_STATIC_DRAW or GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    // GL_STATIC_DRAW : the data will most likely not change at all or very rarely.

    /*
    NOTE: that not always the contigous bindings overwrite each other
    madam awel bind GL_ARRAY_BUFFER wel tanya GL_ELEMENT_ARRAY_BUFFER yb2a m4 hyl8o b3d 34an kol wa7da
    bt3ml bind 3la buffer mo5tlf
    */

    // The vertex array is an object resposible fro dealing with the data in the buffers and reading them
    GLuint vertex_array;
    // this line put the name (id) of the vertex array in the vertex_array variable as it's just unsigned int
    // and this line is the one that initialize the vertex array
    glGenVertexArrays(1, &vertex_array); // 1 vertex array
    // bind the vertex array to the current context to read and write data from it
    glBindVertexArray(vertex_array); // this bind tells the vertex_array that the next orders are for it

    // this bind is not necessary but it's good to do it to make sure that GL_ARRAY_BUFFER is bounded to the vertex_buffer
    // hoa m4 lazm 34an a7na 3amlen bind GL_ARRAY_BUFFER m3 al vertex_buffer fo2 we mafi4 7aga a5dt mkano
    // but it's for the sake of being sure
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // note that the vertex_array knew which buffer has the data of position and color from binding the vertex_buffer to GL_ARRAY_BUFFER

    // get the location of the position in the shader
    GLuint position_loc = 0; // glGetAttribLocation(program, "position");
    // enable the position attribute to tell that we will send data to it from the vertex buffer
    // if we didn't enable it the data will read zeros all the time even you send data to it
    glEnableVertexAttribArray(position_loc);
    // tell the position attribute how to read the data from the vertex buffer
    // glVertexAttribPointer(position_loc, size of each chunck of data , type of the data , normalize the data , stride , offset);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    // 3 : means in each vec3 in position there are 3 floats x,y,z belongs to the same vec3
    // GL_FLOAT : means the type of the data is float
    // GL_FALSE : means we don't want to normalize the data
    // 3 * sizeof(float) : means the stride is 3 floats which means the next vec3 in position is 3 floats after the current vec3
    // 0 : means the offset is 0
    GLuint color_loc = 1; // glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    // here normalized attribute is true because the data is unsigned byte and we want to normalize it to be between 0 and 1
    // to get the required color
    //  (void *)offsetof(Vertex, col) : means the offset is the offset of the col attribute in the Vertex struct
    // ya3ni ybd2 mn awel al col ali fel struct Vertex

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer); // bind the element buffer to get the elements from it
    // this line isn't necessary as you will need to unbind the vertex array only if you need to bind another vertex array
    // or there is an orders would affect the vertex array but here it's not necessary
    glBindVertexArray(0); // unbind the vertex array
    /*
    NOTE:
    if you want to use one buffer for the vertecies and the elements you will need to bind
    the same buffer with the GL_ELEMENT_ARRAY_BUFFER and GL_ARRAY_BUFFER and also change the offset
    in drawing using the elements "glDrawElements"
    =>but it's better to use to buffers
    */

    GLuint time_loc = glGetUniformLocation(program, "time"); // get the location of the time in the shader
    cout << "Time location is " << time_loc << endl;

    while (!glfwWindowShouldClose(window))
    {
        float time = (float)glfwGetTime();
        glClearColor(0, 0, 0, 1.0f);  // to set the configuration : ya3ni da al color ali hyt7t lma a3ml clear
        glClear(GL_COLOR_BUFFER_BIT); // order clear the back buffer
        glUseProgram(program);

        glBindVertexArray(vertex_array); // bind the vertex array to read the data from it

        glUniform1f(time_loc, time); // this line send the time to the time_loc in the shader
        // glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 vertices (3 points) for the first triangle
        // GL_TRIANGLES : means we will draw triangles
        // 0 : means start drawing from the first vertex
        // 3 : means draw 3 vertices (3 points)
        glUniform1f(time_loc, time + 2); // send the time again to give diffrent value to the second triangle
        // glDrawArrays(GL_TRIANGLES, 3, 3); // 3 vertices (3 points) for the second triangle
        // GL_TRIANGLES : means we will draw triangles
        // 3 : means start drawing from the third vertex
        // 3 : means draw 3 vertices (3 points)

        // to draw the square
        // glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 6 vertices (6 points) for the square

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *)0); // draw the square using the elements
        // 6: is the number of elements NOT the number of vertices
        // GL_UNSIGNED_SHORT: is the type of the elements 34an a7na 3amlnhom uint16
        //  (void*)0 : is the offset of the elements in the element buffer 34an ybd2 mn awel al buffer

        glfwSwapBuffers(window); // bygib al back buffer mkan al front buffer
        glfwPollEvents();        // to check if there is any event like keyboard or mouse
    }

    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
