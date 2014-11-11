/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	if(cf != NULL && df !=NULL){
		SortedListPtr list = malloc(sizeof(struct SortedList));
		list->compareFunction = cf;
		list->destructFunction = df;
		list->length = 0;
		list->head = NULL;
		//printf("created list\n");
		return list;
	}
	return NULL;
}

void printList(SortedListPtr list){
	SortedListIteratorPtr iter = SLCreateIterator(list);
	while(iter->curr != NULL){
		printf("%d\t", *(int*)SLGetItem(iter));
		SLNextItem(iter);

	}
	printf("after: %d\t", SLGetItem(iter));
	printf("\n");
}
/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list){
    NodePtr current = list->head;
    NodePtr temp = current->next;
    
    while(current != NULL){
		list->destructFunction(current->data);
       	free(current);
        current=temp;
        if(current != NULL){
            temp=current->next;
        }
    }
    
    //printf("Memory freed!\n");
}


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj){
	//printf("inserting %d\n", *(int*)newObj);
	NodePtr newNode = malloc(sizeof(struct Node));
	newNode->data = newObj;
	newNode->next = NULL;
	newNode->previous = NULL;
	NodePtr curr, prev;
	if(list->head == NULL){ //list is empty
		//printf("empty list\n");
		list->head = newNode;
		list->length++;
		return 1;
	} else {
		//printf("non empty list\n");
		prev = NULL;
		for(curr = list->head; curr != NULL; curr = curr->next){ //scariest for loop ever
			// printf("prev:\n%p\ncurr\n%p\n", prev, curr);
			int compareValue = list->compareFunction(newNode->data, curr->data);
			if(compareValue > 0){
			//new is bigger than current
				if(prev == NULL){
					//printf("first item in list\n");
					newNode->next = curr;
					list->head = newNode;
					curr->previous = newNode;
				} else {
					newNode->next = curr;
					prev->next = newNode;
					newNode->previous = prev;
				}
				return 1;
			}
			prev = curr;

		}
		//smallest and last item
		prev->next = newNode;
		newNode->previous = prev;
		//printf("loop done\n");
		//printList(list);

	}
	free(newNode);
	return 0;
};

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *newObj){
	NodePtr curr;
	for(curr = list->head; curr->next != NULL; curr = curr->next){
		if(list->compareFunction(newObj, curr->data) == 0){
			list->destructFunction(curr->data);

			curr->data = NULL;
// 			printf("got here\n");
			if(curr->previous == NULL){
			    // What this???
				// printf("null\n");
			}
			curr->previous->next = curr->next;
				// 		printf("got here\n");

			curr->next->previous = curr->previous;
// 			printf("got done\n");

			return 1;
		}
	}
	return 0;
}

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
// 	printf("creating iter\n");
	SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));
	iter->curr = list->head;
	return iter;
}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){
	free(iter);
}


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter ){
	if(iter->curr != NULL){
// 		printf("returning data\n");
		return iter->curr->data;
	} else {
		return 0;
	}
}

/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void * SLNextItem(SortedListIteratorPtr iter){
	if(iter->curr->next == NULL){
		iter->curr = NULL;
		return NULL;
	}
	if(iter->curr->data == NULL){
		iter->curr = iter->curr->previous->next;
	} else {
		iter->curr = iter->curr->next;
	}
	return iter->curr;

}
