#ifndef LIST_HPP
#define LIST_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

namespace tools
{
	template<typename T> class list
	{
		public:
			struct item
			{
				T		*value;				

				item 		*prev;
				item		*next;
			};

			struct iterator
			{
				item		*itm;

				list		*lst;

				inline iterator &operator ++ () { this->itm = itm->next; return *this; }
				inline iterator &operator -- () { this->itm = itm->prev; return *this; }

				inline iterator operator ++ (int) { iterator tmp = *this; if(this->itm) this->itm = this->itm->next; return tmp; }
				inline iterator operator -- (int) { iterator tmp = *this; if(this->itm) this->itm = this->itm->prev; return tmp; }

				inline bool operator == ( const iterator &it ) { if(this->lst != it.lst) return false; if(this->itm != it.itm) return false; return true; }

				inline bool operator != ( const iterator &it ) { return !(*this == it); }

				inline T *operator * () { if(this->itm) { return this->itm->value; } else { return nullptr; } }
				inline T *operator -> () { if(this->itm) { return this->itm->value; } else { return nullptr; }  }

				iterator(  ) { this->lst = nullptr; this->itm = nullptr; }
				iterator( list *lst, item *imt ) { this->lst = lst; this->itm = itm; }
			};

			list(  );

			~list(  );

			inline T *front(  ){ return this->first->value; }
			inline T *back(  ){ return this->last->value; }

			iterator begin(  );
			iterator end(  );

			iterator insert( iterator it, T *value );
			iterator erase( iterator it );

			void push_front( T *value );
			void push_back( T *value );

			T *pop_front(  );
			T *pop_back(  );

			inline size_t size(  ) { return this->counter; }

			inline bool empty(  ){ return (this->counter == 0); }

		private:
			item			*first;
			item			*last;
			item			*before;
			item			*post;

			size_t			counter;
	};
}

#include "list.tpp"

#endif
