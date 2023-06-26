#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include <string>
#include <cstdint>

class Window {
public:
	virtual ~Window() = default;

	virtual void WaitForMessageLoop() = 0;

	virtual void SetWindowTitle(const std::string& title) = 0;
	virtual void SetWindowResolution(std::uint32_t width, std::uint32_t height) = 0;
	virtual void EnableCursor() noexcept = 0;
	virtual void DisableCursor() noexcept = 0;
	virtual void ConfineCursor() noexcept = 0;
	virtual void FreeCursor() noexcept = 0;
	virtual void SetWindowIcon(const std::wstring& iconPath) = 0;

	[[nodiscard]]
	virtual bool IsCursorEnabled() const noexcept = 0;
	[[nodiscard]]
	virtual bool IsMinimised() const noexcept = 0;
	[[nodiscard]]
	virtual float GetAspectRatio() const noexcept = 0;
	[[nodiscard]]
	virtual void* GetWindowHandle() const noexcept = 0;
	[[nodiscard]]
	virtual void* GetModuleInstance() const noexcept = 0;
};
#endif
