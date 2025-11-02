#include<pybind11/pybind11.h>
#include"myvirtual.hpp"
namespace py = pybind11;
using namespace pybind11::literals;

template <typename T=myvirtual::Person>
class PyPersonT: public T,public py::trampoline_self_life_support{
    public:
        using T::T;
        void invent() override{
            PYBIND11_OVERRIDE_PURE(void, T, invent);
        }
};

template <typename T=myvirtual::Student>
class PyStudentT:public PyPersonT<T>{
    public:
        using PyPersonT<T>::PyPersonT;
        void study() override{
            PYBIND11_OVERRIDE_PURE(void, T, study);
        }
};

template <typename T=myvirtual::Teacher>
class PyTeacherT:public PyPersonT<T>{
    public:
        using PyPersonT<T>::PyPersonT;
        void teach() override{
            PYBIND11_OVERRIDE_PURE(void, T, teach);
        }
};

PYBIND11_MODULE(myvirtual,m){
    py::class_<myvirtual::Person,PyPersonT<>,py::smart_holder>(m,"Person")
        .def(py::init<const std::string&,int>())
        .def("invent",&myvirtual::Person::invent);

    py::class_<myvirtual::Student,myvirtual::Person,PyStudentT<>,py::smart_holder>(m,"Student")
        .def(py::init<const std::string&,int>())
        .def_property("name",&myvirtual::Student::getName,&myvirtual::Student::setName)
        .def_property("age",&myvirtual::Student::getAge,&myvirtual::Student::setAge)
        .def("invent",&myvirtual::Student::invent)
        .def("study",&myvirtual::Student::study)
        .def("__repr__",[](const myvirtual::Student& self){
            return std::string("Student(name='") + self.getName() + "', age=" + std::to_string(self.getAge()) + ")";
        });
    
    py::class_<myvirtual::Teacher,myvirtual::Person,PyTeacherT<>,py::smart_holder>(m,"Teacher")
        .def(py::init<const std::string&,int>())
        .def_property("name",&myvirtual::Teacher::getName,&myvirtual::Teacher::setName)
        .def_property("age",&myvirtual::Teacher::getAge,&myvirtual::Teacher::setAge)
        .def("invent",&myvirtual::Teacher::invent)
        .def("teach",&myvirtual::Teacher::teach)
        .def("__repr__",[](const myvirtual::Teacher& self){
            return std::string("Teacher(name='") + self.getName() + "', age=" + std::to_string(self.getAge()) + ")";
        });
    
    m.def("call_invent",&myvirtual::call_invent);
}
