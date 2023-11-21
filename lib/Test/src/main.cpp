#include "app/Application.hpp"

int main()
{
	Alloy::TRACE("This is a test!");
	Alloy::INFO("This is a test!");
	Alloy::WARN("This is a test!");
	Alloy::ERROR("This is a test!");
	Alloy::ASSERT(false, "This is a test!");

	Alloy::Application app;
	app.Run();
}