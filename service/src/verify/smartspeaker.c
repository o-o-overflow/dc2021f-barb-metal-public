#include "vm_config.h"
#include <stdint.h>
#include "mrubyc.h"
#include "smartspeaker.h"
#include "heap/o_heap.h"
#define O_HEAPSZ 4096

typedef struct song {
	int rating; // score of song
	uint16_t namelen;
	char name[0];
} song_t;

typedef struct node {
   struct node *next;
   song_t *song;
} node_t;

typedef struct ssheap {
	uint16_t nsongs;
	node_t *head;
	uint8_t heap[O_HEAPSZ];
} ssheap_t;

static void insert_song(ssheap_t *ssheap, song_t *song) {
	node_t *node = o_malloc(sizeof(node_t));
	node->song = song;
	// point to old first node
	node->next = ssheap->head;
	// first goes to the new one
	ssheap->head = node;
}

static node_t *remove_first_song(ssheap_t *ssheap) {
	//save reference to first link
	node_t *tmp = ssheap->head;

	//mark next to first link as first
	ssheap->head = ssheap->head->next;

	//return the deleted link
	return tmp;
}

static node_t *find_song(ssheap_t *ssheap, char *name_ptr, uint16_t len) {
	node_t *tmp = ssheap->head;
	if (!tmp) return NULL;
	while (memcmp(tmp->song->name, name_ptr, len)) {
		if (tmp->next == NULL) {
			return NULL;
		} else {
			tmp = tmp->next;
		}
	}
	return (node_t *)tmp;
}

static void swap_popular_song(ssheap_t *ssheap) {
	node_t *tmp = ssheap->head;
	node_t *head = ssheap->head;
	node_t *popular = tmp;
	int idx = 0;
	int max_pop_idx = 0;
	int max_pop = popular->song->rating;

	if (!tmp) return;
	while (tmp) {
		if (tmp->song->rating > max_pop) {
			max_pop = tmp->song->rating;
			max_pop_idx = idx;
			popular = tmp;
		}
		tmp = tmp->next;
		idx++;
	}
	// if we didn't find a more popular one, don't bother
	if (max_pop_idx == 0) return;
	// now swap them over
	int tmpsz = head->song->namelen;
	char *tName = o_malloc(tmpsz);
	memcpy(tName, head->song->name, tmpsz);
	// now write over the best song here
	memcpy(head->song->name, popular->song->name, popular->song->namelen);
	head->song->namelen = popular->song->namelen;
	memcpy(popular->song->name, tName, tmpsz);
	popular->song->namelen = tmpsz;
	o_free(tName);
}

//================================================================
/*! smartspeaker constructor
  $therm = smartspeaker.new	# new smartspeaker
*/
static void c_smartspeaker_queue_popular(mrbc_vm *vm, mrbc_value v[], int argc) {
	ssheap_t *ssheap = (ssheap_t*)&v->instance->data[0];
	swap_popular_song(ssheap);
	SET_NIL_RETURN();
	return;
}

//================================================================
/*! smartspeaker constructor
  $therm = smartspeaker.new	# new smartspeaker
*/
static void c_smartspeaker_vote_song(mrbc_vm *vm, mrbc_value v[], int argc) {
	if (argc != 2) {
		SET_NIL_RETURN();
		return;
	}
	ssheap_t *ssheap = (ssheap_t*)&v->instance->data[0];
	mrbc_value in_song = GET_ARG(1);
	uint16_t song_len = RSTRING_LEN(in_song);
	char *name_ptr = RSTRING_PTR(in_song);
	int vote = GET_INT_ARG(2); // value to assign to new song
	node_t *n = find_song(ssheap, name_ptr, song_len);
	if (n != NULL) {
		n->song->rating = vote;
		SET_INT_RETURN(vote);
		return;
	} else {
		SET_NIL_RETURN();
		return;
	}
}

//================================================================
/*! smartspeaker constructor
  $therm = smartspeaker.play()# new smartspeaker
*/
static void c_smartspeaker_play_song(mrbc_vm *vm, mrbc_value v[], int argc) {
  ssheap_t *ssheap = (ssheap_t*)&v->instance->data[0];
  if (!ssheap->nsongs) {
	  SET_NIL_RETURN();
	  return;
  }
  node_t *n = remove_first_song(ssheap);
  song_t *s = n->song;
  ssheap->nsongs--;
  mrbc_value ret = mrbc_string_new_cstr(vm, s->name);
  o_free(s);
  SET_RETURN(ret);
  return;
}

//================================================================
/*! smartspeaker constructor
  $therm = smartspeaker.new	# new smartspeaker
*/
static void c_smartspeaker_add_song(mrbc_vm *vm, mrbc_value v[], int argc) {
  ssheap_t *ssheap = (ssheap_t*)&v->instance->data[0];
  mrbc_value in_song = GET_ARG(1);
  uint16_t song_len = RSTRING_LEN(in_song);
  char *name_ptr = RSTRING_PTR(in_song);
  song_t *song = o_malloc(sizeof(song_t) + song_len);
  song->rating = 0;
  song->namelen = song_len;
  memcpy(song->name, name_ptr, song_len);
  insert_song(ssheap, song);
  ssheap->nsongs++;
  SET_INT_RETURN(ssheap->nsongs);
  return;
}

//================================================================
/*! smartspeaker constructor
  $therm = smartspeaker.new	# new smartspeaker
*/
static void c_smartspeaker_new(mrbc_vm *vm, mrbc_value v[], int argc) {
  *v = mrbc_instance_new(vm, v->cls, sizeof(ssheap_t)); // smart speaker
  ssheap_t *ssheap = (ssheap_t*)&v->instance->data[0];
  heap_init(ssheap->heap, sizeof(ssheap->heap));
  ssheap->nsongs = 0;
  ssheap->head = NULL;
  return;
}

//================================================================
/*! initialize
*/
void mrbc_init_class_smartspeaker(struct VM *vm) {
  // define class and methods.
  mrbc_class *smartspeaker;
  smartspeaker = mrbc_define_class(0, "Smartspeaker", mrbc_class_object);
  mrbc_define_method(0, smartspeaker, "new", c_smartspeaker_new);
  mrbc_define_method(0, smartspeaker, "add", c_smartspeaker_add_song);
  mrbc_define_method(0, smartspeaker, "play", c_smartspeaker_play_song);
  mrbc_define_method(0, smartspeaker, "vote", c_smartspeaker_vote_song);
  mrbc_define_method(0, smartspeaker, "queue_popular!", c_smartspeaker_queue_popular);
}
