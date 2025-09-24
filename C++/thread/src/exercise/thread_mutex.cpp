#include<thread>
#include<iostream>
#include<string>
#include<mutex>
static std::mutex mtx;
void TestThread(){
	while(1){
	if(!mtx.try_lock()){
		std::cout<<"."<<std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		continue;
	}
	std::cout<<"===================="<<std::endl;
	std::cout<<"test 001"<<std::endl;
	std::cout<<"test 002"<<std::endl;
	std::cout<<"test 003"<<std::endl;
	std::cout<<"===================="<<std::endl;
	mtx.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));	
	}
}

void ThreadMainMux(int i){
	for(;;){
		mtx.lock();
		std::cout<<i<<"[in]"<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main(int argc,char* argv[]){
	for(int i = 0; i <3;++i){
		std::thread th(ThreadMainMux,i+1);
		th.detach();
	}
	getchar();
	return 0;
}