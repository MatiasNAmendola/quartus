#include "list.hpp"

using tools::list;

template<typename T> list<T>::list(  ) : first(nullptr), last(nullptr), counter(0)
{
	this->before = new item;

	this->before->value = nullptr; 
	this->before->next  = this->first; 
	this->before->prev  = nullptr;


	this->post = new item; 

	this->post->value = nullptr; 
	this->post->next  = nullptr; 
	this->post->prev  = this->last;
}

template<typename T> list<T>::~list(  )
{
	while(!this->empty())
	{
		this->pop_front();
	}

	delete this->post;

	this->first = nullptr;
	this->last  = nullptr;
	this->post  = nullptr;
}

template<typename T> typename list<T>::iterator list<T>::begin(  )
{
	static list<T>::iterator it;

	it.itm = this->first;	

	return it;
}

template<typename T> typename list<T>::iterator list<T>::end(  )
{
	static list<T>::iterator it;

	it.itm = this->post;

	return it;
}

template<typename T> typename list<T>::iterator list<T>::insert( list<T>::iterator it, T *value )
{
	list<T>::item *new_itm = new item;

	new_itm->value = value;

	new_itm->next = it.itm;

	if(it.itm)
	{
		new_itm->prev = it.itm->prev;
	}

	if(new_itm->next)
	{
		new_itm->next->prev = new_itm;
	}

	if(new_itm->prev)
	{
		new_itm->prev->next = new_itm;
	}

	if(this->first == nullptr || it.itm == this->first || it == this->begin())
	{
		this->first = new_itm;

		this->first->prev = this->before;
		this->before->next = this->first;
	}

	if(this->last == nullptr || it == this->end())
	{
		this->last = new_itm;

		this->last->next = this->post;
		this->post->prev = this->last;
	}

	this->counter++;

	static list<T>::iterator new_it;
	new_it.lst = this;
	new_it.itm = new_itm;

	return new_it;
}

template<typename T> typename list<T>::iterator list<T>::erase( list<T>::iterator it )
{
	if(it.itm == this->first)
	{
		this->first = it.itm->next;
	}

	if(it.itm == this->last)
	{
		this->last = it.itm->prev;
	}

	if(it.itm->next)
	{
		it.itm->next->prev = it.itm->prev;
	}
	
	if(it.itm->prev)
	{
		it.itm->prev->next = it.itm->next;
	}

	this->counter--;

	static list<T>::iterator new_it;
	new_it.lst = this;
	new_it.itm = it.itm->next;

	delete it.itm;

	return new_it;
}

template<typename T> void list<T>::push_front( T *value )
{
	this->insert(this->begin(), value);
}

template<typename T> void list<T>::push_back( T *value )
{
	this->insert(this->end(), value);
}

template<typename T> T *list<T>::pop_front(  )
{
	list<T>::iterator it = this->begin();

	T *value = *it;

	this->erase(it);

	return value;
}

template<typename T> T *list<T>::pop_back(  )
{
	list<T>::iterator it = this->end();

	T *value = *--it;

	this->erase(it);

	return value;
}
