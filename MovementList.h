/*
	Movement List, Modified from :
		LinkedList.h - V1.1 - Generic LinkedList implementation
		Works better with FIFO, because LIFO will need to
		search the entire List to find the last one;

		For instructions, go to https://github.com/ivanseidel/LinkedList

		Created by Ivan Seidel Gomes, March, 2013.
		Released into the public domain.
*/


#ifndef MovementList_h
#define MovementList_h

template<class T>
struct ListNode
{
	T data;
	ListNode<T> *next;
	ListNode<T> *previous;
};

template <typename T>
class MovementList{

protected:
	int _size;
	ListNode<T> *first;
	ListNode<T>	*last;

	ListNode<T>* getNode(int index);

public:
	MovementList();
	~MovementList();

	/*
		Returns current size of LinkedList
	*/
	virtual int size();

	/*
		Adds a T object in the beginning of the LinkedList;
		Increment _size;
	*/
	virtual bool push(T);

	/*
		Remove last object;
	*/
	virtual T pop();

	/*
		Get last object;
	*/
	virtual T get_last();

	/*
		Get last object;
	*/
	virtual T get_first();

	/*
		Get the index'th element on the list;
		Return Element if accessible,
		else, return false;
	*/
	virtual T get(int index);

};

// Initialize LinkedList with false values
template<typename T>
MovementList<T>::MovementList()
{
	first = NULL;
	last = NULL;

	_size=0;
}

// Clear Nodes and free Memory
template<typename T>
MovementList<T>::~MovementList()
{
	ListNode<T>* tmp;
	while(first)
	{
		tmp=first;
		first=first->next;
		delete tmp;
	}
	last = NULL;
}

/*
	Actualy "logic" coding
*/

template<typename T>
int MovementList<T>::size(){
	return _size;
}

template<typename T>
bool MovementList<T>::push(T _t){

	ListNode<T> *tmp = new ListNode<T>();
	tmp->data = _t;
	tmp->next = NULL;
	tmp->previous = NULL;
	
	if(first){
		// Already have elements inserted
		first->previous = tmp;
		tmp->next = first; 
		first = tmp;
	}else{
		// First element being inserted
		first = tmp;
		first->next = tmp;
		last = tmp;
		last->previous = tmp;
	}

	_size++;
	return true;
}

template<typename T>
T MovementList<T>::pop(){
	if(_size == 0)
		return NULL;

	_size--;

	ListNode<T> *tmp = last;
	
	if (_size > 0) {
		last = last->previous;
		last->next = NULL;
	} else {
		first = NULL;
		last = NULL;
	}
	return tmp->data;
}

template<typename T>
T MovementList<T>::get(int index){
	ListNode<T> *tmp;
	if (index < _size) {
		tmp = NULL;
	}
	if (index == 0) {
		tmp = first;
	} else {
		tmp = first->next;
		for (int i = 1; i < index; i++) {
			tmp = tmp->next;
		}
	}
	return tmp->data;
}

template<typename T>
T MovementList<T>::get_first(){
	return first->data;
}

template<typename T>
T MovementList<T>::get_last(){
	return last->data;
}

#endif
