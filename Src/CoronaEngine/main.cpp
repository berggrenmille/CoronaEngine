// CoronaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include "World.h"
#include <chrono>
#include <sdl/SDL.h>
#include <glad/glad.h>
#include "Log.h"
#include "Event.h"
/* Example registration
 * #include <rttr/registration>

using namespace rttr;
struct MyStruct { MyStruct() {}; void func(double) {}; int data; };
RTTR_REGISTRATION
{
	std::function<MyStruct & (int)> func = [&](int a) ->MyStruct& {MyStruct& test = *(new MyStruct()); test.data = a; return test; };
	registration::class_<MyStruct>("MyStruct")
		 .constructor<>(func)
		 .property("data", &MyStruct::data)
		 .method("func", &MyStruct::func);
}

	type t = type::get_by_name("MyStruct");
	variant var = t.create();    // will invoke the previously registered ctor
	variant var2 = t.get_property_value("data", var);
*/
struct test
{
	int a = 1, b = 11;
};

RTTR_REGISTRATION
{
	rttr::registration::class_<test>("test")
		 .constructor<>()
		 .property("a", &test::a)
		 .property("b", &test::b);
}

struct OK{};

int main(int argc, char* argv[])
{
	Corona::Debug::Init();

	dexode::EventBus::Listener listener{Corona::Event::MainBus };
	listener.listen([](const OK& event) {DEBUG_TRACE("GOT EVENT"); });
	Corona::Event::MainBus->postpone(OK{});
	Corona::Event::MainBus->process();
	
	Corona::World world = Corona::World();

	for(int i = 0; i<1024; ++i)
	{
		auto ent = world.ecs.RegisterEntity();
		world.ecs.RegisterComponent<test>(ent).a = i;


	}
	world.ecs.Refresh();

	auto t0 = std::chrono::high_resolution_clock::now();
	auto world2 = world;
	auto a = [](int64_t id, test& e) {std::cout << e.a << std::endl; };
	world2.ecs.ForEach<test>(a);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = t1 - t0;

	//std::cout << Corona::Factory::TypeId<void>::GetFlag<test>() << std::endl;
	return 0;
}
