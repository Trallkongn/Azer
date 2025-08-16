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

	glm::vec4 m_Color = { 1.0, 0.8, 0.5, 1.0 };
	glm::vec4 m_EdgeColor = { 1.0, 0.8, 0.5, 1.0 };

	float m_CameraRotationSpeed = 50.0f;

	float m_Scale = 0.5f;

};