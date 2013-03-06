/**
 * @file   cs_rbtree.h
 * @author Zhang Guanxing <zhangguanxing01@baidu.com>
 * @date   Mon Feb 18 14:19:25 2013
 * 
 * @brief  implements of red-black tree based on 
 *         Introduction to Algorithm
 * 
 * 
 */
#ifndef CS_RBTREE_H_
#define CS_RBTREE_H_

#include "caesar.h"

typedef struct cs_rbtree_s      cs_rbtree_t;
typedef struct cs_rbtree_node_s cs_rbtree_node_t;
typedef long   cs_rbtree_key_t;

#define CS_RBTREE_BLACK 0x01
#define CS_RBTREE_RED   0x00 

struct cs_rbtree_s {
  cs_rbtree_node_t *root;
  
  int container_cmp (cs_rbtree_node_t *, cs_rbtree_node_t *);
  int count;
#ifdef CS_DEBUG
  int depth;
#endif
  cs_rbtree_node_t sentinel;
};

struct cs_rbtree_node_s {
  cs_rbtree_node_t    *left, *right, *parent;
  
  cs_rbtree_key_t     key; 
};

/** 
 * 
 * 
 * 
 * @return 
 */
cs_rbtree_t *cs_rbtree_create();

/** 
 * 
 * 
 * @param tree 
 * @param key 
 * @param data 
 * 
 * @return 
 */
cs_bool     cs_rbtree_insert(cs_rbtree_t *tree, cs_rbtree_node_t *node);

/** 
 * 
 * 
 * @param tree 
 * @param key 
 * 
 * @return 
 */
cs_bool     cs_rbtree_delete(cs_rbtree_t *tree, cs_rbtree_node_t *node);

/** 
 * 
 * 
 * @param tree 
 * @param key 
 * 
 * @return 
 */
cs_bool     cs_rbtree_delete_with_key(cs_rbtree_t *tree, cs_rbtree_key_t   key);
/** 
 * 
 * 
 * @param tree 
 * @param key 
 * 
 * @return 
 */
void*       cs_rbtree_search(cs_rbtree_t *tree, cs_rbtree_key_t key);
/** 
 * 
 * 
 * @param tree 
 * 
 * @return 
 */
cs_rbtree_node_t*       cs_rbtree_maximum(cs_rbtree_t *tree);
/** 
 * 
 * 
 * @param tree 
 * 
 * @return 
 */
cs_rbtree_node_t*       cs_rbtree_minimum(cs_rbtree_t *tree);
/** 
 * 
 * 
 * @param tree 
 * 
 * @return 
 */
cs_bool     cs_rbtree_free(cs_rbtree_t *tree);
       
#endif
