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

#include <FreeRTOS.h>
#include <list.h>

/*-----------------------------------------------------------
 * PUBLIC LIST API documented in list.h
 *----------------------------------------------------------*/

void listInit(OSList *list) {
	OSMiniListItem *end = (OSMiniListItem*)&(list->end);
	/* The list structure contains a list item which is used to mark the
	 end of the list.  To initialise the list the list end is inserted
	 as the only list entry. */
	list->index = (OSListItem*)end;
	/* The list end value is the highest possible value in the list to
	 ensure it remains at the end of the list. */
	end->value = MAX_DELAY;
	/* The list end next and previous pointers point to itself so we know
	 when the list is empty. */
	end->next = (OSListItem*)end;
	end->previous = (OSListItem*)end;
	list->size = 0;
}

void listInitItem(OSListItem *item) {
	/* Make sure the list item is not recorded as being on a list. */
	item->container = NULL;
}

void listInsertEnd(OSList *list, OSListItem *newItem) {
	/* Insert a new list item into list, but rather than sort the list,
	 makes the new list item the last item to be removed by a call to
	 listGetNextValue.  This means it has to be the item pointed to by
	 the index member. */
	OSListItem *index = (OSListItem*)list->index;
	/* Fix up links */
	newItem->next = index->next;
	newItem->previous = list->index;
	index->next->previous = (volatile OSListItem*)newItem;
	index->next = (volatile OSListItem*)newItem;
	list->index = (volatile OSListItem*)newItem;
	/* Remember which list the item is in. */
	newItem->container = (void*)list;
	(list->size)++;
}

void listInsert(OSList *list, OSListItem *newItem) {
	volatile OSListItem *iterator;
	clock_t value;
	/* Insert the new list item into the list, sorted in value order. */
	value = newItem->value;
	/* If the list already contains a list item with the same item value then
	 the new list item should be placed after it.  This ensures that TCB's which
	 are stored in ready lists (all of which have the same item value)
	 get an equal share of the CPU.  However, if the item is the same as
	 the back marker the iteration loop below will not end.  This means we need
	 to guard against this by checking the value first and modifying the
	 algorithm slightly if necessary. */
	if (value == MAX_DELAY)
		iterator = list->end.previous;
	else {
		/* *** NOTE ***********************************************************
		 If you find your application is crashing here then likely causes are:
		 1) Stack overflow
		 2) Incorrect interrupt priority assignment, especially on Cortex-M3
		 parts where numerically high priority values denote low actual
		 interrupt priories, which can seem counter intuitive.
		 3) Calling an API function from within a critical section or when
		 the scheduler is suspended.
		 4) Using a queue or semaphore before it has been initialised or
		 before the scheduler has been started (are interrupts firing
		 before taskStartScheduler() has been called?).
		 **********************************************************************/
		for (iterator = (OSListItem*)&(list->end); iterator->next->value <= value;
				iterator = iterator->next);
			/* There is nothing to do here, we are just iterating to the
			 wanted insertion position. */
	}
	/* Set links */
	newItem->next = iterator->next;
	newItem->next->previous = (volatile OSListItem*)newItem;
	newItem->previous = iterator;
	iterator->next = (volatile OSListItem*)newItem;
	/* Remember which list the item is in.  This allows fast removal of the
	 item later. */
	newItem->container = (void*)list;
	(list->size)++;
}

void listRemove(OSListItem *itemToRemove) {
	OSList *list;
	itemToRemove->next->previous = itemToRemove->previous;
	itemToRemove->previous->next = itemToRemove->next;
	/* The list item knows which list it is in.  Obtain the list from the list
	 item. */
	list = (OSList*)itemToRemove->container;
	/* Make sure the index is left pointing to a valid item. */
	if (list->index == itemToRemove)
		list->index = itemToRemove->previous;
	itemToRemove->container = NULL;
	(list->size)--;
}
