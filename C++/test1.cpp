#include<iostream>
#include<functional>
#include<string>

int test(std::function<std::string(int,int)>func){
    int a = 10;
    int b = 20;
    std::cout<<func(a,b)<<std::endl;
    return a + b;
}

int main(){
    int ret = test([](int t1,int t2){
        return std::to_string(t1 + t2);
    });

    std::cout<<ret<<std::endl;
}


