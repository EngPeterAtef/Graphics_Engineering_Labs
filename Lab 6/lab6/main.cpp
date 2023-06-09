#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

struct Color
{
    uint8_t r, g, b, a;
};

struct Vertex
{
    glm::vec3 position;
    Color color;
    glm::vec2 tex_coord;
};

GLuint load_shader(const std::string &path, GLenum shader_type)
{
    std::ifstream file(path);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCodeCStr = sourceCode.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
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

int main()
{

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create Window\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
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
        {{-0.5, -0.5, 0.0}, {255, 0, 0, 255}, {0.0, 0.0}},
        {{0.5, -0.5, 0.0}, {0, 255, 0, 255}, {1.0, 0.0}},
        {{0.5, 0.5, 0.0}, {0, 0, 255, 255}, {1.0, 1.0}},
        {{-0.5, 0.5, 0.0}, {255, 255, 0, 255}, {0.0, 1.0}},
    };

    uint16_t elements[6] = {0, 1, 2, 2, 3, 0};

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint16_t), elements, GL_STATIC_DRAW);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    GLint position_loc = 0; // glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, false, sizeof(Vertex), 0);

    GLint color_loc = 1; // glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));

    GLint tex_coord_loc = 2; // glGetAttribLocation(program, "tex_coord");
    glEnableVertexAttribArray(tex_coord_loc);
    glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

    glBindVertexArray(0);

    GLint mvp_loc = glGetUniformLocation(program, "MVP");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.5, 0.5, 0.0, 1.0);
    glClearDepth(1.0f);

    // glViewport(0, 0, 320, 240);

    // glEnable(GL_SCISSOR_TEST);
    // glScissor(0, 0, 320, 240);

    Color B = {0, 0, 0, 255};
    Color W = {255, 255, 255, 0};
    Color Y = {255, 255, 0, 255};

    Color image[] = {
        W,
        W,
        Y,
        Y,
        Y,
        Y,
        W,
        W,
        W,
        Y,
        Y,
        B,
        B,
        Y,
        Y,
        W,
        Y,
        Y,
        B,
        Y,
        Y,
        B,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        B,
        Y,
        Y,
        B,
        Y,
        Y,
        Y,
        Y,
        B,
        Y,
        Y,
        B,
        Y,
        Y,
        W,
        Y,
        Y,
        Y,
        Y,
        Y,
        Y,
        W,
        W,
        W,
        Y,
        Y,
        Y,
        Y,
        W,
        W,
    };

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint sampler;
    glGenSamplers(1, &sampler);

    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /*
    Frame buffer
    it doesn't contain data but it defines the textures that will be used
    ya3ni badl ma barsm al texture bta3i 3la al screen barsmo 3la al frame buffer
    we arsmo kol ma ab2a 3aizo mn hnak as the texture itself is a pixels ya3ni ha3tbr al texture bta3i 
    akn al frame buffer da window so8yra gwa al window bta3ti
    */
    GLuint frame_buffer;
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
    // DRAW_FRAMEBUFFER: draw to this framebuffer

    GLuint render_target;
    glGenTextures(1, &render_target);
    glBindTexture(GL_TEXTURE_2D, render_target);

    // width and height of rander target
    int RTW = 256;
    int RTH = 256;

    GLsizei levels = (GLsizei)glm::floor(glm::log2((float)glm::max(RTW, RTH))) + 1;
    /*
    max(RTW, RTH): max width or height of the texture 34an a4of na m7tag as8ro ad eh 34an awsl lel 1:1 mipmap
    log2: 34an fe kol mipmap ba2sm al size 3la 2 fa 3aiz a3rf na ha2dr 2a2smo kam mara we da haykon number of mipmap levels
    floor: to convert it to integer
    +1 : 34an al level al asli level zero
    a7na bn7sb da 34an lo hanst5dm mipmaps fa 3aiz a3rf na ha2dr a3ml kam level 34an a6gzlha storage
    */

    // al function de bt7gz storgae bs m4 bt7ot gwaha 7aga
    glTexStorage2D(GL_TEXTURE_2D,levels, GL_RGBA8, RTW, RTH);
    // attach the texture to the framebuffer
    // 1: one level

    GLuint depth_buffer;
    glGenTextures(1, &depth_buffer);
    glBindTexture(GL_TEXTURE_2D, depth_buffer);
    //lazm ykon al size bta3 al storage bta3t al depth buffer = size bta3 al storage bta3t al color buffer
    //both RTW and RTH
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, RTW, RTH);//number of levels: 1 34an a7na m4 bnrsm al levels now
    //GL_DEPTH_COMPONENT32: 32 bit depth buffer "max resolution"


    // attach the texture to the framebuffer
    // GL_DRAW_FRAMEBUFFER: draw to this framebuffer
    // GL_COLOR_ATTACHMENT0: attach to the first color attachment ya3ni momkan asrsm 3la aktr mn texture fe nfs al frame buffer
    // rander_target: the texture to attach
    // 0: the mipmap level al level ali na harsm 3lah
    // GL_TEXTURE_2D: the type of texture
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_target, 0);
    
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0);
    /*
    now we have two textures attached to the framebuffer
    one for color and one for depth
    */
    GLint tex_loc = glGetUniformLocation(program, "tex");

    // glEnable(GL_BLEND);
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        float time = (float)glfwGetTime();

        glUseProgram(program);
        glBindVertexArray(vertex_array);

        /*
        ali by7sl hna ani bsrm al sora 3la al framebuffer we b3den al sora de barsmha 3la al window
        */

        {//framebuffer
            //bind the framebuffer 34an harsm 3lah
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);

            glViewport(0, 0, RTW, RTH);
            glClearColor(1.0, 0.1, 0.1, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);//al texture da hoa al smile face
            glBindSampler(0, sampler);
            glUniform1i(tex_loc, 0);

            glm::mat4 P = glm::perspective(glm::pi<float>() * 0.5f, float(RTW) / RTH, 0.01f, 1000.0f);
            glm::mat4 V = glm::lookAt(
                glm::vec3(2.0f * cosf(time), 1.0f, 2.0f * sinf(time)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            for (int i = 0; i < 8; i++)
            {
                float theta = glm::two_pi<float>() * (float(i) / 8);
                for (int z = 1; z <= 2; z++)
                {
                    glm::mat4 M = glm::yawPitchRoll(theta, 0.0f, 0.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)z));

                    glm::mat4 MVP = P * V * M;

                    glUniformMatrix4fv(mvp_loc, 1, false, &MVP[0][0]);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
                }
            }
        }

        {//window
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            //unbinding the framebuffer 34an na harsm 3la l 4a4a 3adi delw2ty

            glViewport(0, 0, width, height);
            glClearColor(0, 0.1, 0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, render_target);//al texture da hoa al 16 smile face bel red background

            //a7na bnrsm 3la level 0 so we need to generate the other mipmaps to  use them if needed
            glGenerateMipmap(GL_TEXTURE_2D);//34an lo al sora bt2rb we tb3t y3mlha aktr mn mipmap levels

            glBindSampler(0, sampler);
            glUniform1i(tex_loc, 0);

            glm::mat4 P = glm::perspective(glm::pi<float>() * 0.5f, float(width) / height, 0.01f, 1000.0f);
            glm::mat4 V = glm::lookAt(
                glm::vec3(2.0f , 1.0f, 2.0f ),
                // glm::vec3(2.0f * cosf(time), 1.0f, 2.0f * sinf(time)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            
            //de ya3ni harsm al framebuffer 3la al window 8*2 = 16 mara
            for (int i = 0; i < 8; i++)
            {
                float theta = glm::two_pi<float>() * (float(i) / 8);//angle
                for (int z = 1; z <= 2; z++)//position
                {
                    glm::mat4 M = glm::yawPitchRoll(theta, 0.0f, 0.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)z));

                    glm::mat4 MVP = P * V * M;

                    glUniformMatrix4fv(mvp_loc, 1, false, &MVP[0][0]);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
                }
            }
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
