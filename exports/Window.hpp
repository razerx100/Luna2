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
