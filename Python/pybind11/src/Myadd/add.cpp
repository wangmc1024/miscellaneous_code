#include<pybind11/pybind11.h>
#include<iostream>
namespace py = pybind11;
using namespace pybind11::literals;

int add_without_arg(int a, int b){
    std::cout<<" a is"<<a<<std::endl;
    std::cout<<" b is"<<b<<std::endl;
    std::cout<<"hello world"<<std::endl;
    return a + b;
}

int add_with_arg(int a, int b){
    std::cout<<" a is"<<a<<std::endl;
    std::cout<<" b is"<<b<<std::endl;
    std::cout<<"hello world"<<std::endl;
    return a + b;
}

int add_with_default_arg(int a, int b){
    std::cout<<" a is"<<a<<std::endl;
    std::cout<<" b is"<<b<<std::endl;
    std::cout<<"hello world"<<std::endl;
    return a + b;
}
PYBIND11_MODULE(Myadd, m,py::mod_gil_not_used()){
    m.attr("char_a") = "nihao,shijie";
    m.attr("char_b") = "hello,world";

    py::object world = py::cast("world");//explicit cast
    m.def("add_without_arg", &add_without_arg, "a"_a, "b"_a);
    m.def("add_with_arg", &add_with_arg, "a"_a, "b"_a);
    m.def("add_with_default_arg", &add_with_default_arg, "a"_a, "b"_a=100);
}
