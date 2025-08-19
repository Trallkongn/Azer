#pragma once
#include <Azer.h>

class SandBox2D : public Azer::Layer
{
public:
	SandBox2D();
	~SandBox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Azer::TimeStep delta) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Azer::Event& event) override;

private:
	Azer::OrthoGraphicCameraController m_CameraController;
	Azer::ShaderLibrary m_ShaderLib;

	// Temp
	Azer::Ref<Azer::VertexArray> m_VertexArray;
	Azer::Ref<Azer::Texture2D> m_Texture1;
	Azer::Ref<Azer::Texture2D> m_Texture2;


	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_Color = {0.2f, 0.3f, 0.5f, 1.0f};
	glm::vec4 m_Color2 = {0.2f, 0.3f, 0.5f, 1.0f};
};