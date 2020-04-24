/**
 * Deadlock Demolition Lab
 * CS 241 - Spring 2019
 */
 
#include "graph.h"
#include "dictionary.h"
#include "vector.h"
#include "libdrm.h"
#include "set.h"
#include <pthread.h>
#include <stdio.h>

struct drm_t {
	pthread_mutex_t m;
};

static graph * g = NULL;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //TODO: make graph thread-safe

int isCyclicR(graph * g, void * vertex, dictionary * visited) {
	if (!dictionary_empty(visited) && dictionary_contains(visited, vertex))
		return 1;
	else {
		dictionary_set(visited, vertex, (void*) NULL);
		vector * neighbors = graph_neighbors(g, vertex);
		VECTOR_FOR_EACH(neighbors, v, {
			if(isCyclicR(g, v, visited)) {
				return 1;
			}
		});
	}
	return 0;
}

int isCyclic(graph * g, void * vertex) {
	dictionary * visited = shallow_to_shallow_dictionary_create();
	int ret = isCyclicR(g, vertex, visited);
	//fprintf(stderr, "IS IT CYCLIC: %d\n", ret);
	dictionary_destroy(visited);
	return ret;
}

drm_t *drm_init() {
    /* Your code here */
	pthread_mutex_lock(&lock);
	if (g == NULL)
		g = shallow_graph_create();
	drm_t * drm = malloc(sizeof(drm_t));
	pthread_mutex_init(&(drm->m), NULL);
	graph_add_vertex(g, drm);
	pthread_mutex_unlock(&lock);
    return drm;
}

int drm_post(drm_t *drm, pthread_t *thread_id) {
    /* Your code here */
	pthread_mutex_lock(&lock);
	//fprintf(stderr, "Thread: %p attempting to unlock drm: %p\n", thread_id, drm);
	if (!graph_contains_vertex(g, drm)) {
		//fprintf(stderr, "\tPOST: graph does not contain drm vertex\n");
		//fprintf(stderr, "\tPOST: failed to unlock drm\n");
		pthread_mutex_unlock(&lock);
		return 0;
	}
	else {
		if (!graph_contains_vertex(g, thread_id)) {
			//fprintf(stderr, "\tPOST: graph does not contain thread vertex\n");
			//fprintf(stderr, "\tPOST: failed to unlock drm\n");
			pthread_mutex_unlock(&lock);
			return 0;
		}
		if (graph_adjacent(g, drm, thread_id)) {
			pthread_mutex_unlock(&(drm->m));
			graph_remove_edge(g, drm, thread_id);
			//fprintf(stderr, "\tPOST: thread owns drm -> remove edge\n");
			//fprintf(stderr, "\tPOST: successfully attempted unlock\n");
			pthread_mutex_unlock(&lock);
			return 1;
		}
		//fprintf(stderr, "\tPOST: thread does not own drm\n");
		//fprintf(stderr, "\tPOST: failed to unlock drm\n");
		pthread_mutex_unlock(&lock);
		return 0;
	}
}

int drm_wait(drm_t *drm, pthread_t *thread_id) {
    /* Your code here */
	pthread_mutex_lock(&lock);
	//fprintf(stderr, "Thread: %p attempting to lock drm: %p\n", thread_id, drm);
	if (!graph_contains_vertex(g, drm)) {
		//fprintf(stderr, "\tWAIT: graph does not contain drm vertex -> adding vertex and edge\n");
		//fprintf(stderr, "\tWAIT: failed to unlock drm\n");
		pthread_mutex_unlock(&lock);
		return 0;
	}
	//add thread to graph if doesnt already exist
	if (!graph_contains_vertex(g, thread_id))
		graph_add_vertex(g, thread_id);
	//trying to lock a drm a thread already owns will cause a deadlock
	if (graph_adjacent(g, drm, thread_id)) {
		//fprintf(stderr, "\tWAIT: thread already owns drm -> DEADLOCK\n");
		pthread_mutex_unlock(&lock);
		return 0;
	}
	//fprintf(stderr, "\tWAIT: adding edge to graph\n");
	graph_add_edge(g, thread_id, drm);
	//adding edge will cause cycle -> deadlock
	if (isCyclic(g, thread_id)) {
		graph_remove_edge(g, thread_id, drm);
		//fprintf(stderr, "\tWAIT: cycle -> DEADLOCK\n");
		pthread_mutex_unlock(&lock);
		return 0;
	}
	pthread_mutex_unlock(&lock);
	pthread_mutex_lock(&(drm->m));
	//when prev line unblocks means that mutex can be owned
	pthread_mutex_lock(&lock);
	graph_remove_edge(g, thread_id, drm);
	graph_add_edge(g, drm, thread_id);
	pthread_mutex_unlock(&lock);
	//fprintf(stderr, "\tWAIT: successfully attempted lock\n");
    return 1;
}

void drm_destroy(drm_t *drm) {
    /* Your code here */
	pthread_mutex_lock(&lock);
	if (graph_contains_vertex(g, drm))
		graph_remove_vertex(g, drm);
	pthread_mutex_destroy(&(drm->m));
	free(drm);
	pthread_mutex_unlock(&lock);
}
