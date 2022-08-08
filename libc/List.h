template <typename T>
class ListItem {
    private:
        T* item;
        ListItem<T>* next;
        ListItem<T>* last;
    public:
        ListItem<T>(){
            // this->item = item;
        }
};
template <typename T>
class List {
    private:
        int nbItemMax;
        ListItem<T> *items;
    public:
        List<T>(unsigned int nbItemMax) {
            this->nbItemMax = nbItemMax;
            this->items = new ListItem<T>[nbItemMax];
        }
};

// template <class T>
// List<int> * create_list(int nbThreadMax);