#include "Application.h"

Application::Application(Window *wnd_ptr)
    :   m_window(wnd_ptr) {
    Init();
}

Application::Application() {
    m_window = new Window();
    Init();
}

Application::~Application() {
    glfwTerminate();
}

void Application::Init() {
    m_cube = new Object("assets/objfiles/cube.obj");
    m_shader = new Shader("assets/shaders/basic.vs", "assets/shaders/basic.fs");
    m_camera = new Camera();
}

void Application::drawTest() {
    m_shader->bind();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    // projection = glm::perspective(glm::radians(45.0f), (float)m_window->getWidth() / (float)m_window->getHeight(), 0.1f, 100.0f);
    // view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    projection = glm::perspective(glm::radians(m_camera->fov), (float)m_window->getWidth() / (float)m_window->getHeight(), 0.1f, 100.0f);
    view       = m_camera->getLookAt();
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 0));
    m_shader->setMat4("model", model);
    m_cube->draw();
    m_shader->unbind();
}

void Application::processCamera(float delta_time) {
    if(m_window->isPressed(GLFW_KEY_W)){
        m_camera->updateInput(FORWARD, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_S)){
        m_camera->updateInput(BACKWARD, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_A)){
        m_camera->updateInput(LEFT, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_D)){
        m_camera->updateInput(RIGHT, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_K)){
        m_camera->updateInput(RPITCH, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_J)){
        m_camera->updateInput(PITCH, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_L)){
        m_camera->updateInput(YAW, delta_time);
    }
    if(m_window->isPressed(GLFW_KEY_H)){
        m_camera->updateInput(RYAW, delta_time);
    }
}

void Application::processInput() {
    if(m_window->isPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(m_window->getWindow(), true);
    }
}

void Application::run() {
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    std::string filePath = "assets\\Scalar\\";
    Model model(filePath, "engine.inf", 80);
    std::cout << "done\n";
    while(!m_window->shouldClose()) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // glViewport(0, 0, m_window->getWidth()/2, m_window->getHeight()/2);
        m_window->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        m_window->clear();

        processCamera(deltaTime);
        processInput();

        glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // drawTest();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_camera->width = m_window->getWidth();
        m_camera->height = m_window->getHeight();
        m_camera->center = model.center;
        m_shader->bind();
        m_shader->setVec3("light_color", {1.f, 1.f, 1.f});
        m_shader->setVec3("model_color", {0.f, 0.5f, 1.f});
        m_shader->setMat4("view_proj_matrix", m_camera->ViewProjectionMatrix());
        m_shader->setVec3("light_src", m_camera->Position);
        m_shader->setVec3("light_pos", m_camera->Position);
        model.draw();

        m_window->swapBuffers();
    }
}