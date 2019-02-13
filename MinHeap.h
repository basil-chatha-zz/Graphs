#include <vector>
#include <iostream>
#include <stdexcept>

template <class T>
struct Item {
  T myItem;
  int priority;

  Item(T i, int p) {
    myItem = i;
    priority = p;
  }
};
/* Used Item struct so I could pair the item's with
their respective priorities. Could have used std::pair to
do the same thing */

template <class T>
class MinHeap {
public:
  MinHeap (int d) {
    _d = d;
  }
  /* Constructor that builds a d-ary Min Heap
  This should work for any d >= 2,
  but doesn't have to do anything for smaller d.*/

  ~MinHeap () {
    for(size_t i = 0; i < _minHeap.size(); i++) {
      delete _minHeap.at(i);
    } _minHeap.clear();
  }
  /* destructor */

  void add (T item, int priority) {
    Item<T> *newItem = new Item<T>(item, priority);

    _minHeap.push_back(newItem);
    trickleUp((int) _minHeap.size() - 1);
  }
  /* adds the item to the heap, with the given priority. */

  const T & peek () const {
    if(!isEmpty()) {
      return (_minHeap.at(0))->myItem;
    } else {
      throw std::logic_error("Empty heap.");
    }
  }
  /* returns the element with smallest priority.  
  Break ties however you wish.  
  Throws an exception if the heap is empty. */

  void remove () {
    if(!isEmpty()) {
      delete _minHeap.at(0);
      _minHeap.at(0) = _minHeap.at(_minHeap.size() - 1);
      
      int greatestChildLocation = greatestLeafNode();

      _minHeap.at(0) = _minHeap.at(greatestChildLocation);

      _minHeap.erase(_minHeap.begin() + greatestChildLocation);

      trickleDown(0);
    } else {
      throw std::logic_error("Empty heap.");
    }
  }
  /* removes the element with smallest priority.
  Break ties however you wish.
  Throws an exception if the heap is empty. */

  bool isEmpty () const { return _minHeap.empty(); }
  /* returns true iff there are no elements on the heap. */

private:
// whatever you need to naturally store things.
// You may also add helper functions here
  int _d;
  std::vector<Item<T>*> _minHeap;

  void swap(int location, int parentLocation) {
    Item<T> *temp = _minHeap.at(location);

    _minHeap.at(location) = _minHeap.at(parentLocation);
    _minHeap.at(parentLocation) = temp;
  }

  int greatestLeafNode() {
    if((int) _minHeap.size() == 1) return 0;
    int parent = (_minHeap.size() - 2) / _d;
    int largestIndex = (_d * parent) + 1;
    int i = (_d * parent) + 1;

    while((i < (int) _minHeap.size()) && (i <= ((_d * parent) + _d))) {
      if(_minHeap.at(i)->priority > _minHeap.at(largestIndex)->priority) {
        largestIndex = i;
      } i++;
    }

    return largestIndex;
  }

  int smallestChild(int parentLocation) {
    if((int) _minHeap.size() == 1) return 0;
    int smallestIndex = (_d * parentLocation) + 1;
    int i = (_d * parentLocation) + 1;
    while((i < (int) _minHeap.size()) && (i <= ((_d * parentLocation) + _d))) {
      if(_minHeap.at(i)->priority < _minHeap.at(smallestIndex)->priority) {
        smallestIndex = i;
      } i++;
    }

    return smallestIndex;
  }

  void trickleDown(int location) {
    int smallestChildLocation = smallestChild(location);

    if(smallestChildLocation < (int) _minHeap.size()) {
      if(_minHeap.at(smallestChildLocation)->priority < _minHeap.at(location)->priority) {
        swap(smallestChildLocation, location);
        trickleDown(smallestChildLocation);
      } else return;
    }
  }

  void trickleUp(int location) {
    int parentLoc = (location - 1) / _d;

    if(location > 0) {
      if(_minHeap.at(location)->priority < _minHeap.at(parentLoc)->priority) {
        swap(location, parentLoc);
        trickleUp(parentLoc);
      } else return;
    }
  }
};