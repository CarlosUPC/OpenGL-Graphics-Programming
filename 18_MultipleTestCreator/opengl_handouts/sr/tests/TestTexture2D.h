#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
//#include "VertexBuffer.h"
#include "Texture.h"

#include <memory>

namespace test {

	class TestTexture2D : public Test {
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGUIRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture2D;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
	};
}
