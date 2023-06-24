#ifndef WINDOW_WIN_HPP_
#define WINDOW_WIN_HPP_
#include <Window.hpp>
#include <future>
#include <CleanWin.hpp>

class WindowWin final : public Window {
public:
	WindowWin(std::uint32_t width, std::uint32_t height, const std::string& name);
	~WindowWin() noexcept;

	void WaitForMessageLoop() override;

	void SetWindowTitle(const std::string& title) override;
	void SetWindowResolution(std::uint32_t width, std::uint32_t height) override;

	[[nodiscard]]
	bool IsMinimised() const noexcept override;
	[[nodiscard]]
	float GetAspectRatio() const noexcept override;
	[[nodiscard]]
	void* GetWindowHandle() const noexcept override;
	[[nodiscard]]
	void* GetModuleInstance() const noexcept override;

private:
	[[nodiscard]]
	static LRESULT CALLBACK WindowProcInitial(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
	) noexcept;
	[[nodiscard]]
	static LRESULT CALLBACK WindowProcWrapper(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
	);

	[[nodiscard]]
	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	class WndClass {
	public:
		WndClass() noexcept : m_wndClass{
				.cbSize = static_cast<UINT>(sizeof(m_wndClass)),
				.style = CS_OWNDC,
				.lpfnWndProc = WindowProcInitial,
				.lpszClassName = GetName()
		} {}

		~WndClass() noexcept {
			UnregisterClass(GetName(), m_wndClass.hInstance);
		}

		void Register() noexcept {
			m_wndClass.hInstance = GetModuleHandle(nullptr);

			RegisterClassEx(&m_wndClass);
		}

		[[nodiscard]]
		HINSTANCE GetHInstance() const noexcept {
			return m_wndClass.hInstance;
		}
		[[nodiscard]]
		static constexpr const char* GetName() noexcept {
			return wndClassName;
		}

	private:
		static constexpr const char* wndClassName = "Luna";
		WNDCLASSEX m_wndClass;
	};

	struct WindowCreateStruct {
		std::string windowName;
		WindowWin::WndClass wndClass;
		DWORD windowStyle;
		int width;
		int height;
		LPVOID thisPtr;
	};

private:
	static void MessageLoop();
	[[nodiscard]]
	static bool CreateWindowThreadSafe(
		std::promise<HWND> windowPromise, WindowCreateStruct windowStruct
	);
	static void AsyncThreadFunc(
		std::promise<HWND> windowPromise, WindowCreateStruct windowStruct
	);

	void ToggleFullScreenMode();

private:
	WndClass m_wndClass;
	RECT m_windowRect;
	HWND m_windowHandle;
	std::uint32_t m_width;
	std::uint32_t m_height;
	DWORD m_windowStyle;
	std::future<void> m_messageLoopExited;
	bool m_minimised;
	bool m_fullScreenMode;
};
#endif
