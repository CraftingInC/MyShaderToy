/*
-lglfw3
-lgdi32
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define LOGGER_IMPLEMENTATION
#include "logging.h"

#include <stdlib.h>     // malloc()   free()

unsigned int LoadShader(const char* fragmentPath);
void useShader(unsigned int programID);
int  loadShaderFromFile(const char* fileName, int shaderType);
void checkCompileErrors(unsigned int shader);
void ShaderCleanUp(unsigned int programID);
void setFloat(unsigned int programID, const char* name, float value);
void setInt(unsigned int programID, const char* name, int value);
void quitProgram();

unsigned int VAO;
unsigned int programID;
int isPressed = 0;
GLFWwindow *window;
int uresolution;
int utime;
int umouse;
float WIDTH = 800.0;
float HEIGHT = 600.0;
double timer;
float mouseX, mouseY, isLeftMouseDown, isMiddleMouseDown, isRightMouseDown, wheelState;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    mouseX = (float)(xposIn);
    mouseY = (float)(yposIn);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        isLeftMouseDown = 1;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        isLeftMouseDown = 0;
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        isMiddleMouseDown = 1;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        isMiddleMouseDown = 0;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        isRightMouseDown = 1;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        isRightMouseDown = 0;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset < 0)
    {
        wheelState = -1.0;
    } else if(yoffset > 0)
    {
        wheelState = 1.0;
    }
}

void installShaders()
{
    if(VAO) // If the Shader was created already, we delete it before recreating it.
    {
        glDeleteVertexArrays(1, &VAO);
        ShaderCleanUp(programID);
    }

    programID = LoadShader("shader.txt");
    if(programID == 0)
    {
        quitProgram();
    } else {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        uresolution = glGetUniformLocation(programID, "iResolution");
        utime = glGetUniformLocation(programID, "iTime");
        umouse = glGetUniformLocation(programID, "iMouse");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}

void processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if(isPressed == 0)
        {
            installShaders();
            isPressed = 1;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && isPressed == 1)
    {
        isPressed = 0;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    WIDTH = w;
    HEIGHT = h;
    glViewport(0, 0, WIDTH, HEIGHT);
    useShader(programID);
    glUniform2f(uresolution, WIDTH, HEIGHT);
    timer = glfwGetTime();
    glUniform1f(utime, timer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void quitProgram()
{
    if(window)
    {
        glfwDestroyWindow(window);
        log("INFO : Window Closed");
    }
    glfwTerminate();
    log("INFO : Quit Program");
    quitLogging();
}

int main()
{
    initLogging();
    log("INFO : Initializing GLFW3");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "My Shader Toy", NULL, NULL);
    if (window == NULL)
    {
        log("ERROR : Failed to create GLFW window");
        quitProgram();
        return -1;
    } else {
        log("INFO : GLFW3 Window created");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log("ERROR : Failed to initialize GLAD");
        quitProgram();
        return -1;
    } else {
        log("INFO : OpenGL ( GLAD ) context created");
    }

    log("INFO : OpenGL Version  : %s", glGetString(GL_VERSION));
    log("INFO : OpenGL Renderer : %s", glGetString(GL_RENDERER));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    installShaders();

    timer = glfwGetTime();

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        processInput();

        useShader(programID);
        glUniform2f(uresolution, WIDTH, HEIGHT);
        timer = glfwGetTime();
        glUniform1f(utime, timer);

        // uniform vec4 iMouse;
        // mouse pixel coords. xy: current (if MLB down), zw: click
        glUniform4f(umouse, mouseX, mouseY, isLeftMouseDown, isMiddleMouseDown);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    ShaderCleanUp(programID);

    quitProgram();
    return 0;
}

// -------Shader Stuff--------
unsigned int LoadShader(const char* fragmentPath)
{
    // This is needed to cover the whole screen
    const char *vertexShaderSource = "#version 450 core\n"
    "const vec2 quadVertices[4] = {\n"
    " vec2(-1.0, -1.0),\n"
    " vec2(1.0, -1.0),\n"
    " vec2(-1.0, 1.0),\n"
    " vec2(1.0, 1.0) };\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);\n"
    "}\n";

    unsigned int programID;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        log("ERROR : SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        return 0;
    } else {
        log("INFO : VERTEX Shader created");
        unsigned int fragment = loadShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
        log("INFO : FRAGMENT Shader created");

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragment);
        glLinkProgram(programID);
        checkCompileErrors(programID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragment);
        return programID;
    }
}

void useShader(unsigned int programID)
{
    glUseProgram(programID);
}

void setInt(unsigned int programID, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(programID, name), value);
}

void setFloat(unsigned int programID, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(programID, name), value);
}

int loadShaderFromFile(const char* fileName, int shaderType)
{
	FILE* shaderSource = fopen(fileName, "rb");
	if (shaderSource != NULL)
    {
        char* shader;
        fseek(shaderSource, 0, SEEK_END);
        size_t TOTAL_SIZE = ftell(shaderSource);
        rewind(shaderSource);

        shader = (char*)malloc(TOTAL_SIZE + 1);
        fread(shader, 1, TOTAL_SIZE, shaderSource);
        shader[TOTAL_SIZE] = 0;
        fclose(shaderSource);

        GLuint shaderID = 0;
        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, (const char**)&shader, NULL);
        glCompileShader(shaderID);

        GLint fragment_compiled;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &fragment_compiled);
        if (fragment_compiled != GL_TRUE)
        {
            GLsizei logLength;
            GLchar  log[1024];
            glGetShaderInfoLog(shaderID, sizeof(log), &logLength, log);
            log("ERROR: %s -->   %s", fileName, log);

        } else {
            free(shader);
            return shaderID;
        }
	} else {
		log("ERROR : Unable to open shader file : %s", fileName);
	}
    return -1;
}

void checkCompileErrors(unsigned int shader)
{
    int success;
    char infoLog[1024];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        log("ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM\n%s\n"
               " -- --------------------------------------------------- -- ", infoLog);
    }
}

void ShaderCleanUp(unsigned int programID)
{
    glDeleteProgram(programID);
}
