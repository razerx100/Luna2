#ifndef WINDOW_WIN_HPP_
#define WINDOW_WIN_HPP_
#include <Window.hpp>

class WindowWin : public Window {
public:
	[[nodiscard]]
	int GetSummation(int x, int y) const noexcept override;
};
#endif
