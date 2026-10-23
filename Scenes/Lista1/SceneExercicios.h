#pragma once
#include <array>
#include <random>
#include <unordered_map>
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CPrimitives.h"

class SceneExercicios : public CSceneBase
{
public:
    explicit SceneExercicios(int numero);
    ~SceneExercicios() override;
    void DrawScene(float deltaTime) override;
    void ProcessSceneInput(GLFWwindow* window, float deltaTime) override;

private:
    int exercicio;
    CCamera camera;
    CShader shader;
    CTextures textures;
    CPrimitives cube, sphere, cylinder, cone, plane;
    std::array<std::vector<glm::vec4>, 5> pilhas;
    std::mt19937 gerador;
    float tempo = 0.0f;
    float intervalo = 0.0f;
    bool pausado = false;
    GLuint casaVAO = 0;
    GLuint casaVBO = 0;
    GLsizei totalVertices = 0;
    std::string erro;

    void ResetCamera();
    void PrepararObjeto(const glm::mat4& model, glm::vec4 cor, int textura = -1, float repeticao = 1.0f);
    void DrawPrimitive(CPrimitives& primitive, const glm::mat4& base, glm::vec3 pos, glm::vec3 escala, glm::vec4 cor, int textura = -1, float repeticao = 1.0f);
    void DrawTree(glm::vec3 pos, glm::vec3 escala);
    void DrawSnowman();
    void AtualizarPilhas(float deltaTime);
    void DrawStacks();
    void CriarCasa();
    void CarregarCasa(const char* caminho);
    void EnviarCasa(const std::vector<PrimVertex>& vertices);
    void DrawHouse();
};
