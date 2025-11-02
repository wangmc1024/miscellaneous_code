#include"myvirtual.hpp"
#include<iostream>
namespace myvirtual{
    Person::Person(const std::string& name,int age):name_(name),age_(age){}
    std::string Person::getName() const{
        return name_;
    }
    int Person::getAge() const{
        return age_;
    }
    void Person::setName(const std::string& name){
        name_ = name;
    }
    void Person::setAge(int age){
        age_ = age;
    }
    Person::~Person(){}

    void Student::invent(){
        std::cout<<"Student "<<getName()<<" is inventing."<<std::endl;
    }
    void Student::study(){
        std::cout<<"Student "<<getName()<<" is studying."<<std::endl;
    }
    Student::~Student(){}

    void Teacher::invent(){
        std::cout<<"Teacher "<<getName()<<" is inventing."<<std::endl;
    }
    void Teacher::teach(){
        std::cout<<"Teacher "<<getName()<<" is teaching."<<std::endl;
    }
    Teacher::~Teacher(){}

    void call_invent(Person& person){
        person.invent();
    }
};
