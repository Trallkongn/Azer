#include "azpch.h"
#include "ObjectCreationLayer.h"
#include <imgui.h>
#include <Azer/Renderer3D/Renderer3D.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Mesh/VAOLibrary.h>

Azer::ObjectCreationLayer::ObjectCreationLayer()
	: Layer("Object Creation Layer"),
    m_CameraController(200,45.0f,1920.0f/1080.0f,0.1f,1000.0f,true)
{
    VAO = VAOLibrary::CreateCubeVAO();
}

Azer::ObjectCreationLayer::~ObjectCreationLayer()
{
}

void Azer::ObjectCreationLayer::OnAttach()
{
	GameObject defaultObj;
	defaultObj.name = "Default Cube";
	m_GameObjects.push_back(defaultObj);
}

void Azer::ObjectCreationLayer::OnDetach()
{

}

void Azer::ObjectCreationLayer::OnUpdate(TimeStep delta)
{
    m_CameraController.OnUpdate(delta);
    OnRender();
}

void Azer::ObjectCreationLayer::OnRender()
{
	RenderObjects();
}

void Azer::ObjectCreationLayer::OnImGuiRender()
{
    ImGui::Begin("Object Creator");
    
    if (ImGui::Button("Create Object"))
        m_ShowCreationWindow = true;

    if (m_ShowCreationWindow) ShowObjectCreationWindow();

    ShowObjectEditor();

    ImGui::End();
}

void Azer::ObjectCreationLayer::OnEvent(Event& event)
{
    ImGuiIO& io = ImGui::GetIO();

    // 如果ImGui想要捕获事件，先让ImGui处理
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        event.Handled = true;
        return;
    }

    m_CameraController.OnEvent(event);
}

void Azer::ObjectCreationLayer::ShowObjectCreationWindow()
{
    static char objName[128] = "New Object";
    static float pos[3] = { 0.0f, 0.0f, 0.0f };
    static float scale[3] = { 1.0f, 1.0f, 1.0f };
    static float color[3] = { 1.0f, 1.0f, 1.0f };

    ImGui::Begin("Create New Object", &m_ShowCreationWindow);

    ImGui::InputText("Name", objName, IM_ARRAYSIZE(objName));
    ImGui::InputFloat3("Position", pos);
    ImGui::InputFloat3("Scale", scale);
    ImGui::ColorEdit3("Color", color);

    if (ImGui::Button("Create")) {
        GameObject newObj;
        newObj.name = objName;
        newObj.position = glm::vec3(pos[0], pos[1], pos[2]);
        newObj.scale = glm::vec3(scale[0], scale[1], scale[2]);
        newObj.color = glm::vec3(color[0], color[1], color[2]);

        m_GameObjects.push_back(newObj);
    }
    ImGui::End();
}

void Azer::ObjectCreationLayer::ShowObjectEditor()
{
    ImGui::Begin("Object Editor");

    static int selected = -1;

    // 显示物体列表 
    ImGui::BeginChild("Object List", ImVec2(150, 0), true);
    for (int i = 0; i < m_GameObjects.size(); i++) {
        if (ImGui::Selectable(m_GameObjects[i].name.c_str(), selected == i)) {
            selected = i;
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // 显示选中物体的属性编辑器
    ImGui::BeginGroup();
    if (selected != -1 && selected < m_GameObjects.size()) {
        auto& obj = m_GameObjects[selected];

        char nameBuf[128];
        strcpy(nameBuf, obj.name.c_str());
        if (ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf))) {
            obj.name = nameBuf;
        }

        float pos[3] = { obj.position.x,  obj.position.y,  obj.position.z };
        if (ImGui::InputFloat3("Position", pos)) {
            obj.position = glm::vec3(pos[0], pos[1], pos[2]);
        }

        float scale[3] = { obj.scale.x,  obj.scale.y,  obj.scale.z };
        if (ImGui::InputFloat3("Scale", scale)) {
            obj.scale = glm::vec3(scale[0], scale[1], scale[2]);
        }

        float color[3] = { obj.color.r,  obj.color.g,  obj.color.b };
        if (ImGui::ColorEdit3("Color", color)) {
            obj.color = glm::vec3(color[0], color[1], color[2]);
        }

        if (ImGui::Button("Delete")) {
            m_GameObjects.erase(m_GameObjects.begin() + selected);
            selected = -1;
        }
    }
    ImGui::EndGroup();

    ImGui::End();
}

void Azer::ObjectCreationLayer::RenderObjects()
{
    Renderer3D::BeginScene(m_CameraController.GetCamera());

    for (auto& obj : m_GameObjects)
    {
        Renderer3D::DrawCube(obj.position, obj.scale, glm::vec4(obj.color, 1.0f), glm::vec3(0.0f));
    }

    Renderer3D::EndScene();
}
