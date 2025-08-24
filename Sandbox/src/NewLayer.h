#pragma once
#include <Azer.h>

using namespace Azer;

class NewLayer : public Layer
{
public:
	NewLayer(const std::string& name);
	~NewLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(TimeStep delta) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& event) override;
private:

	PerspectiveGraphicCameraController m_CameraController;

	glm::vec3 m_Position = { 0.0, 0.0, 0.0 };

	glm::vec3 m_Rotation = { 0.0, 0.0, 0.0 };

	glm::vec4 m_Color = { 92.0/255.0, 214.0/255.0, 41.0/255.0, 1.0 };

	float m_CameraRotationSpeed = 50.0f;

	glm::vec3 m_Center = {0.0f,0.0f,0.0f};

	float m_Scale = 0.5f;

	bool m_EnvStatu = false;
	bool m_PBRstatu = false;
	bool m_ToonStatu = false;

	float m_Metalness = 1.0f;
	float m_AO = 1.0f;
	float m_Roughness = 1.0f;
	float m_Exposure = 1.0f;
};