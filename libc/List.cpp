#include "List.h"

// template<typename T>
// List<T>::List(unsigned int nbItemMax) {
//     this->nbItemMax = nbItemMax;
//     this->items = new T[nbItemMax];
// }

// template<typename T>
// List<T>::~List() {
//     delete this->items;
// }

// template<typename T>
// void List<T>::push (T item) {
//     if (this->nbItems + 1 >= this->nbItemMax) {
//         this->resize(this->nbItemMax * 2);
//     }
//     this->items[this->nbItems++] = item;
// }

// template<typename T>
// T List<T>::get (int index) {
//     return this->items[index];
// }

// template<typename T>
// T * List<T>::pop() {
//     return this->items[--this->nbItems];
// }

// template<typename T>
// int List<T>::length() {
//     return this->nbItems;
// }

// template<typename T>
// void List<T>::resize (int newSize) {
//     T* newItems = new T[newSize];
//     for (int i = 0; i < this->nbItemMax && i < newSize; i++) {
//         newItems[i] = this->items[i];
//     }
//     delete[] this->items;
//     this->nbItemMax = newSize;
//     this->items = newItems;
// }