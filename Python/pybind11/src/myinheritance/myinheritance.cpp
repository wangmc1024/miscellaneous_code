#include<pybind11/pybind11.h>
#include<iostream>
namespace py = pybind11;
using namespace pybind11::literals;

class People{
public:
    People(const std::string& name, int age):name_(name), age_(age){
        std::cout<<"People constructor called"<<std::endl;
    }
    std::string getName() const{
        return name_;
    }
    void setName(const std::string& name){
        name_ = name;
    }
    int getAge() const{
        return age_;
    }
    void setAge(int age){
        age_ = age;
    }

    virtual void invent() = 0;
    virtual ~People(){
        std::cout<<"People destructor called"<<std::endl;
    }
private:
    std::string name_;
    int age_;
};


class ComputerScientist:public People{
public:
    ComputerScientist(const std::string& name, int age):People(name, age){
        std::cout<<"ComputerScientist constructor called"<<std::endl;
        std::cout<<"hello everybody, I am a computer scientist.😁😁😁"<<std::endl;
    }
    ~ComputerScientist(){
        std::cout<<"ComputerScientist destructor called"<<std::endl;
    }
    void invent() override{
        std::cout<<getName()<<" is inventing a new computer."<<std::endl;
    }
};

PYBIND11_MODULE(myinheritance,m,py::mod_gil_not_used()){
    py::class_<People>(m,"People");
    //     .def_property("name", &People::getName, &People::setName)
    //     .def_property_readonly("age", &People::getAge);

    py::class_<ComputerScientist,People>(m,"ComputerScientist")
        .def(py::init<const std::string&, int>())
        .def("invent", &ComputerScientist::invent)
        .def("__repr__", [](const ComputerScientist& self){
            return "<ComputerScientist name=" + self.getName() + ", age=" + std::to_string(self.getAge()) + ">";
        });
}