#include "AlloyCore/app/Application.hpp"

struct TestResource : public Alloy::Resource
{
	int x = 2;
	int y = 6;
};

struct TestResource2 : public Alloy::Resource
{
	int num = 100'000;
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

void TestSystem4(Alloy::Query<const TestComponent> query)
{
	for (auto entity : query)
	{
		auto component = query.Get<const TestComponent>(entity);

		Alloy::TRACE("TestSystem4: {0}, {1}", component.x, component.y);
	}
}

void TestSystem5(Alloy::Commands commands)
{
	for (int i = 1; i < 101; i++)
	{
		Alloy::EntityCommands entityCommands = commands.SpawnEntity();
		entityCommands.AddComponent<TestComponent>(1, i);

		if (i == 4)
		{
			entityCommands.Destroy();
		}
	}
}

void TestSystem6(Alloy::Query<TestComponent> query, const TestResource& resource)
{
	for (Alloy::EntityID id : query)
	{
		auto component = query.Get<TestComponent>(id);

		component.x = resource.x * (component.x / resource.y) % component.y;
	}
}

void TestSystem7(Alloy::AppStatus& appStatus, TestResource2& resource)
{
	Alloy::TRACE("{}", --resource.num);

	if (resource.num == 0)
		appStatus.Running = false;
}

class TestPlugin : public Alloy::Plugin
{
public:
	void Build(Alloy::Commands commands) override
	{
		commands.AddResource<TestResource>();
		commands.AddResource<TestResource2>();

		commands.AddSystem(Alloy::ScheduleID::Startup, "Startup", TestSystem);
		commands.AddSystem(Alloy::ScheduleID::Startup, "Startup", TestSystem5);

		//commands.AddSystem(Alloy::ScheduleID::Update, "Update", TestSystem2);
		//commands.AddSystem(Alloy::ScheduleID::Update, "Update", TestSystem3);
		commands.AddSystem(Alloy::ScheduleID::Update, "Update", TestSystem6);
		commands.AddSystem(Alloy::ScheduleID::Update, "Update", TestSystem7);

		commands.AddSystem(Alloy::ScheduleID::Shutdown, "Shutdown", TestSystem4);
	}
};

int main()
{
	Alloy::Application app;

	app.AddPlugin<TestPlugin>();

	app.Run();
}