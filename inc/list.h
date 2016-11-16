/*
 * $Id: list.h,v 1.6 2006/10/31 06:25:28 gmm Exp $
 * Copyright (C) 2004-2005 HighPoint Technologies, Inc. All rights reserved.
 */
#ifndef _HPT_LIST_H_
#define _HPT_LIST_H_

#ifndef _LINUX_LIST_H

#ifndef HPT_INLINE
#define HPT_INLINE __inline
#endif

struct list_head {
	struct list_head *next, *prev;
};

#define INIT_LIST_HEAD(ptr) do { (ptr)->next = (ptr); (ptr)->prev = (ptr); } while (0)

static HPT_INLINE void __list_add(struct list_head * _new, struct list_head * prev, struct list_head * next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

static HPT_INLINE void list_add(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head, head->next);
}

static HPT_INLINE void list_add_tail(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head->prev, head);
}

static HPT_INLINE void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static HPT_INLINE void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static HPT_INLINE void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

static HPT_INLINE int list_empty(struct list_head *head)
{
	HPT_ASSERT(!(head->next==head && head->prev!=head));
	return head->next == head;
}

static HPT_INLINE void __list_splice(struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

static HPT_INLINE void list_splice(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

static HPT_INLINE void list_splice_init(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(HPT_UPTR)(&((type *)0)->member)))

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define get_first_item(attached, type, member) \
	((type *)((char *)((attached)->next)-(HPT_UPTR)(&((type *)0)->member)))

#endif

#endif