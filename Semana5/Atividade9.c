#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "cglm/cglm.h"

// Código do vertex shader.
const char *VERTEX_SHADER_CODE =
    "#version 330 core\n"
    "layout(location = 0) in vec3 a_Pos;\n"
    "layout(location = 1) in vec3 a_Color;\n"
    "out vec3 v_FragColor;\n"
    "uniform mat4 u_MVPMatrix;\n"
    "void main() {\n"
    "   gl_Position = u_MVPMatrix * vec4(a_Pos, 1.0);\n"
    "   v_FragColor = a_Color;\n"
    "}\0";

// Código do fragment shader.
const char *FRAG_SHADER_CODE =
    "#version 330 core\n"
    "in vec3 v_FragColor;\n"
    "out vec4 f_Color;\n"
    "void main() {\n"
    "   f_Color = vec4(v_FragColor, 1.0);\n"
    "}\0";

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[])
{
    // Inicialização do SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erro ao iniciar o SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Criação da janela usando SDL (suporte para OpenGL).
    SDL_Window *window = SDL_CreateWindow("Hello, OpenGL",
                            0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "Erro ao criar a janela: %s\n", SDL_GetError());
        return -1;
    }

    // Configuração do contexto OpenGL.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Criação do contexto OpenGL.
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fprintf(stderr, "Erro ao criar o contexto OpenGL: %s\n", SDL_GetError());
        return -1;
    }

    // Inicialização do GLEW.
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Erro ao iniciar o GLEW: %s\n", glewGetErrorString(err));
        return -1;
    }

    // Compilação do vertex shader.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VERTEX_SHADER_CODE, NULL);
    glCompileShader(vertexShader);

    // Compilação do fragment shader.
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FRAG_SHADER_CODE, NULL);
    glCompileShader(fragmentShader);

    // Criação do shader program, atribuindo o vertex shader e fragment shader.
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Mapeamento da variável uniform mat4 u_MVPMatrix do vertex shader para um identificador neste código.
    GLint u_MVPMatrix = glGetUniformLocation(shaderProgram, "u_MVPMatrix");
    if (u_MVPMatrix == -1)
    {
        fprintf(stderr, "Erro ao obter a localização da variável uniform 'u_MVPMatrix' do vertex shader.\n");
        return -1;
    }

    // Definição dos vértices do cubo.
    GLfloat vertices[] = {
        // Frente
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Atrás
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // Índices dos vértices para formar os triângulos do cubo.
    GLuint indices[] = {
        // Frente
        0, 1, 2, 2, 3, 0,
        // Direita
        1, 5, 6, 6, 2, 1,
        // Atrás
        7, 6, 5, 5, 4, 7,
        // Esquerda
        4, 0, 3, 3, 7, 4,
        // Topo
        3, 2, 6, 6, 7, 3,
        // Fundo
        4, 5, 1, 1, 0, 4
    };

    // Definição das cores dos vértices do cubo.
    GLfloat colors[] = {
        // Frente
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        // Atrás
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    // Criação e configuração do Vertex Array Object (VAO).
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Criação e configuração do Vertex Buffer Object (VBO).
    GLuint VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configuração do atributo de posição.
    // "layout(location = 0) in vec3 a_Pos;\n"
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Criação e configuração do buffer de cores.
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Configuração do atributo de cor.
    // "layout(location = 1) in vec3 a_Color;\n"
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    // Criação e configuração do Element Buffer Object (EBO).
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configuração da viewport do OpenGL.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Exibe apenas a parte frontal dos polígonos.
    glEnable(GL_CULL_FACE);

    // Variáveis para definir a rotação.
    float angle = 0.0f;

    // Criação das matrizes de modelo, visão, projeção.
    mat4 modelMatrix, viewMatrix, projectionMatrix;

    // Configuração da câmera (matriz de visão).
    vec3 cameraPos = {0.0f, 2.0f, 5.0f};
    vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPos, cameraTarget, cameraUp, viewMatrix);

    // Criação da matriz de projeção perspectiva.
    float fov = glm_rad(
