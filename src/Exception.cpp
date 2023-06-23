#include <Exception.hpp>

Exception::Exception(const std::string& errorType, const std::string& errorMessage) noexcept
	: m_exceptionType{ errorType }, m_errorMessage{ errorMessage } {}

const char* Exception::what() const noexcept {
	return m_errorMessage.c_str();
}

const char* Exception::GetType() const noexcept {
	return m_exceptionType.c_str();
}
