#ifndef WINDOW_HPP_
#define WINDOW_HPP_

class Window {
public:
	virtual ~Window() = default;

	[[nodiscard]]
	virtual int GetSummation(int x, int y) const noexcept = 0;
};
#endif
