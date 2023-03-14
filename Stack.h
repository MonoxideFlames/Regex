#pragma once
#include <iostream>
template<typename T> 
class Stack {
	unsigned int top;
	unsigned int capacity;
	T* arr;
	void resize(unsigned int newCap){
		T* newArr = new T[newCap];
		unsigned int i = capacity - 1, j = newCap - 1;
		for(; i > top; i--, j--){
			newArr[j] = arr[i];
		}
		newArr[j] = arr[i];

		top = j;
		capacity = newCap;
		delete[] arr;
		arr = newArr;
	}
public:
	T pop(){
		if(capacity > 16 && top >= (3*capacity >> 2)){
			resize(capacity >> 2);
		}
		return arr[top++];
	}
	void push(T obj){
		if(top == 0){
			resize(capacity << 1);
		}
		arr[--top] = obj;
	}
	T peek(){
		return arr[top];
	}
	unsigned int size(){
		return capacity - top;
	}
	T& operator[](unsigned int i){
		return arr[top + i];
	}
	Stack(){
		arr = new T[8];
		capacity = 8;
		top = 8;
	}
	~Stack(){
		delete[] arr;
	}
};
