#include "EditLayer.h"
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"

namespace Azer {
    EditLayer::EditLayer(const std::string& name)
        : Layer(name), m_CameraController(50, 45.0f, 1920.0 / 1080.0, 0.1f, 1000.0f, false),
        m_Cac(1920.0f / 1080.0f, true)
    {

    }

    EditLayer::~EditLayer()
    {

    }

    void EditLayer::OnAttach()
    {
        FramebufferSpecification fbspec;
        fbspec.Width = 1280;
        fbspec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbspec);
    }

    void EditLayer::OnDetach()
    {

    }

    void EditLayer::OnUpdate(TimeStep delta)
    {
        m_CameraController.SetCenter(m_Center);
        m_CameraController.SetCameraRotationSpeed(m_CameraRotationSpeed);
        m_CameraController.OnUpdate(delta);

        //m_ImGuiRenderBuffer->Bind();

        m_Framebuffer->Bind();
        RenderCommand::Clear();

        Renderer3D::BeginScene(m_CameraController.GetCamera());

        if (m_EnableGrid)
        {
            Renderer3D::DrawWorldGrid(m_CameraController.GetCamera());
        }

        if (m_EnvStatu)
        {
            Renderer3D::DrawEnv();
        }

        if (m_PBRstatu)
        {
            Renderer3D::DrawPBR(m_Position, glm::vec3(m_Scale), m_Rotation, m_Metalness, m_AO, m_Roughness, m_Exposure);
        }
        else if (m_ToonStatu)
        {
            Renderer3D::DrawToon(m_Position, glm::vec3(m_Scale), m_Rotation, m_lightStrength);
        }
        else
        {
            Renderer3D::DrawCube(m_Position, glm::vec3(m_Scale), m_Color, m_Rotation);
        }

        if (m_EnableCursor)
        {
            Renderer3D::DrawCursor(m_CameraController.GetCamera(), m_CameraController.GetCamera().GetCenter(), m_ViewportSize);
        }

        Renderer3D::EndScene();

        m_Framebuffer->Unbind();
    }

    void EditLayer::OnRender()
    {

    }

    void EditLayer::OnImGuiRender()
    {
        static bool DockspaceStatu = true;

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &DockspaceStatu, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        FocusTest::ViewportOnFocus = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
        FocusTest::ViewportOnHover = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        ImVec2 viewportPanelSize =  ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize)
        {
            m_ViewportSize = { viewportPanelSize.x , viewportPanelSize.y };
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(texID, ImVec2(m_ViewportSize.x,m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::Begin("Global Settings");

        ImGui::Checkbox("Enable Env", &m_EnvStatu);
        ImGui::Checkbox("Enable Grid", &m_EnableGrid);
        ImGui::Checkbox("Enable Cursor", &m_EnableCursor);
        ImGui::SliderFloat3("Center", glm::value_ptr(m_Center),-100,100);

        ImGui::End();

        ImGui::Begin("Object Settings");

        ImGui::Checkbox("Enable PBR", &m_PBRstatu);
        ImGui::Checkbox("Enable Toon", &m_ToonStatu);
        ImGui::ColorEdit4("Object Color", glm::value_ptr(m_Color));
        ImGui::DragFloat3("Object Position", glm::value_ptr(m_Position), 0.05f, -10, 10);
        ImGui::DragFloat3("Object Rotation", glm::value_ptr(m_Rotation), 1.0f, -180, 180);
        ImGui::SliderFloat("Object Scale", &m_Scale, 0.1f, 5.0f);
        ImGui::SliderFloat("Camera Rotation Speed", &m_CameraRotationSpeed, 0.0f, 100.0f);

        ImGui::End();

        ImGui::Begin("PBR");

        ImGui::SliderFloat("Metalness", &m_Metalness, 0.0f, 1.0f);
        ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("AO", &m_AO, 0.0f, 1.0f);
        ImGui::SliderFloat("Exposure", &m_Exposure, 1.0f, 10.0f);

        ImGui::End();

        ImGui::Begin("Toon");

        ImGui::SliderFloat("LightStrength", &m_lightStrength, 0.0f, 8.0f);

        ImGui::End();

        bool flag = true;
        DrawLog(&flag);

        ImGui::End();
    }

    void EditLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }

    void EditLayer::DrawLog(bool* p_open)
    {
        auto sink = Azer::Log::GetImGuiSink().get();
        if (!sink) return;

        ImGui::Begin("Log", p_open);

        if (ImGui::Button("Clear")) {
            sink->items.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Copy")) {
            ImGui::LogToClipboard();
            for (auto& line : sink->items)
                ImGui::LogText("%s\n", line.text.c_str());
            ImGui::LogFinish();
        }

        ImGui::Separator();

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (auto& item : sink->items) {
            ImVec4 color;
            switch (item.level) {
            case spdlog::level::trace:    color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); break;
            case spdlog::level::debug:    color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); break;
            case spdlog::level::info:     color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
            case spdlog::level::warn:     color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); break;
            case spdlog::level::err:      color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); break;
            case spdlog::level::critical: color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); break;
            default:                      color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
            }
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item.text.c_str());
            ImGui::PopStyleColor();
        }

        if (sink->auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
}


