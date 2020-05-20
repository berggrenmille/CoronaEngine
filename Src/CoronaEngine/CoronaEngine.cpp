// CoronaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include "ECS.h"

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
	for(int i = 0; i<10; ++i)
	{
		auto ent = Corona::ECS::RegisterEntity();
		Corona::ECS::RegisterComponent<test>(ent);

	}

	std::function a = [](test& e) {std::cout << e.a << std::endl; };
	Corona::ECS::ForEach(a);
	std::cout << Corona::Factory::TypeId<void>::GetFlag<test>() << std::endl;

}
