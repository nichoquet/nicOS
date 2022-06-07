template <typename T>
class ListItem {
    ListItem<T>* next;
    ListItem<T>* last;
};
template <typename T>
class List {
    private:
        int nbItemMax;
        ListItem<T> *items[];
    public:
        List<T>(unsigned int nbItemMax) {
            this->nbItemMax = nbItemMax;
        }
};

// template <class T>
// List<int> * create_list(int nbThreadMax);