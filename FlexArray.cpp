#include "FlexArray.h"
FlexArray::FlexArray() {
	size_ = 0;
	capacity_ = INITIALCAP;
	arr_ = new int[capacity_];
	head_=capacity_/2;
	tail_=head_-1;
}
FlexArray::FlexArray(const int* arr, int size) {
	capacity_ = LO_THRESHOLD * size;
	arr_ = new int[capacity_];
	int start = (capacity_ -size)/2; //start at centre
	for(int i=0; i<capacity_; i++){
		if(i<start || i>=start + size)arr_[i]= NULL;
		else arr_[i] = arr[i-start]; 
	}
	size_ = size;
	head_ = start;
	tail_ = start + size-1;
}
FlexArray::~FlexArray() {
	delete[] arr_;
}
FlexArray::FlexArray(const FlexArray& other) {
	//copy all members
	size_ = other.size_;
	capacity_ = other.capacity_;
	head_ = other.head_;
	tail_ = other.tail_;
	//new memory space for array
	arr_ = new int[capacity_];
	//copy everything into new array
	for (int i = 0; i < capacity_; ++i) {
		arr_[i] = other.arr_[i];
	}
}
FlexArray& FlexArray::operator=(const FlexArray& other) {
	if (this == &other) return *this;
    delete[] arr_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    head_ = other.head_;
    tail_ = other.tail_;
    arr_ = new int[capacity_];
	//Copy all members into new array
    for (int i = 0; i < capacity_; ++i){
		arr_[i] = other.arr_[i];
	} 
    return *this;
}
int FlexArray::getSize() const {
	return size_;
}
int FlexArray::getCapacity() const {
	return capacity_;
}
string FlexArray::print() const {
    std::string printString = "["; //stringbuilder
    if (size_ > 0) { 
        for (int i = 0; i < size_; ++i) {
            printString += std::to_string(arr_[head_ + i]);
            if (i < size_ - 1) {
                printString += ", ";
            }
        }
    }
    printString += "]";
    return printString;
}
string FlexArray::printAll() const {
	std::string printString = "["; //stringbuilder
	for(int i=0; i<capacity_;i++) {
		if(i < head_ || i > tail_)printString += "X";
		else printString += std::to_string(arr_[i]);		
		if (i < capacity_ - 1) printString += ", ";
	}
	printString += "]";
    return printString;
}
int FlexArray::get(int i) const {
	if (i < 0 || i >= size_) return -1;
	int internalIndex = head_ + i;
	return arr_[internalIndex];
}
bool FlexArray::set(int i, int v) {
	if (i < 0 || i >= size_) return false;
	int internalIndex = head_ + i;
	arr_[internalIndex] = v;
	return true;
}
void FlexArray::push_back(int v) {
	//check for resize
    if (size_ >= capacity_ || (tail_ == capacity_ - 1 && size_ < capacity_)) resize();
	tail_++;
    arr_[tail_] = v;
    size_++; 
}
bool FlexArray::pop_back() {
	if (size_ == 0)return false;
    size_--;
	tail_--; //item now popped
    if (size_ > 0 && capacity_ > HI_THRESHOLD * size_) resize();
	if(size_==0) {
		head_ = capacity_ / 2; //recentre
        tail_ = head_-1;
	}
    return true;
}
void FlexArray::push_front(int v) {
    if (size_ >= capacity_  || (head_ == 0 && size_ < capacity_)) resize();
    if (size_ == 0) {
        head_ = capacity_ / 2;
        tail_ = head_;
    } else head_--;//item will be pushed into this space, which is being freed
    arr_[head_] = v; //push
    size_++; 

}
bool FlexArray::pop_front() {
	if (size_ == 0)return false;
    size_--;
	head_++;//item now popped, head takes the space
    if (size_ > 0 && capacity_ > HI_THRESHOLD * size_) resize();
	if(size_==0){
		head_ = capacity_ / 2; //recentre
        tail_ = head_-1;
	}
    return true;
}
bool FlexArray::insert(int i, int v) {
	if(i<0 || i>size_)return false; 
	if (size_ == capacity_) {
        resize();
		insert(i, v); //resize then business as usual
    } 
    else if (head_==0) { //if were running out of space after head/tail in next case
        shift_Right(i);
        arr_[head_ + i] = v;
    }
    else if (tail_==capacity_-1) {
        shift_Left(i);
        arr_[head_ + i] = v;
    }
	else { //normal insertion, calculating correct side too
		if(i>=((float)size_/2)) { //will always shift right if size=0
			shift_Right(i);
			arr_[head_ + i] = v;
		}
		else{
			shift_Left(i);
			arr_[head_ + i] = v;
		}
	}
	if(size_==0){ //accounts for previous comment and shifts
		shift_Left(1);
		tail_--;
	}
	size_++;
	return true;
}
bool FlexArray::erase(int i) 
{
    if (i < 0 || i >= size_) return false; 
	//deletes item then shifts in right direction according to index provided
    if (i >= size_ / 2) {
        for (int j = i; j < size_ - 1; ++j)  {
			arr_[head_ + j] = arr_[head_ + j + 1];
		}
        tail_--; 
    } 
	else {
        for (int j = i; j > 0; --j) {
			arr_[head_ + j] = arr_[head_ + j - 1];
		}
        head_++; 
    }
    size_--; 
    if (size_ > 0 && capacity_ > HI_THRESHOLD * size_) resize();
	else if(size_==0) {//Recentre if empty
		head_ = capacity_ / 2;
        tail_ = head_-1;
	}
    return true;
}
void FlexArray::shift_Left(int i) 
{
	for (int j = 0; j < i; ++j) {
		arr_[head_ + j - 1] = arr_[head_ + j];
	}
	head_--;
}
void FlexArray::shift_Right(int i)
{
	for (int j = size_; j > i; --j) {
		arr_[head_ + j] = arr_[head_ + j - 1];
	}
	tail_++; // Increment the tail position.
}
void FlexArray::resize()
{
	int new_capacity = LO_THRESHOLD * size_;
	int* new_arr = new int[new_capacity];
	int new_head = (new_capacity - size_) / 2;
	for (int i = 0; i < size_; ++i) {
		new_arr[new_head + i] = arr_[head_ + i]; //copy array 
	}
	delete[] arr_;
	arr_ = new_arr;
	head_ = new_head;
	tail_ = head_ + size_ - 1; // The tail points to the last element.
	capacity_ = new_capacity;
}

