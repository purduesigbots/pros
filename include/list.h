/*
 FreeRTOS V7.1.0 - Copyright (C) 2011 Real Time Engineers Ltd.

 ***************************************************************************
 *                                                                       *
 *    FreeRTOS tutorial books are available in pdf and paperback.        *
 *    Complete, revised, and edited pdf reference manuals are also       *
 *    available.                                                         *
 *                                                                       *
 *    Purchasing FreeRTOS documentation will not only help you, by       *
 *    ensuring you get running as quickly as possible and with an        *
 *    in-depth knowledge of how to use FreeRTOS, it will also help       *
 *    the FreeRTOS project to continue with its mission of providing     *
 *    professional grade, cross platform, de facto standard solutions    *
 *    for microcontrollers - completely free of charge!                  *
 *                                                                       *
 *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
 *                                                                       *
 *    Thank you for using FreeRTOS, and thank you for your support!      *
 *                                                                       *
 ***************************************************************************

 This file is part of the FreeRTOS distribution.

 FreeRTOS is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License (version 2) as published by the
 Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
 >>>NOTE<<< The modification to the GPL is included to allow you to
 distribute a combined work that includes FreeRTOS without being obliged to
 provide the source code for proprietary components outside of the FreeRTOS
 kernel.  FreeRTOS is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details. You should have received a copy of the GNU General Public
 License and the FreeRTOS license exception along with FreeRTOS; if not it
 can be viewed here: http://www.freertos.org/a00114.html and also obtained
 by writing to Richard Barry, contact details for whom are available on the
 FreeRTOS WEB site.

 http://www.FreeRTOS.org - Documentation, latest information, license and
 contact details.

 This version of FreeRTOS has been modified explicitly for use on the VEX
 Cortex by Stephen Carlson of Purdue ACM SIGBots, College Challenge team #BLRS.
 */

/*
 * This is the list implementation used by the scheduler.  While it is tailored
 * heavily for the schedulers needs, it is also available for use by
 * application code.
 *
 * OSLists can only store pointers to OSListItems.  Each OSListItem contains a
 * numeric value (xItemValue).  Most of the time the lists are sorted in
 * descending item value order.
 *
 * Lists are created already containing one list item.  The value of this
 * item is the maximum possible that can be stored, it is therefore always at
 * the end of the list and acts as a marker.  The list member pxHead always
 * points to this marker - even though it is at the tail of the list.  This
 * is because the tail contains a wrap back pointer to the true head of
 * the list.
 *
 * In addition to it's value, each list item contains a pointer to the next
 * item in the list (pxNext), a pointer to the list it is in (pxContainer)
 * and a pointer to back to the object that contains it.  These later two
 * pointers are included for efficiency of list manipulation.  There is
 * effectively a two way link between the object containing the list item and
 * the list item itself.
 *
 *
 * \page ListIntroduction List Implementation
 * \ingroup FreeRTOSIntro
 */

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Definition of the only type of object that a list can contain.
 */
typedef struct ListItem_Struct {
	clock_t value; // Value
	volatile struct ListItem_Struct *next; // Doubly linked list
	volatile struct ListItem_Struct *previous;
	void *owner; // Pointer to object (usually TCB) containing the list and items
	void *container; // Pointer to the parent list object
} OSListItem;

typedef struct {
	clock_t value;
	volatile OSListItem *next;
	volatile OSListItem *previous;
} OSMiniListItem;

/*
 * Definition of the type of queue used by the scheduler.
 */
typedef struct {
	volatile uint32_t size;
	volatile OSListItem *index;
	volatile OSMiniListItem end;
} OSList;

/*
 * Access function to set the owner of a list item.  The owner of a list item
 * is the object (usually a TCB) that contains the list item.
 */
static INLINE void listSetOwner(OSListItem *listItem, const void *owner) {
	listItem->owner = (void*)owner;
}

/*
 * Access function to set the value of the list item.  In most cases the value is
 * used to sort the list in descending order.
 */
static INLINE void listSetValue(OSListItem *listItem, const clock_t value) {
	listItem->value = value;
}

/*
 * Access function to retrieve the value of the list item.  The value can
 * represent anything - for example a the priority of a task, or the time at
 * which a task should be unblocked.
 */
static INLINE clock_t listGetValue(const OSListItem *listItem) {
	return listItem->value;
}

/*
 * Access function to retrieve the value of the list item at the head of a given
 * list.
 */
static INLINE clock_t listGetFirstValue(const OSList *list) {
	return (&(list->end))->next->value;
}

/*
 * Access function to determine if a list contains any items.  The macro will
 * only have the value true if the list is empty.
 */
static INLINE bool listIsEmpty(const OSList *list) {
	return list->size == 0;
}

/*
 * Access macro to return the number of items in the list.
 */
static INLINE uint32_t listSize(const OSList *list) {
	return list->size;
}

/*
 * Access function to obtain the owner of the next entry in a list.
 *
 * The list member index is used to walk through a list.  Calling
 * listGetNextOwner increments index to the next item in the list
 * and returns that entry's owner parameter.  Using multiple calls to this
 * function it is therefore possible to move through every item contained in
 * a list.
 *
 * The owner parameter of a list item is a pointer to the object that owns
 * the list item.  In the scheduler this is normally a task control block.
 * The owner parameter effectively creates a two way link between the list
 * item and its owner.
 *
 * @param list The list from which the next item owner is to be returned.
 */
static INLINE void* listGetNextOwner(OSList *list) {
	// Increment index to the next item and return the item
	volatile OSListItem *idx = list->index->next;
	if (idx == (volatile OSListItem*)&(list->end))
		idx = idx->next;
	list->index = idx;
	return idx->owner;
}

/*
 * Access function to obtain the owner of the first entry in a list.  Lists
 * are normally sorted in ascending item value order.
 *
 * This function returns the pxOwner member of the first item in the list.
 * The pxOwner parameter of a list item is a pointer to the object that owns
 * the list item.  In the scheduler this is normally a task control block.
 * The pxOwner parameter effectively creates a two way link between the list
 * item and its owner.
 *
 * @param list The list from which the owner of the head item is to be
 * returned.
 */
static INLINE void* listGetFirstOwner(const OSList *list) {
	return (&(list->end))->next->owner;
}

/*
 * Check to see if a list item is within a list.  The list item maintains a
 * "container" pointer that points to the list it is in.  All this macro does
 * is check to see if the container and the list match.
 *
 * @param list The list we want to know if the list item is within.
 * @param listItem The list item we want to know if is in the list.
 * @return true is the list item is in the list, otherwise false.
 */
static inline bool listContains(const OSList *list, const OSListItem *listItem) {
	return listItem->container == (void*)list;
}

/*
 * Must be called before a list is used!  This initialises all the members
 * of the list structure and inserts the OSListEnd item into the list as a
 * marker to the back of the list.
 *
 * @param list Pointer to the list being initialised.
 */
void listInit(OSList *list);

/*
 * Must be called before a list item is used.  This sets the list container to
 * null so the item does not think that it is already contained in a list.
 *
 * @param item Pointer to the list item being initialised.
 */
void listInitItem(OSListItem *item);

/*
 * Insert a list item into a list.  The item will be inserted into the list in
 * a position determined by its item value (descending item value order).
 *
 * @param list The list into which the item is to be inserted.
 *
 * @param newItem The item to that is to be placed in the list.
 */
void listInsert(OSList *list, OSListItem *newItem);

/*
 * Insert a list item into a list.  The item will be inserted in a position
 * such that it will be the last item within the list returned by multiple
 * calls to listGetNextOwner.
 *
 * The list member index is used to walk through a list.  Calling
 * listGetNextOwner increments index to the next item in the list.
 * Placing an item in a list using listInsertEnd effectively places the item
 * in the list position pointed to by index.  This means that every other
 * item within the list will be returned by listGetNextOwner before
 * the index parameter again points to the item being inserted.
 *
 * @param list The list into which the item is to be inserted.
 *
 * @param newItem The list item to be inserted into the list.
 */
void listInsertEnd(OSList *list, OSListItem *newItem);

/*
 * Remove an item from a list.  The list item has a pointer to the list that
 * it is in, so only the list item need be passed into the function.
 *
 * @param itemToRemove The item to be removed.  The item will remove itself from
 * the list pointed to by it's container parameter.
 */
void listRemove(OSListItem *itemToRemove);

#ifdef __cplusplus
}
#endif

#endif
