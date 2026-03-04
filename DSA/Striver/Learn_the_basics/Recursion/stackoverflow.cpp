#include<iostream>
//stackoverflow
void print() {
    std::cout << "Hello, World!" << std::endl;
    print();
}

int main(){

	print();
    return 0;
}