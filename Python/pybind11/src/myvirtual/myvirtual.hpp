#pragma once
#include<string>
namespace myvirtual{
    class Person{
        public:
            Person(const std::string& name,int age);
            std::string getName() const;
            int getAge() const;
            void setName(const std::string& name);
            void setAge(int age);
            virtual void invent() = 0;
            virtual ~Person();
        private:
            std::string name_;
            int age_;
    };
    class Student:public Person{
        public:
            using Person::Person;
            void invent() override;
            virtual void study();
            virtual ~Student();
    };
    class Teacher:public Person{
        public:
            using Person::Person;
            void invent() override;
            virtual void teach();
            virtual ~Teacher();
    };

    void call_invent(Person& person);
};
