#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>    // GLEW: The OpenGL Extension Wrangler Library.
#include <SDL2/SDL.h>   // Simple DirectMedia Layer.
#include "cglm/cglm.h"  // OpenGL Mathematics (glm) for C.

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

    // Definição dos vértices do triângulo.
    GLfloat vertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // Definição das cores dos vértices do triângulo.
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    // Criação e configuração do Vertex Array Object (VAO).
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Criação e configuração do Vertex Buffer Object (VBO).
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configuração do atributo de posição.
    // "layout(location = 0) in vec3 a_Pos;\n"
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Criação e configuração do buffer de cores.
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Configuração do atributo de cor.
    // "layout(location = 1) in vec3 a_Color;\n"
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    // Configuração da viewport do OpenGL.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Exibe apenas a parte frontal dos polígonos.
    glEnable(GL_CULL_FACE);


    // Variáveis para definir translação, rotação e escala.
    float tx = 0.0f, ty = 0.0f, tz = 0.0f;
    float rx = 0.0f, ry = 0.0f, rz = 0.0f;
    float sx = 1.0f, sy = 1.0f, sz = 1.0f;

    // Criação das matrizes de modelo, visão, projeção.
    mat4 modelMatrix, viewMatrix, projectionMatrix;

    // Configuração da matriz do modelo.
    // Matriz identidade e sem nenhuma transformação, já que
    // tx=ty=tz=0.0f, rx=ry=rz=0.0f e sx=sy=sz=1.0f.
    glm_mat4_identity(modelMatrix);
    glm_translate(modelMatrix, (vec3){ tx, ty, tz });
    glm_rotate(modelMatrix, glm_rad(rx), (vec3){ 1.0f, 0.0f, 0.0f });
    glm_rotate(modelMatrix, glm_rad(ry), (vec3){ 0.0f, 1.0f, 0.0f });
    glm_rotate(modelMatrix, glm_rad(rz), (vec3){ 0.0f, 0.0f, 1.0f });
    glm_scale(modelMatrix, (vec3){ sx, sy, sz });

    // Configuração da câmera (matriz de visão).
    // Câmera localizada na posição (0.0f, 2.0f, 5.0f), apontando
    // para a origem da cena (0.0f, 0.0f, 0.0f) e "de pé", já que
    // o vetor up está com valor (0.0f, 1.0f, 0.0f).
    vec3 cameraPos = {0.0f, 2.0f, 5.0f};
    vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPos, cameraTarget, cameraUp, viewMatrix);

    bool isPerspective = false;
    if (isPerspective)
    {
        // Criação da matriz de projeção perspectiva.
        float fov = glm_rad(60.0f);
        float aspect = WINDOW_WIDTH * 1.0f / WINDOW_HEIGHT * 1.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        glm_perspective(fov, aspect, nearPlane, farPlane, projectionMatrix);
    }
    else
    {
        // Criação da matriz de projeção ortogonal.
        float left = -1.0f;
        float right = 1.0f;
        float bottom = -1.0f;
        float top = 1.0f;
        float near = -10.0f;
        float far = 10.0f;
        glm_ortho(left, right, bottom, top, near, far, projectionMatrix);
    }

    // Multiplicação das matrizes de modelo, visão e projeção para obter
    // a matriz MVP que será enviada para o vertex shader.
    // Observe a ordem de multiplicação: MVP = M(V(P)), isto é:
    // P * V primeiro, depois M * VP.
    mat4 MVPMatrix;
    glm_mat4_mul(projectionMatrix, viewMatrix, MVPMatrix);
    glm_mat4_mul(MVPMatrix, modelMatrix, MVPMatrix);

    // Envio da matriz MVP para o vertex shader.
    glUniformMatrix4fv(u_MVPMatrix, 1, GL_FALSE, (const GLfloat *)MVPMatrix);

    SDL_Event event;
    bool isRunning = true;
    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        // Limpa o buffer (tela) com a cor preta.
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Renderização do triângulo.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Atualização da janela.
        SDL_GL_SwapWindow(window);
    }

    // Liberação de recursos (shaders).
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Liberação de recursos (contexto OpenGL e janela SDL).
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}
