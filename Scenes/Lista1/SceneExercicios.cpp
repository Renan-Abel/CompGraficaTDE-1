#include "SceneExercicios.h"
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>

SceneExercicios::SceneExercicios(int numero)
    : exercicio(numero), gerador(std::random_device{}())
{
    ResetCamera();
    shader.LoadShader("Exercicios", "Scenes/Lista1/Exercicios.vert", "Scenes/Lista1/Exercicios.frag");
    cube.generateCube(1.0f);
    sphere.generateSphere(1.0f);
    cylinder.generateCylinder(1.0f, 1.0f);
    cone.generateCone(1.0f, 1.0f);
    plane.generatePlane(1.0f, 1.0f);
    cube.setupBuffers();
    sphere.setupBuffers();
    cylinder.setupBuffers();
    cone.setupBuffers();
    plane.setupBuffers();
    if (exercicio == 3)
    {
        textures.LoadTextureAnisotropic(0, "Scenes/Lista1/Assets/grama.bmp");
        textures.LoadTextureAnisotropic(1, "Scenes/Lista1/Assets/CRATE.BMP");
        textures.LoadTextureClamp(2, "Scenes/Lista1/Assets/CasaAtlas.jpg");
        CriarCasa();
    }
    if (exercicio == 4)
    {
        textures.LoadTextureClamp(0, "Scenes/Lista1/Assets/CasaTex.jpg");
        try
        {
            CarregarCasa("Scenes/Lista1/Assets/Casa.txt");
        }
        catch (const std::exception& e)
        {
            erro = e.what();
            std::cerr << erro << std::endl;
        }
    }
}

SceneExercicios::~SceneExercicios()
{
    if (casaVBO) glDeleteBuffers(1, &casaVBO);
    if (casaVAO) glDeleteVertexArrays(1, &casaVAO);
}

void SceneExercicios::ResetCamera()
{
    if (exercicio == 1)
        camera = CCamera(glm::vec3(0.0f, 7.0f, 22.0f), glm::vec3(0, 1, 0), -90.0f, -12.0f);
    else if (exercicio == 2)
        camera = CCamera(glm::vec3(13.0f, 10.0f, 20.0f), glm::vec3(0, 1, 0), -123.0f, -15.0f);
    else if (exercicio == 3)
        camera = CCamera(glm::vec3(15.0f, 6.0f, 24.0f), glm::vec3(0, 1, 0), -120.0f, -8.0f);
    else
        camera = CCamera(glm::vec3(25.0f, 17.0f, 32.0f), glm::vec3(0, 1, 0), -128.0f, -16.0f);
}

void SceneExercicios::PrepararObjeto(const glm::mat4& model, glm::vec4 cor, int textura, float repeticao)
{
    shader.SetMat4("model", model);
    shader.SetVec4("Color", cor);
    shader.SetBool("isTextured", textura >= 0);
    shader.SetFloat("repeticao", repeticao);
    if (textura >= 0)
    {
        glActiveTexture(GL_TEXTURE0);
        textures.ApplyTexture(textura);
    }
}

void SceneExercicios::DrawPrimitive(CPrimitives& primitive, const glm::mat4& base, glm::vec3 pos, glm::vec3 escala, glm::vec4 cor, int textura, float repeticao)
{
    glm::mat4 model = glm::scale(glm::translate(base, pos), escala);
    PrepararObjeto(model, cor, textura, repeticao);
    primitive.render();
}

void SceneExercicios::DrawTree(glm::vec3 pos, glm::vec3 escala)
{
    glm::mat4 base = glm::scale(glm::translate(glm::mat4(1.0f), pos), escala);
    DrawPrimitive(cylinder, base, {0, 1, 0}, {0.28f, 2, 0.28f}, {0.65f, 0.28f, 0.06f, 1});
    DrawPrimitive(cone, base, {0, 2.8f, 0}, {1.6f, 3.6f, 1.6f}, {0.02f, 0.45f, 0.04f, 1});
    DrawPrimitive(cone, base, {0, 4.2f, 0}, {1.3f, 3.4f, 1.3f}, {0.04f, 0.62f, 0.03f, 1});
}

void SceneExercicios::DrawSnowman()
{
    glm::mat4 identity(1.0f);
    DrawPrimitive(plane, identity, {0, -0.03f, 0}, {50, 1, 50}, {0.95f, 0.96f, 1, 1});
    DrawTree({-6, 0, 3}, {1.1f, 1.3f, 1.1f});
    DrawTree({-8, 0, -5}, {1.5f, 1.7f, 1.5f});
    DrawTree({-3, 0, -8}, {1.0f, 1.5f, 1.0f});
    DrawTree({4, 0, -8}, {1.4f, 1.8f, 1.4f});
    DrawTree({8, 0, -3}, {1.1f, 1.4f, 1.1f});
    glm::mat4 base = glm::rotate(identity, glm::radians(22.5f * std::sin(tempo * 1.5f)), glm::vec3(0, 0, 1));
    glm::vec4 neve(0.9f, 0.96f, 1.0f, 1.0f);
    DrawPrimitive(sphere, base, {0, 1.25f, 0}, {1.25f, 1.25f, 1.25f}, neve);
    DrawPrimitive(sphere, base, {0, 2.95f, 0}, {0.9f, 0.9f, 0.9f}, neve);
    DrawPrimitive(sphere, base, {0, 4.2f, 0}, {0.65f, 0.65f, 0.65f}, neve);
    DrawPrimitive(cylinder, base, {0, 4.8f, 0}, {0.9f, 0.1f, 0.9f}, {0.08f, 0.1f, 0.8f, 1});
    DrawPrimitive(cylinder, base, {0, 5.12f, 0}, {0.55f, 0.6f, 0.55f}, {0.12f, 0.16f, 1, 1});
    for (float x : {-0.23f, 0.23f})
        DrawPrimitive(sphere, base, {x, 4.35f, 0.59f}, {0.085f, 0.085f, 0.085f}, {0.02f, 0.02f, 0.02f, 1});
    for (float y : {2.7f, 3.2f})
        DrawPrimitive(sphere, base, {0, y, 0.87f}, {0.14f, 0.14f, 0.14f}, {0.9f, 0.05f, 0.03f, 1});
    glm::mat4 nariz = glm::rotate(glm::translate(base, glm::vec3(0, 4.12f, 0.8f)), glm::radians(90.0f), glm::vec3(1, 0, 0));
    DrawPrimitive(cone, nariz, {0, 0, 0}, {0.14f, 0.65f, 0.14f}, {1, 0.4f, 0, 1});
    for (int i = 0; i < 80; ++i)
    {
        float x = float((i * 37) % 47) - 23.0f;
        float z = float((i * 19) % 37) - 18.0f;
        float y = 14.0f - std::fmod(tempo * 1.8f + i * 0.73f, 14.0f);
        DrawPrimitive(sphere, identity, {x, y, z}, {0.045f, 0.045f, 0.045f}, {1, 1, 1, 1});
    }
}

void SceneExercicios::AtualizarPilhas(float deltaTime)
{
    intervalo += deltaTime;
    while (intervalo >= 1.0f)
    {
        intervalo -= 1.0f;
        std::vector<int> disponiveis;
        for (int i = 0; i < 5; ++i)
            if (pilhas[i].size() < 10) disponiveis.push_back(i);
        if (disponiveis.empty())
        {
            for (auto& pilha : pilhas) pilha.clear();
            disponiveis = {0, 1, 2, 3, 4};
        }
        std::uniform_int_distribution<int> escolher(0, static_cast<int>(disponiveis.size()) - 1);
        std::uniform_real_distribution<float> cor(0.15f, 1.0f);
        float r = cor(gerador), g = cor(gerador), b = cor(gerador);
        pilhas[disponiveis[escolher(gerador)]].push_back({r, g, b, 1});
    }
}

void SceneExercicios::DrawStacks()
{
    glm::mat4 base(1.0f);
    DrawPrimitive(plane, base, {0, -0.02f, 0}, {22, 1, 12}, {0.3f, 0.32f, 0.36f, 1});
    for (int i = 0; i < 5; ++i)
        for (size_t j = 0; j < pilhas[i].size(); ++j)
            DrawPrimitive(cube, base, {(i - 2) * 2.5f, 0.5f + static_cast<float>(j), 0}, {1, 1, 1}, pilhas[i][j]);
}

void SceneExercicios::EnviarCasa(const std::vector<PrimVertex>& vertices)
{
    totalVertices = static_cast<GLsizei>(vertices.size());
    glGenVertexArrays(1, &casaVAO);
    glGenBuffers(1, &casaVBO);
    glBindVertexArray(casaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, casaVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PrimVertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), reinterpret_cast<void*>(offsetof(PrimVertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), reinterpret_cast<void*>(offsetof(PrimVertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PrimVertex), reinterpret_cast<void*>(offsetof(PrimVertex, texCoord)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void SceneExercicios::CriarCasa()
{
    std::vector<PrimVertex> vertices;
    auto face = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 inicio, glm::vec2 fim)
    {
        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
        glm::vec3 pontos[] = {a, b, c, a, c, d};
        glm::vec2 uv[] = {inicio, {fim.x, inicio.y}, fim, inicio, fim, {inicio.x, fim.y}};
        for (int i = 0; i < 6; ++i)
            vertices.push_back({{pontos[i].x, pontos[i].y, pontos[i].z}, {uv[i].x, uv[i].y}, {normal.x, normal.y, normal.z}});
    };
    face({-6, 0, 3}, {0, 0, 3}, {0, 4, 3}, {-6, 4, 3}, {0.501f, 0.001f}, {0.999f, 0.499f});
    face({0, 0, 3}, {6, 0, 3}, {6, 4, 3}, {0, 4, 3}, {0.001f, 0.501f}, {0.499f, 0.999f});
    face({6, 0, -3}, {-6, 0, -3}, {-6, 4, -3}, {6, 4, -3}, {0.001f, 0.001f}, {0.499f, 0.499f});
    face({-6, 0, -3}, {-6, 0, 3}, {-6, 4, 3}, {-6, 4, -3}, {0.501f, 0.001f}, {0.999f, 0.499f});
    face({6, 0, 3}, {6, 0, -3}, {6, 4, -3}, {6, 4, 3}, {0.001f, 0.001f}, {0.499f, 0.499f});
    face({-6.5f, 4, 3.5f}, {6.5f, 4, 3.5f}, {6.5f, 6, 0}, {-6.5f, 6, 0}, {0.501f, 0.501f}, {0.999f, 0.999f});
    face({6.5f, 4, -3.5f}, {-6.5f, 4, -3.5f}, {-6.5f, 6, 0}, {6.5f, 6, 0}, {0.501f, 0.501f}, {0.999f, 0.999f});
    face({-6, 4, -3}, {-6, 4, 3}, {-6, 6, 0}, {-6, 6, 0}, {0.001f, 0.001f}, {0.499f, 0.499f});
    face({6, 4, 3}, {6, 4, -3}, {6, 6, 0}, {6, 6, 0}, {0.001f, 0.001f}, {0.499f, 0.499f});
    EnviarCasa(vertices);
}

void SceneExercicios::CarregarCasa(const char* caminho)
{
    std::ifstream arquivo(caminho);
    if (!arquivo) throw std::runtime_error("Nao foi possivel abrir Casa.txt.");
    std::string conteudo((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
    std::replace(conteudo.begin(), conteudo.end(), ',', ' ');
    std::istringstream dados(conteudo);
    std::vector<float> valores;
    float valor;
    while (dados >> valor) valores.push_back(valor);
    if (!dados.eof() || valores.empty() || valores.size() % 15 != 0)
        throw std::runtime_error("Casa.txt deve conter triangulos com posicao XYZ e coordenadas UV.");
    std::vector<PrimVertex> vertices;
    for (size_t i = 0; i < valores.size(); i += 15)
    {
        glm::vec3 a(valores[i], valores[i + 1], valores[i + 2]);
        glm::vec3 b(valores[i + 5], valores[i + 6], valores[i + 7]);
        glm::vec3 c(valores[i + 10], valores[i + 11], valores[i + 12]);
        glm::vec3 normal = glm::cross(b - a, c - a);
        float comprimento = glm::length(normal);
        if (comprimento <= 0.000001f) throw std::runtime_error("Casa.txt contem um triangulo degenerado.");
        normal /= comprimento;
        for (size_t j = i; j < i + 15; j += 5)
            vertices.push_back({{valores[j], valores[j + 1], valores[j + 2]}, {valores[j + 3], valores[j + 4]}, {normal.x, normal.y, normal.z}});
    }
    EnviarCasa(vertices);
}

void SceneExercicios::DrawHouse()
{
    glm::mat4 base(1.0f);
    if (exercicio == 3)
    {
        DrawPrimitive(plane, base, {0, -0.02f, 0}, {100, 1, 100}, {1, 1, 1, 1}, 0, 35.0f);
        DrawPrimitive(cube, base, {10, 1, 2}, {2, 2, 2}, {1, 1, 1, 1}, 1);
        DrawPrimitive(cube, base, {10, 3, 2}, {2, 2, 2}, {1, 1, 1, 1}, 1);
        DrawPrimitive(cube, base, {10, 5, 2}, {2, 2, 2}, {1, 1, 1, 1}, 1);
        DrawPrimitive(cube, base, {8, 1, 2}, {2, 2, 2}, {1, 1, 1, 1}, 1);
        DrawPrimitive(cube, base, {8, 3, 2}, {2, 2, 2}, {1, 1, 1, 1}, 1);
    }
    else
        DrawPrimitive(plane, base, {0, -0.03f, 0}, {70, 1, 70}, {0.28f, 0.36f, 0.22f, 1});
    if (totalVertices == 0) return;
    PrepararObjeto(base, {1, 1, 1, 1}, exercicio == 3 ? 2 : 0);
    glBindVertexArray(casaVAO);
    glDrawArrays(GL_TRIANGLES, 0, totalVertices);
    glBindVertexArray(0);
}

void SceneExercicios::DrawScene(float deltaTime)
{
    if (!pausado)
    {
        tempo += deltaTime;
        if (exercicio == 2) AtualizarPilhas(deltaTime);
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.015f, 0.02f, 0.035f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    float aspecto = height > 0 ? static_cast<float>(width) / height : ASPECT_RATIO;
    shader.Use("Exercicios");
    shader.SetMat4("projection", glm::perspective(glm::radians(camera.Zoom), aspecto, Z_NEAR, Z_FAR));
    shader.SetMat4("view", camera.GetViewMatrix());
    shader.SetInt("Texture0", 0);
    if (exercicio == 1) DrawSnowman();
    else if (exercicio == 2) DrawStacks();
    else DrawHouse();
}

void SceneExercicios::ProcessSceneInput(GLFWwindow* window, float deltaTime)
{
    InputManager& input = InputManager::GetInstance();
    if (input.IsKeyJustPressed(GLFW_KEY_TAB))
        input.SetCursorMode(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    if (input.IsKeyJustPressed(GLFW_KEY_SPACE)) pausado = !pausado;
    if (input.IsKeyJustPressed(GLFW_KEY_C)) ResetCamera();
    if (input.IsKeyJustPressed(GLFW_KEY_R))
    {
        tempo = 0.0f;
        intervalo = 0.0f;
        for (auto& pilha : pilhas) pilha.clear();
    }
    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) return;
    camera.MovementSpeed = input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ? SPEED_MULTIPLIER * 2.0f : SPEED * 2.0f;
    if (input.IsKeyPressed(GLFW_KEY_W)) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (input.IsKeyPressed(GLFW_KEY_S)) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (input.IsKeyPressed(GLFW_KEY_A)) camera.ProcessKeyboard(LEFT, deltaTime);
    if (input.IsKeyPressed(GLFW_KEY_D)) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (input.IsKeyPressed(GLFW_KEY_E)) camera.ProcessKeyboard(UP, deltaTime);
    if (input.IsKeyPressed(GLFW_KEY_Q)) camera.ProcessKeyboard(DOWN, deltaTime);
    glm::vec2 movimento = input.GetMouseDelta();
    camera.ProcessMouseMovement(movimento.x, movimento.y);
    camera.ProcessMouseScroll(input.GetScrollOffset());
}
