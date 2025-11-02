#include<pybind11/pybind11.h>
#include<iostream>
namespace py = pybind11;
using namespace pybind11::literals;

class Animals{
public:
    Animals(const std::string& name, int age):name_(name), age_(age){
        std::cout<<"Animals constructor called"<<std::endl;
    }
    std::string getName() const{
        return name_;
    }

    void setName(const std::string& name){
        name_ = name;
    }
    void setAge(int age){
        age_ = age;
    }
    int getAge() const{
        return age_;
    }
    void speak() const{
        std::cout<<name_<<" says: Hello! I am "<<age_<<" years old."<<std::endl;
    }
    ~Animals(){
        std::cout<<"Animals destructor called"<<std::endl;
    }
private:
    std::string name_;
    int age_;
};

PYBIND11_MODULE(myclass, m,py::mod_gil_not_used()){
    py::class_<Animals>(m, "Animals",py::dynamic_attr())
        .def(py::init<const std::string&, int>(),"name"_a="cat", "age"_a=1054819)
        .def("speak", &Animals::speak)
        .def_property("name", &Animals::getName, &Animals::setName)
        .def_property_readonly("age", &Animals::getAge)
        .def("__repr__",[](const Animals& a){
            return "<Animals name=" + a.getName() + ", age=" + std::to_string(a.getAge()) + ">";
        });
}