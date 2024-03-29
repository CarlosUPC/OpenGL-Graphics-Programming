#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
	GLFWwindow* window;


	/* Initialize GLFW library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Init GLEW Library Loader
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	//Vertex positions
	float positions[] = {
		-50.0f, -50.0f, 0.0f, 0.0f, //0
		 50.0f, -50.0f, 1.0f, 0.0f, //1
		 50.0f,  50.0f, 1.0f, 1.0f, //2
		-50.0f,  50.0f, 0.0f, 1.0f  //3
	};

	//Vertex Indices
	unsigned int indices[] = {
		0, 1, 2,
		3, 2, 0
	};



	//Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Vertex Array
	VertexArray vao;

	//Vertex Buffer
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	//Vertex Layout
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vb, layout);
	
	//Index Buffer
	IndexBuffer ib(indices, 6);
	
	// View Model Projection
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	
	//Shaders
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
	
	//Texture
	Texture tex("res/textures/OpenGL.png");
	tex.Bind();
	shader.SetUniform1i("u_Texture", 0);

	//Unbound buffers and shader
	vao.UnBind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	//Renderer
	Renderer render;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();

	glm::vec3 translationA(200, 200, 0);
	glm::vec3 translationB(400, 200, 0);


	float r = 0.0f;
	float increment = 0.05f;
	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		render.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		{

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			render.Draw(vao, ib, shader);

		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			render.Draw(vao, ib, shader);

		}

		


		if (r > 1.0f)
			increment = -0.05;
		else if (r < 0.0f)
			increment = 0.05;

		r += increment;
		{
	
			ImGui::SliderFloat3("Translation A", (float*)&translationA, 0.0f, 960.0f);    
			ImGui::SliderFloat3("Translation B", (float*)&translationB, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}