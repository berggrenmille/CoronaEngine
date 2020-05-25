// CoronaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include "ECS.h"
#include <chrono>
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



int main()
{
	Corona::ECS ecs = Corona::ECS();
	for(int i = 0; i<1024; ++i)
	{
		auto ent = ecs.RegisterEntity();
		ecs.RegisterComponent<test>(ent).a = i;


	}
	ecs.Refresh();

	auto t0 = std::chrono::high_resolution_clock::now();
	std::function a = [](int64_t id, test& e) {e.a = e.a * e.b; };
	ecs.ForEach<test>(a);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = t1 - t0;
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t2).count() << std::endl;
	//std::cout << Corona::Factory::TypeId<void>::GetFlag<test>() << std::endl;

}
