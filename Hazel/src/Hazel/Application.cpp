#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"


namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{	
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		//Application is constructed
		//	Window::Create will call WindowsWindow::Create,
		//		which calls the WindowsWindow constructor, 
		//			which calls WindowsWindo::Init()
		m_Window = std::unique_ptr<Window>(Window::Create());
		
		//Bind which function is called OnEventCallback
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}
	
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//Events handled reverse order, i.e overlays are at end of layerstack (buttons)
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			auto [x, y] = Input::GetMousePosition();
			HZ_CORE_TRACE("Mouse Position: {0}, {1}", x, y);

			auto state = Input::IsMouseButtonPressed(0);
			HZ_CORE_TRACE("Is LMB Down: {0}", state);

			auto keyState = Input::IsKeyPressed(32);
			HZ_CORE_TRACE("Is Sapce Down: {0}", keyState);
			
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}//End namespace Hazel 