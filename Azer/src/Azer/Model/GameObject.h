#pragma once
#include "Azer/Core/Core.h"
#include "Azer/Core/UUID.hpp"

#include <string>

namespace Azer {

	class GameObject
	{
	public:
		GameObject(const std::string& name)
			: m_ObjectName(name)
		{
			id = UUID::GenerateV4();
		}
		virtual ~GameObject() {}

		const std::string& GetId() const { id.ToString(); }
	private:
		UUID id;
		std::string m_ObjectName;
	};
}