#include <WindowWin.hpp>
#include <Exception.hpp>

WindowWin::WindowWin(std::uint32_t width, std::uint32_t height, const std::string& name)
	: m_wndClass{}, m_windowHandle{ nullptr }, m_width{ width }, m_height{ height },
	m_windowStyle{ WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU } {

	RECT wr{
	.left = 0,
	.top = 0,
	.right = static_cast<LONG>(width),
	.bottom = static_cast<LONG>(height)
	};

	AdjustWindowRect(&wr, m_windowStyle, FALSE);

	WindowCreateStruct windowStruct{
		.windowName = name,
		.wndClass = m_wndClass,
		.windowStyle = m_windowStyle,
		.width = static_cast<int>(wr.right - wr.left),
		.height = static_cast<int>(wr.bottom - wr.top),
		.thisPtr = this
	};

	std::promise<HWND> windowPromise;
	std::future<HWND> fWindowHandle = windowPromise.get_future();

	m_messageLoopExited = std::async(
		std::launch::async,
		&WindowWin::AsyncThreadFunc, std::move(windowPromise), windowStruct
	);

	m_windowHandle = fWindowHandle.get();
}

void WindowWin::WaitForMessageLoop() {
	m_messageLoopExited.get();
}

WindowWin::~WindowWin() noexcept {
	// Won't receive any exceptions, since the destructor is noexcept
	m_messageLoopExited.get();
}

void WindowWin::SetWindowTitle(const std::string& title) {
	// PostMessage doesn't work here, since title becomes invalid as soon as this
	// function exits
	SendMessage(m_windowHandle, WM_SETTEXT, 0u, reinterpret_cast<LPARAM>(title.c_str()));
}

void WindowWin::SetWindowResolution(std::uint32_t width, std::uint32_t height) {

}

bool WindowWin::IsMinimised() const noexcept {
	return true;
}

float WindowWin::GetAspectRatio() const noexcept {
	return static_cast<float>(m_width) / m_height;
}

void* WindowWin::GetWindowHandle() const noexcept {
	return m_windowHandle;
}

void* WindowWin::GetModuleInstance() const noexcept {
	return m_wndClass.GetHInstance();
}

void WindowWin::AsyncThreadFunc(
	std::promise<HWND> windowPromise, WindowCreateStruct windowStruct
) {
	bool windowCreated = CreateWindowThreadSafe(std::move(windowPromise), windowStruct);

	if (windowCreated)
		MessageLoop();
}

void WindowWin::MessageLoop() {
	MSG msg{};
	BOOL mRet = 1;

	while ((mRet = GetMessage(&msg, nullptr, 0u, 0u)) != 0) {
		if (mRet == -1)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool WindowWin::CreateWindowThreadSafe(
	std::promise<HWND> windowPromise, WindowCreateStruct windowStruct
) {
	WndClass& wndClass = windowStruct.wndClass;
	wndClass.Register();

	HWND windowHandle = CreateWindowEx(
		0,
		wndClass.GetName(), windowStruct.windowName.c_str(),
		windowStruct.windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowStruct.width,
		windowStruct.height,
		nullptr, nullptr, wndClass.GetHInstance(), windowStruct.thisPtr
	);

	if (!windowHandle) {
		std::exception_ptr exPtr = std::make_exception_ptr(
			Exception("Window Exception", "Failed to create the window.")
		);

		windowPromise.set_exception(exPtr);

		return false;
	}

	ShowWindow(windowHandle, SW_SHOWDEFAULT);

	windowPromise.set_value(windowHandle);

	return true;
}

LRESULT CALLBACK WindowWin::WindowProcInitial(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
) noexcept {
	if (msg == WM_NCCREATE) {
		auto pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		auto pWnd = static_cast<WindowWin*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(
			hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowWin::WindowProcWrapper)
		);

		return pWnd->WindowProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WindowWin::WindowProcWrapper(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
) {
	auto pWnd = reinterpret_cast<WindowWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->WindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WindowWin::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: {
		DestroyWindow(m_windowHandle);

		return 0;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);

		return 0;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
