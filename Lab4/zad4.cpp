#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

const char* vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;
    layout(location = 2) in float aPointSize;

    uniform mat4 MVP;

    out vec3 color;

    void main() {
        gl_Position = MVP * vec4(aPos, 1.0);
        color = aColor;
        gl_PointSize = aPointSize;
    }
)";

const char* fragmentSrc = R"(
    #version 330 core
    in vec3 color;
    out vec4 fragColor;

    void main() {
        fragColor = vec4(color, 1.0);
    }
)";

struct Vertex {
    float x, y, z;
    float r, g, b;
    float ps;
};

int objectNumber = 1;
float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
        if (key == GLFW_KEY_1) objectNumber = 1;
        if (key == GLFW_KEY_2) objectNumber = 2;
        
        float rotSpeed = 5.0f;
        if (key == GLFW_KEY_UP)    rotX -= rotSpeed;
        if (key == GLFW_KEY_DOWN)  rotX += rotSpeed;
        if (key == GLFW_KEY_LEFT)  rotY -= rotSpeed;
        if (key == GLFW_KEY_RIGHT) rotY += rotSpeed;
        if (key == GLFW_KEY_PAGE_UP)   rotZ -= rotSpeed;
        if (key == GLFW_KEY_PAGE_DOWN) rotZ += rotSpeed;
        
        if (key == GLFW_KEY_HOME) { rotX = 0; rotY = 0; rotZ = 0; }
    }
}

std::vector<Vertex> buildCorkscrew(int turns, int pointsPerTurn) {
    std::vector<Vertex> vertices;
    int totalPoints = turns * pointsPerTurn;
    float radius = 0.5f;
    float lengthStep = 4.0f / totalPoints;
    
    for (int i = 0; i < totalPoints; ++i) {
        float angle = (float)i / pointsPerTurn * 2.0f * (float)M_PI;
        
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        float z = -2.0f + i * lengthStep;
        
        float pointSize = 2.0f + (15.0f * (float)i / totalPoints);
        
        float r = 0.6f + 0.1f * (float)i / totalPoints; 
        float g = 0.3f;
        float b = 0.1f;
        
        vertices.push_back({x, y, z, r, g, b, pointSize});
    }
    return vertices;
}

std::vector<Vertex> buildPyramidBase(int N, float radius) {
    std::vector<Vertex> vertices;
    vertices.push_back({0.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f});
    
    for (int i = 0; i <= N; ++i) {
        float angle = (float)i / N * 2.0f * (float)M_PI;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        vertices.push_back({x, -0.5f, z, 0.4f, 0.4f, 0.4f, 1.0f});
    }
    return vertices;
}

std::vector<Vertex> buildPyramidSide(int N, float radius, float height) {
    std::vector<Vertex> vertices;
    float angle = 2.0f * (float)M_PI / N;
    
    vertices.push_back({0.0f, height - 0.5f, 0.0f, 1.0f, 0.2f, 0.2f, 1.0f});
    vertices.push_back({radius, -0.5f, 0.0f, 0.2f, 0.5f, 1.0f, 1.0f});
    vertices.push_back({radius * cosf(angle), -0.5f, -radius * sinf(angle), 1.0f, 0.8f, 0.2f, 1.0f});
    
    return vertices;
}

GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

GLuint setupVAO(const std::vector<Vertex>& vertices) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    return VAO;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Wariant 12 - Korkociag i Piramida", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glUseProgram(program);
    GLint uMVP = glGetUniformLocation(program, "MVP");

    int N = 12;
    auto corkscrewData = buildCorkscrew(N, 50); 
    auto pyramidBaseData = buildPyramidBase(N, 0.6f);
    auto pyramidSideData = buildPyramidSide(N, 0.6f, 1.2f);
    
    GLuint vaoCorkscrew = setupVAO(corkscrewData);
    GLuint vaoPyrBase   = setupVAO(pyramidBaseData);
    GLuint vaoPyrSide   = setupVAO(pyramidSideData);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(rotX), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotY), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0, 0, 1));

        std::stack<glm::mat4> ms;
        ms.push(proj * view * model);

        if (objectNumber == 1) {
            glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ms.top()));
            glBindVertexArray(vaoCorkscrew);
            glDrawArrays(GL_POINTS, 0, corkscrewData.size());
            
        } else if (objectNumber == 2) {
            glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ms.top()));
            glBindVertexArray(vaoPyrBase);
            glDrawArrays(GL_TRIANGLE_FAN, 0, pyramidBaseData.size());
            
            glBindVertexArray(vaoPyrSide);
            for (int i = 0; i < N; ++i) {
                ms.push(ms.top());
                
                ms.top() = glm::rotate(ms.top(), glm::radians((float)i * (360.0f / N)), glm::vec3(0, 1, 0));
                
                glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(ms.top()));
                glDrawArrays(GL_TRIANGLES, 0, 3);
                
                ms.pop();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}