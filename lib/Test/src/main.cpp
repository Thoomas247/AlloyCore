#include "AlloyCore/app/Application.hpp"

struct TestResource : public Alloy::Resource
{
	int x;
	int y;
};

struct TestComponent : public Alloy::Component
{
	int x;
	int y;

	TestComponent(int x, int y)
		: x(x), y(y)
	{
	}
};

void TestSystem(const TestResource& resource)
{
	Alloy::TRACE("TestSystem: {0}, {1}", resource.x, resource.y);
}

void TestSystem2(TestResource& resource)
{
	resource.x = 10;
	resource.y = 20;
}

void TestSystem3(Alloy::AppStatus& appStatus, Alloy::Query<TestComponent> query)
{
	for (auto entity : query)
	{
		auto component = query.Get<TestComponent>(entity);

		Alloy::TRACE("TestSystem3: {0}, {1}", component.x, component.y);
	}

	appStatus.Running = false;
}

void TestSystem4(Alloy::Query<const TestComponent> query, const TestResource& resource)
{
	for (auto entity : query)
	{
		auto component = query.Get<const TestComponent>(entity);

		Alloy::TRACE("TestSystem4: {0}, {1}", component.x, component.y);
	}
}


class TestPlugin : public Alloy::Plugin
{
public:
	void Build(Alloy::Application& app) override
	{
		app.AddResource<TestResource>();

		app.AddSystem(Alloy::ScheduleID::Startup, "Startup", TestSystem);
		app.AddSystem(Alloy::ScheduleID::Update, "Update", TestSystem2);
		app.AddSystem(Alloy::ScheduleID::Update, "Update", TestSystem3);
		app.AddSystem(Alloy::ScheduleID::Shutdown, "Shutdown", TestSystem4);
	}
};


#include "AlloyCore/commands/Commands.hpp"

int main()
{
	Alloy::Application app;

	/*--------------------------------------------------------*/
	Alloy::Internal::AppState appState;
	appState.ResourceList.AddResource<TestResource>();

	Alloy::Internal::CommandList commandList;
	Alloy::Commands commands = Alloy::Commands(commandList, appState);

	commands.SpawnEntity().AddComponent<TestComponent>(10, 20);

	commandList.Run(appState);

	/*--------------------------------------------------------*/

	app.AddPlugin<TestPlugin>();

	app.Run();

	Alloy::TRACE("This is a test!");
	Alloy::INFO("This is a test!");
	Alloy::WARN("This is a test!");
	Alloy::ERROR("This is a test!");
	Alloy::ASSERT(false, "This is a test!");
}