#pragma once
#include <Azer/Core/Core.h>
#include <glm/glm.hpp>

namespace {

	class GraphicCamera
	{
	public:
		virtual ~GraphicCamera() = default;

		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual const glm::vec3& GetPosition()	const   	= 0;
													
		virtual const glm::mat4& GetProjectionMatrix() 		const = 0;
		virtual const glm::mat4& GetViewMatrix()			const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix()	const = 0;

	private:

		virtual void ReCalculateViewMatrix() = 0;
	};
}