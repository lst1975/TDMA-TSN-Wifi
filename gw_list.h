/******************************************************************************
 *  Author:    980680431@qq.com
 *  Liscence:  GPLV3
 *  Copyright: 980680431@qq.com 
 ******************************************************************************/
#ifndef  __GW_LIST_H__
#define  __GW_LIST_H__

#define WRITE_ONCE(a,v)        ((a) = (v))
#define READ_ONCE(v)           (v)
#define smp_store_release(a,v) (*(a) = (v))
#define smp_load_acquire(v)    *(v)
#define rcu_dereference(v)     (v)

#define LIST_POISON1 ((list_head_s *)(uintptr_t)0xabababababababab)
#define LIST_POISON2 ((list_head_s *)(uintptr_t)0xcdcdcdcdcdcdcdcd)

struct list_head {
  struct list_head *next;
  struct list_head *prev;
};
typedef struct list_head list_head_s;

/*
 * Circular doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
  list_head_s name = LIST_HEAD_INIT(name)

static inline void __list_poison(list_head_s *entry)
{
  entry->next = LIST_POISON1;
  entry->prev = LIST_POISON2;
}

/**
 * INIT_LIST_HEAD - Initialize a list_head structure
 * @list: list_head structure to be initialized.
 *
 * Initializes the list_head to point to itself.  If it is a list header,
 * the result is an empty list.
 */
static inline void INIT_LIST_HEAD(list_head_s *list)
{
  WRITE_ONCE(list->next, list);
  WRITE_ONCE(list->prev, list);
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(list_head_s *new,
            list_head_s *prev,
            list_head_s *next)
{
  next->prev = new;
  new->next = next;
  new->prev = prev;
  WRITE_ONCE(prev->next, new);
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(list_head_s *new, list_head_s *head)
{
  __list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(list_head_s *new, list_head_s *head)
{
  __list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(list_head_s * prev, list_head_s * next)
{
  next->prev = prev;
  WRITE_ONCE(prev->next, next);
}

/*
 * Delete a list entry and clear the 'prev' pointer.
 *
 * This is a special-purpose list clearing method used in the networking code
 * for lists allocated as per-cpu, where we don't want to incur the extra
 * WRITE_ONCE() overhead of a regular list_del_init(). The code that uses this
 * needs to check the node 'prev' pointer instead of calling list_empty().
 */
static inline void __list_del_clearprev(list_head_s *entry)
{
  __list_del(entry->prev, entry->next);
  entry->prev = NULL;
}

static inline void __list_del_entry(list_head_s *entry)
{
  __list_del(entry->prev, entry->next);
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void list_del(list_head_s *entry)
{
  __list_del_entry(entry);
  __list_poison(entry);
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void list_replace(list_head_s *old,
        list_head_s *new)
{
  new->next = old->next;
  new->next->prev = new;
  new->prev = old->prev;
  new->prev->next = new;
}

/**
 * list_replace_init - replace old entry by new one and initialize the old one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void list_replace_init(list_head_s *old,
             list_head_s *new)
{
  list_replace(old, new);
  INIT_LIST_HEAD(old);
}

/**
 * list_swap - replace entry1 with entry2 and re-add entry1 at entry2's position
 * @entry1: the location to place entry2
 * @entry2: the location to place entry1
 */
static inline void list_swap(list_head_s *entry1,
           list_head_s *entry2)
{
  list_head_s *pos = entry2->prev;

  list_del(entry2);
  list_replace(entry1, entry2);
  if (pos == entry1)
    pos = entry2;
  list_add(entry1, pos);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(list_head_s *entry)
{
  __list_del_entry(entry);
  INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(list_head_s *list, list_head_s *head)
{
  __list_del_entry(list);
  list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(list_head_s *list,
          list_head_s *head)
{
  __list_del_entry(list);
  list_add_tail(list, head);
}

/**
 * list_bulk_move_tail - move a subsection of a list to its tail
 * @head: the head that will follow our entry
 * @first: first entry to move
 * @last: last entry to move, can be the same as first
 *
 * Move all entries between @first and including @last before @head.
 * All three entries must belong to the same linked list.
 */
static inline void list_bulk_move_tail(list_head_s *head,
               list_head_s *first,
               list_head_s *last)
{
  first->prev->next = last->next;
  last->next->prev = first->prev;

  head->prev->next = first;
  first->prev = head->prev;

  last->next = head;
  head->prev = last;
}

/**
 * list_is_first -- tests whether @list is the first entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_first(const list_head_s *list, const list_head_s *head)
{
  return list->prev == head;
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_last(const list_head_s *list, const list_head_s *head)
{
  return list->next == head;
}

/**
 * list_is_head - tests whether @list is the list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_head(const list_head_s *list, const list_head_s *head)
{
  return list == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const list_head_s *head)
{
  return READ_ONCE(head->next) == head;
}

/**
 * list_del_init_careful - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 *
 * This is the same as list_del_init(), except designed to be used
 * together with list_empty_careful() in a way to guarantee ordering
 * of other memory operations.
 *
 * Any memory operations done before a list_del_init_careful() are
 * guaranteed to be visible after a list_empty_careful() test.
 */
static inline void list_del_init_careful(list_head_s *entry)
{
  __list_del_entry(entry);
  WRITE_ONCE(entry->prev, entry);
  smp_store_release(&entry->next, entry);
}

/**
 * list_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
static inline int list_empty_careful(const list_head_s *head)
{
  list_head_s *next = smp_load_acquire(&head->next);
  return list_is_head(next, head) && (next == READ_ONCE(head->prev));
}

/**
 * list_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void list_rotate_left(list_head_s *head)
{
  list_head_s *first;

  if (!list_empty(head)) {
    first = head->next;
    list_move_tail(first, head);
  }
}

/**
 * list_rotate_to_front() - Rotate list to specific item.
 * @list: The desired new front of the list.
 * @head: The head of the list.
 *
 * Rotates list so that @list becomes the new front of the list.
 */
static inline void list_rotate_to_front(list_head_s *list,
          list_head_s *head)
{
  /*
   * Deletes the list head from the list denoted by @head and
   * places it as the tail of @list, this effectively rotates the
   * list so that @list is at the front.
   */
  list_move_tail(head, list);
}

/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int list_is_singular(const list_head_s *head)
{
  return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(list_head_s *list,
    list_head_s *head, list_head_s *entry)
{
  list_head_s *new_first = entry->next;
  list->next = head->next;
  list->next->prev = list;
  list->prev = entry;
  entry->next = list;
  head->next = new_first;
  new_first->prev = head;
}

/**
 * list_cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *  and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void list_cut_position(list_head_s *list,
    list_head_s *head, list_head_s *entry)
{
  if (list_empty(head))
    return;
  if (list_is_singular(head) && !list_is_head(entry, head) && (entry != head->next))
    return;
  if (list_is_head(entry, head))
    INIT_LIST_HEAD(list);
  else
    __list_cut_position(list, head, entry);
}

/**
 * list_cut_before - cut a list into two, before given entry
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *
 * This helper moves the initial part of @head, up to but
 * excluding @entry, from @head to @list.  You should pass
 * in @entry an element you know is on @head.  @list should
 * be an empty list or a list you do not care about losing
 * its data.
 * If @entry == @head, all entries on @head are moved to
 * @list.
 */
static inline void list_cut_before(list_head_s *list,
           list_head_s *head,
           list_head_s *entry)
{
  if (head->next == entry) {
    INIT_LIST_HEAD(list);
    return;
  }
  list->next = head->next;
  list->next->prev = list;
  list->prev = entry->prev;
  list->prev->next = list;
  head->next = entry;
  entry->prev = head;
}

static inline void __list_splice(const list_head_s *list,
         list_head_s *prev,
         list_head_s *next)
{
  list_head_s *first = list->next;
  list_head_s *last = list->prev;

  first->prev = prev;
  prev->next = first;

  last->next = next;
  next->prev = last;
}

/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(const list_head_s *list,
        list_head_s *head)
{
  if (!list_empty(list))
    __list_splice(list, head, head->next);
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice_tail(list_head_s *list,
        list_head_s *head)
{
  if (!list_empty(list))
    __list_splice(list, head->prev, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(list_head_s *list,
            list_head_s *head)
{
  if (!list_empty(list)) {
    __list_splice(list, head, head->next);
    INIT_LIST_HEAD(list);
  }
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void list_splice_tail_init(list_head_s *list,
           list_head_s *head)
{
  if (!list_empty(list)) {
    __list_splice(list, head->prev, head);
    INIT_LIST_HEAD(list);
  }
}

/**
 * list_entry - get the struct for this entry
 * @ptr:  the &list_head_s pointer.
 * @type:  the type of the struct this is embedded in.
 * @member:  the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
  tsn_container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:  the list head to take the element from.
 * @type:  the type of the struct this is embedded in.
 * @member:  the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
  list_entry((ptr)->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * @ptr:  the list head to take the element from.
 * @type:  the type of the struct this is embedded in.
 * @member:  the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_last_entry(ptr, type, member) \
  list_entry((ptr)->prev, type, member)

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:  the list head to take the element from.
 * @type:  the type of the struct this is embedded in.
 * @member:  the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) \
  (list_empty(ptr) ? list_entry((ptr)->next, type, member) : NULL)

/**
 * list_next_entry - get the next element in list
 * @pos:  the type * to cursor
 * @member:  the name of the list_head within the struct.
 */
#define list_next_entry(pos, type, member) \
  list_entry((pos)->member.next, type, member)

/**
 * list_next_entry_circular - get the next element in list
 * @pos:  the type * to cursor.
 * @head:  the list head to take the element from.
 * @member:  the name of the list_head within the struct.
 *
 * Wraparound if pos is the last element (return the first element).
 * Note, that list is expected to be not empty.
 */
#define list_next_entry_circular(pos, type, head, member) \
  (list_is_last(&(pos)->member, head) ? \
  list_first_entry(head, type, member) : list_next_entry(pos, member))

/**
 * list_prev_entry - get the prev element in list
 * @pos:  the type * to cursor
 * @member:  the name of the list_head within the struct.
 */
#define list_prev_entry(pos, type, member) \
  list_entry((pos)->member.prev, type, member)

/**
 * list_prev_entry_circular - get the prev element in list
 * @pos:  the type * to cursor.
 * @head:  the list head to take the element from.
 * @member:  the name of the list_head within the struct.
 *
 * Wraparound if pos is the first element (return the last element).
 * Note, that list is expected to be not empty.
 */
#define list_prev_entry_circular(pos, type, head, member) \
  (list_is_first(&(pos)->member, head) ? \
  list_last_entry(head, type, member) : list_prev_entry(pos, member))

/**
 * list_for_each  -  iterate over a list
 * @pos:  the &list_head_s to use as a loop cursor.
 * @head:  the head for your list.
 */
#define list_for_each(pos, head) \
  for (pos = (head)->next; !list_is_head(pos, (head)); pos = pos->next)

/**
 * list_for_each_rcu - Iterate over a list in an RCU-safe fashion
 * @pos:  the &list_head_s to use as a loop cursor.
 * @head:  the head for your list.
 */
#define list_for_each_rcu(pos, head)      \
  for (pos = rcu_dereference((head)->next); \
       !list_is_head(pos, (head)); \
       pos = rcu_dereference(pos->next))

/**
 * list_for_each_continue - continue iteration over a list
 * @pos:  the &list_head_s to use as a loop cursor.
 * @head:  the head for your list.
 *
 * Continue to iterate over a list, continuing after the current position.
 */
#define list_for_each_continue(pos, head) \
  for (pos = pos->next; !list_is_head(pos, (head)); pos = pos->next)

/**
 * list_for_each_prev  -  iterate over a list backwards
 * @pos:  the &list_head_s to use as a loop cursor.
 * @head:  the head for your list.
 */
#define list_for_each_prev(pos, head) \
  for (pos = (head)->prev; !list_is_head(pos, (head)); pos = pos->prev)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:  the &list_head_s to use as a loop cursor.
 * @n:    another &list_head_s to use as temporary storage
 * @head:  the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
  for (pos = (head)->next, n = pos->next; \
       !list_is_head(pos, (head)); \
       pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:  the &list_head_s to use as a loop cursor.
 * @n:    another &list_head_s to use as temporary storage
 * @head:  the head for your list.
 */
#define list_for_each_prev_safe(pos, n, head) \
  for (pos = (head)->prev, n = pos->prev; \
       !list_is_head(pos, (head)); \
       pos = n, n = pos->prev)

/**
 * list_entry_is_head - test if the entry points to the head of the list
 * @pos:  the type * to cursor
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 */
#define list_entry_is_head(pos, head, member)        \
  (&pos->member == (head))

/**
 * list_for_each_entry  -  iterate over list of given type
 * @pos:  the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, type, head, member)        \
  for (pos = list_first_entry(head, type, member);  \
       !list_entry_is_head(pos, head, member);      \
       pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:  the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 */
#define list_for_each_entry_reverse(pos, type, head, member)      \
  for (pos = list_last_entry(head, type, member);    \
       !list_entry_is_head(pos, head, member);       \
       pos = list_prev_entry(pos, member))

/**
 * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
 * @pos:  the type * to use as a start point
 * @head:  the head of the list
 * @member:  the name of the list_head within the struct.
 *
 * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
 */
#define list_prepare_entry(pos, type, head, member) \
  ((pos) ? : list_entry(head, type, member))

/**
 * list_for_each_entry_continue - continue iteration over list of given type
 * @pos:  the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define list_for_each_entry_continue(pos, head, member)     \
  for (pos = list_next_entry(pos, member);      \
       !list_entry_is_head(pos, head, member);      \
       pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:  the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define list_for_each_entry_continue_reverse(pos, head, member)    \
  for (pos = list_prev_entry(pos, member);      \
       !list_entry_is_head(pos, head, member);      \
       pos = list_prev_entry(pos, member))

/**
 * list_for_each_entry_from - iterate over list of given type from the current point
 * @pos:  the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define list_for_each_entry_from(pos, head, member)       \
  for (; !list_entry_is_head(pos, head, member);      \
       pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_from_reverse - iterate backwards over list of given type
 *                                    from the current point
 * @pos:  the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, continuing from current position.
 */
#define list_for_each_entry_from_reverse(pos, head, member)    \
  for (; !list_entry_is_head(pos, head, member);      \
       pos = list_prev_entry(pos, member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:  the type * to use as a loop cursor.
 * @n:    another type * to use as temporary storage
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, type, n, head, member)      \
  for (pos = list_first_entry(head, type, member),  \
    n = list_next_entry(pos, member);      \
       !list_entry_is_head(pos, head, member);       \
       pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:  the type * to use as a loop cursor.
 * @n:    another type * to use as temporary storage
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define list_for_each_entry_safe_continue(pos, n, head, member)     \
  for (pos = list_next_entry(pos, member),         \
    n = list_next_entry(pos, member);        \
       !list_entry_is_head(pos, head, member);        \
       pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:  the type * to use as a loop cursor.
 * @n:    another type * to use as temporary storage
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define list_for_each_entry_safe_from(pos, n, head, member)       \
  for (n = list_next_entry(pos, member);          \
       !list_entry_is_head(pos, head, member);        \
       pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_entry_safe_reverse - iterate backwards over list safe against removal
 * @pos:  the type * to use as a loop cursor.
 * @n:    another type * to use as temporary storage
 * @head:  the head for your list.
 * @member:  the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define list_for_each_entry_safe_reverse(pos, type, n, head, member)    \
  for (pos = list_last_entry(head, type, member),    \
    n = list_prev_entry(pos, member);      \
       !list_entry_is_head(pos, head, member);       \
       pos = n, n = list_prev_entry(n, member))

/**
 * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
 * @pos:  the loop cursor used in the list_for_each_entry_safe loop
 * @n:    temporary storage used in list_for_each_entry_safe
 * @member:  the name of the list_head within the struct.
 *
 * list_safe_reset_next is not safe to use in general if the list may be
 * modified concurrently (eg. the lock is dropped in the loop body). An
 * exception to this is if the cursor element (pos) is pinned in the list,
 * and list_safe_reset_next is called after re-taking the lock and before
 * completing the current iteration of the loop body.
 */
#define list_safe_reset_next(pos, n, member)        \
  n = list_next_entry(pos, member)

#endif  /* !_GW_LIST_H_ */

