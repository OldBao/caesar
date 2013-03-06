#include "cs_rbtree.h"

static void left_rotate(cs_rbtree_t *tree, cs_rbtree_node_t *viot);
static void right_rotate(cs_rbtree_t *tree, cs_rbtree_node_t *viot);

void 
cs_rbtree_init(cs_rbtree_t *tree){
  cs_rbtree_t* tree;
  tree = cs_malloc(sizeof(cs_rbtree_t));
  if (NULL == tree) {
    CS_ERROR("malloc error");
    return NULL;
  }

  tree->root   = NULL;
  tree->count  = 0;
  
#ifdef CS_DEBUG
  tree->depth  = 0;
#endif
}

cs_bool
cs_rbtree_insert(cs_rbtree_t *tree, cs_rbtree_node_t *node) {
  assert(tree);
  assert(key);
  assert(key_len > 0);
  assert(data);
  
  cs_rbtree_node_t *p, *parent;
  if (tree->root == NULL) {
    tree->root = node;
    return TRUE;
  }
  p = tree->root;

  while(p) {
    parent = p; //record parent position

    if (p->key > node->key) {
      p = p->left;
    } else if (p->key < node->key) {
      p = p->right;
    } else {
      if (tree->container_cmp(p, node) != 0) {
	p = p->left;
      } else {
	break;
      }
    }
  }

  //node is exists
  if (p) {
    return FALSE;
  } else {
    
  }
}
