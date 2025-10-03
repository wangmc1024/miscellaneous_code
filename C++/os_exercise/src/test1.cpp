#include<iostream>

void test(unsigned int a){
    for(int i = 0; i < a -1;i++){
        std::cout<<i<<std::endl;
        if(i == 4)break;
    }
}

int main(){
    int a = 3;
    int b = 0;
    test(a);
    std::cout<<"--------------"<<std::endl;
    test(b);

    return 0;
}