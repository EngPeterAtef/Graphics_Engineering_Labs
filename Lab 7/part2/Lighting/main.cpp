#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "mesh.hpp"
#include "texture.hpp"

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

    GLuint albedo[] = {
        loadImage("assets/images/metal/albedo.jpg"),
        fromColor({0, 0, 0, 255}),
    };
    GLuint specular[] = {
        loadImage("assets/images/metal/specular.jpg"),
        fromColor({128, 128, 128, 255}),
    };
    GLuint roughness[] = {
        loadImage("assets/images/metal/roughness.jpg"),
        loadImage("assets/images/ground/roughness.png"),
    };
    GLuint ao[] = {
        loadImage("assets/images/monkey/ambient_occlusion.jpg"),
        fromColor({255, 255, 255, 255}),
    };
    GLuint emissive[] = {
        loadImage("assets/images/monkey/emissive.jpg"),
        fromColor({0, 0, 0, 255}),
    };

    GLuint sampler;
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLuint program = glCreateProgram();
    GLuint vs = load_shader("assets/shaders/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = load_shader("assets/shaders/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    Mesh obj = loadObj("assets/models/Suzanne.obj");
    Mesh ground = createPlane();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.5, 0.5, 0.0, 1.0);
    glClearDepth(1.0f);

    // GLint vp_loc = glGetUniformLocation(program, "VP");
    // GLint campos_loc = glGetUniformLocation(program, "camera_position");
    // GLint m_loc = glGetUniformLocation(program, "M");
    // GLint m_it_loc = glGetUniformLocation(program, "M_IT");

    auto uloc = [program](const std::string &name) -> GLint
    {
        return glGetUniformLocation(program, name.c_str());
    };

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        float time = (float)glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        // the ambient colors depends on the normal vector of the surface
        glUniform3f(uloc("sky.top"), 0.0f, 0.0f, 1.0f);
        glUniform3f(uloc("sky.horizon"), 1.0f, 0.0f, 0.0f);
        glUniform3f(uloc("sky.bottom"), 0.0f, 1.0f, 0.0f);

        glm::vec3 spot_position = glm::vec3(1.0f, 0.01f, 0.0f);
        glm::vec3 spot_direction = glm::normalize(glm::vec3(-1.0f, -0.1f, 0.0f));

        glUniform1i(uloc("lights[0].type"), 2);
        glUniform3f(uloc("lights[0].position"), spot_position.x, spot_position.y, spot_position.z);
        glUniform3f(uloc("lights[0].direction"), spot_direction.x, spot_direction.y, spot_direction.z);
        glUniform3f(uloc("lights[0].color"), 0.8f, 0.6f, 0.2f);
        glUniform3f(uloc("lights[0].attenuation"), 0.0f, 1.0f, 0.0f);
        glUniform2f(uloc("lights[0].cone_angles"), 0.5f * glm::quarter_pi<float>(), 0.5f * glm::half_pi<float>());

        glm::vec3 point_position = glm::vec3(0.0f, 0.01f, 0.0f);

        glUniform1i(uloc("lights[1].type"), 1);
        glUniform3f(uloc("lights[1].position"), point_position.x, point_position.y, point_position.z);
        glUniform3f(uloc("lights[1].color"), 0.2f, 0.6f, 0.8f);
        glUniform3f(uloc("lights[1].attenuation"), 0.0f, 1.0f, 0.0f);

        glm::vec3 directional_direction = glm::normalize(glm::vec3(2.0f, -1.0f, 2.0f));

        glUniform1i(uloc("lights[2].type"), 0);
        glUniform3f(uloc("lights[2].direction"), directional_direction.x, directional_direction.y, directional_direction.z);
        glUniform3f(uloc("lights[2].color"), 1.0f, 1.0f, 1.0f);

        glUniform1i(uloc("light_count"), 3);

        glm::mat4 P = glm::perspective(glm::pi<float>() * 0.5f, float(width) / height, 0.01f, 1000.0f);
        glm::vec3 camera_position = glm::vec3(2.0f, 1.0f, 2.0f);
        glm::mat4 V = glm::lookAt(
            camera_position,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 VP = P * V;
        glUniformMatrix4fv(uloc("VP"), 1, false, &VP[0][0]);
        glUniform3f(uloc("camera_position"), camera_position.x, camera_position.y, camera_position.z);

        {
            glm::mat4 M = glm::yawPitchRoll(time, 0.0f, 0.0f);
            glUniformMatrix4fv(uloc("M"), 1, false, &M[0][0]);
            glm::mat4 M_I = glm::inverse(M);
            glUniformMatrix4fv(uloc("M_IT"), 1, true, &M_I[0][0]);

            // texture unit zero
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, albedo[0]);
            glBindSampler(0, sampler);
            glUniform1i(uloc("material.albedo"), 0);

            // texture unit one
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specular[0]);
            glBindSampler(1, sampler);
            glUniform1i(uloc("material.specular"), 1); // we send the unit number to the uniform

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, roughness[0]);
            glBindSampler(2, sampler);
            glUniform1i(uloc("material.roughness"), 2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, ao[0]);
            glBindSampler(3, sampler);
            glUniform1i(uloc("material.ambient_occlusion"), 3);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, emissive[0]);
            glBindSampler(4, sampler);
            glUniform1i(uloc("material.emissive"), 4);

            obj.draw();
        }
        {
            glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 3.0f));
            glUniformMatrix4fv(uloc("M"), 1, false, &M[0][0]);
            glm::mat4 M_I = glm::inverse(M);
            glUniformMatrix4fv(uloc("M_IT"), 1, true, &M_I[0][0]);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, albedo[1]);
            glBindSampler(0, sampler);
            glUniform1i(uloc("material.albedo"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specular[1]);
            glBindSampler(1, sampler);
            glUniform1i(uloc("material.specular"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, roughness[1]);
            glBindSampler(2, sampler);
            glUniform1i(uloc("material.roughness"), 2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, ao[1]);
            glBindSampler(3, sampler);
            glUniform1i(uloc("material.ambient_occlusion"), 3);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, emissive[1]);
            glBindSampler(4, sampler);
            glUniform1i(uloc("material.emissive"), 4);

            ground.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ground.destroy();
    obj.destroy();
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
