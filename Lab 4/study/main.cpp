#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
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

struct Color
{
    uint8_t r, g, b, a;
};

struct Vertex
{
    vec3 pos;
    Color col; // m5lna4 al color vec3 34an da m4 floats
    vec2 tex_coord;
};

float width = 640, height = 480;
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

    GLFWwindow *window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
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
    // m4 m7tgen al shader tani after linking the program
    glDeleteShader(vs);
    glDeleteShader(fs);

    // the vectices and their colors
    Vertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {255, 0, 0, 255}, {0, 0}},
        {{0.5f, -0.5f, 0.0f}, {0, 0, 255, 255},{1.0,0} },
        {{0.5f, 0.5f, 0.0f}, {0, 255, 0, 255},{1.0,1.0}},
        {{-0.5f, 0.5f, 0.0f}, {0, 255, 255, 255},{0,1.0}},
    };

    uint16_t elements[] = {
        0, 1, 2, // kda the vertex 0,1,2 will be connected to make the first triangle
        2, 3, 0  // the vertex 2,3,0 will be connected to make the second triangle
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
    NOTE: not always the contigous bindings overwrite each other
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

    GLuint texcoord_loc = 2; // glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(texcoord_loc);
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));


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

    // glViewport(0, 0, width, height); // to set the configuration : ya3ni da al viewport ali hyt7t lma a3ml clear
    /*
    al clear color malo4 da3wa be al viewport ya3ni lma a3ml clear color al 4a4a kolha hata5od al color ali na 3aizo
    Note that:
    glViewport(0, 0, width, height);
    even you did that after incraesing the size of the window the viewport will be the same
    */

    // glEnable(GL_SCISSOR_TEST);
    // glScissor(0, 0, 320, 240);
    /*
    the difference between the viewport and the scissor is that the scissor will prevent the drawing
    outside the scissor area and the viewport will just change the viewport area
    but why do we need viewport if we have scissor?
    => because the viewport is used to set the area of the screen that will be drawn on
    and the scissor is used to prevent the drawing outside the scissor area
    if we removed viewport we will draw on the whole screen as it's the default but we will see on the screen
    the viewport area only
    */
    GLint mvp_loc = glGetUniformLocation(program, "MVP");
    glEnable(GL_DEPTH_TEST);       // enable the test buffer to use it
    glDepthFunc(GL_LESS);          // to tell that the point with the smaller depth is the one that will be drawn
    glClearColor(0, 0.5, 0, 1.0f); // to set the configuration : ya3ni da al color ali hyt7t lma a3ml clear
    glClearDepth(1.0f);            // to set the configuration : ya3ni da al depth ali hyt7t lma a3ml clear
    // 1.0 ya3ni ab3d depth 3ndi 34an al depth [0,1] 0 2a2rb 7aga
    // lo 5lat al defaualt 0.0 hyzhr screen soda 34an ai 7aga leha depth
    
    Color B = { 0, 0, 0, 255 };
    Color W = { 255, 255, 255, 255 };
    Color Y = { 255, 255, 0, 255 };

    // 8 * 8 img
    Color img[] = {
        W,W,Y,Y,Y,Y,W,W,
        W,Y,Y,Y,Y,Y,Y,W,
        W,Y,Y,B,B,Y,Y,W,
        Y,Y,B,Y,Y,B,Y,Y,
        Y,Y,Y,Y,Y,Y,Y,Y,
        Y,Y,B,Y,Y,B,Y,Y,
        Y,Y,B,Y,Y,B,Y,Y,
        W,W,Y,Y,Y,Y,W,W,
    };
    //note that howa by2ra al image mn ta7t le fo2 ya3ni a5er row ana katbo blnsbalo awel row 
    //34an kda lazm aktb al sora ma3ksoa



    GLuint texture;
    glGenTextures(1, &texture);//1 is number of textures
    //this bind to send the img to gpu
    glBindTexture(GL_TEXTURE_2D, texture);
    //to send the the image to gpu
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    /*
    GL_TEXTURE_2D: means the type of the texture
    0: means the mipmap level   (0 is the base level)
    GL_RGBA8: means the format of the texture
    8: means the width of the texture
    8: means the height of the texture
    0: means the border (always 0)
    GL_RGBA: means the format of the data
    GL_UNSIGNED_BYTE: means the type of the data
    img: means the data
    */
    
    //this function to generate the mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    /*
    the filter of the texture by default interpolate between the pixels to get the colors in between but to stop that
    write the following lines
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //GL_TEXTURE_MIN_FILTER: means the filter of the texture when the texture is smaller than the pixel
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//magnification
    //GL_TEXTURE_MAG_FILTER: means the filter of the texture when the texture is bigger than the pixel
    //GL_NEAREST: means that the texture will be pixelated

    //Now we need to send the texture to the frag shader as a uniform
    GLuint texture_loc = glGetUniformLocation(program, "tex");


    while (!glfwWindowShouldClose(window))
    {
        int h,w;
        glfwGetFramebufferSize(window, &w, &h); // to get the size of the window (the size of the framebuffer
        glViewport(0, 0, w, h);
        width = w;
        height = h;

        float time = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // order clear the back buffer and the depth buffer
        /*
        lo m4 bn3ml clear al frames hattrsm 3la b3d
        */

        glUseProgram(program);

        glBindVertexArray(vertex_array); // bind the vertex array to read the data from it

        //this 
        //this line is so useful if we have more than one texture on multiple texture channels
        glActiveTexture(GL_TEXTURE0); // to activate the texture channel 0
        
        // this bind to send the data of the texture to gpu 
        //so we need to bind the texture to the texture channel before using it 
        glBindTexture(GL_TEXTURE_2D, texture); // to bind the texture to the texture channel 0

        glUniform1i(texture_loc, 0); // to send the texture to the frag shader as one integer  

        // Model
        // mat4 M ;//= mat4(1.0f); // identity matrix
        /*
        this matrix determines the position and the rotation of the object in the game world
        if it's identity matrix the object will be in the origin and it's not rotated
        */

        // View
        mat4 V = lookAt(vec3(2.0 * cosf(time), 0, 2.0 * sinf(time)), vec3(0, 0, 0), vec3(0, 1, 0));
        /*
        from world to camera space
        vec3(2.0 * cosf(time), 1.0, 2.0 * sinf(time)) : the gaze point "the point that the camera is looking at"
        vec3(0, 0, 0) : "it's like the origin of the camera space ya3ni ba2ol le al camera de al reference bta3k"  the position of the camera here it's function of time so it's like a moving camera
        vec3(0, 1, 0) : the up vector "y axis"
        */
        // Projection
        mat4 P = perspective(pi<float>() * 0.5f, width / height, 0.01f, 1000.0f);
        /*
        the prev function determines the projection matrix which is the matrix that determines how the 3d world is projected on the 2d screen
        ya3ni akno b7dd al lense of the camera
        pi<float>()*0.5f : the sight angle
        width/height : the aspect ratio
        0.01f : the near plane: as8r distance momkan a4ofha
        1000.0f : the far plane akbr distance momkan a4ofha
        */
        for (int i = -2; i <= 2; i++)
        {
            mat4 M = translate(mat4(1.0f), vec3(0, 0.0, (float)i));
            mat4 MVP = P * V * M; // lazm al order da 34an a7na 3aizen n3ml al model matrix al awel we b3den al view we b3den al projection
            glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &MVP[0][0]);
            // mvp_loc : is the location of the MVP matrix in the shader
            // 1 : means we will send one matrix
            // GL_FALSE : means we don't want to transpose the matrix
            // &MVP[0][0] : means the address of the first element in the matrix
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *)0); // draw the square using the elements
            // 6: is the number of elements NOT the number of vertices
            // GL_UNSIGNED_SHORT: is the type of the elements 34an a7na 3amlnhom uint16
            //  (void*)0 : is the offset of the elements in the element buffer 34an ybd2 mn awel al buffer
        }

        glfwSwapBuffers(window); // bygib al back buffer mkan al front buffer
        glfwPollEvents();        // to check if there is     any event like keyboard or mouse
    }

    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
