template<typename T>
class List {
    private:
        int nbItems;
        int nbItemMax;
        T *items;
    public:
        List<T>(){
            this->nbItemMax = 1;
            this->items = new T[1];
        }
        ~List<T>(){
            delete[] this->items;
        }
        void push (T item) {
            if (this->nbItems + 1 >= this->nbItemMax) {
                this->resize(this->nbItemMax * 2);
            }
            this->items[this->nbItems++] = item;
        }
        T get (int index) {
            return this->items[index];
        }
        T pop() {
            return this->items[--this->nbItems];
        }
        int length() {
            return this->nbItems;
        }
        void resize (int newSize) {
            T* newItems = new T[newSize];
            for (int i = 0; i < this->nbItemMax && i < newSize; i++) {
                newItems[i] = this->items[i];
            }
            delete[] this->items;
            this->nbItemMax = newSize;
            this->items = newItems;
        }
};