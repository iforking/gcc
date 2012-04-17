/* Symbol table.
   Copyright (C) 2012 Free Software Foundation, Inc.
   Contributed by Jan Hubicka

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-inline.h"
#include "hashtab.h"
#include "ggc.h"
#include "cgraph.h"
#include "diagnostic.h"

/* Hash table used to convert declarations into nodes.  */
static GTY((param_is (union symtab_node_def))) htab_t symtab_hash;
/* Hash table used to convert assembler names into nodes.  */
static GTY((param_is (union symtab_node_def))) htab_t assembler_name_hash;

/* Linked list of symbol table nodes.  */
symtab_node symtab_nodes;

/* The order index of the next symtab node to be created.  This is
   used so that we can sort the cgraph nodes in order by when we saw
   them, to support -fno-toplevel-reorder.  */
int symtab_order;

/* Returns a hash code for P.  */

static hashval_t
hash_node (const void *p)
{
  const_symtab_node n = (const_symtab_node ) p;
  return (hashval_t) DECL_UID (n->symbol.decl);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_node (const void *p1, const void *p2)
{
  const_symtab_node n1 = (const_symtab_node) p1;
  const_symtab_node n2 = (const_symtab_node) p2;
  return DECL_UID (n1->symbol.decl) == DECL_UID (n2->symbol.decl);
}

/* Returns a hash code for P.  */

static hashval_t
hash_node_by_assembler_name (const void *p)
{
  const_symtab_node n = (const_symtab_node) p;
  return (hashval_t) decl_assembler_name_hash (DECL_ASSEMBLER_NAME (n->symbol.decl));
}

/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_assembler_name (const void *p1, const void *p2)
{
  const_symtab_node n1 = (const_symtab_node) p1;
  const_tree name = (const_tree)p2;
  return (decl_assembler_name_equal (n1->symbol.decl, name));
}

/* Insert NODE to assembler name hash.  */

static void
insert_to_assembler_name_hash (symtab_node node)
{
  gcc_checking_assert (!node->symbol.previous_sharing_asm_name
		       && !node->symbol.next_sharing_asm_name);
  if (assembler_name_hash)
    {
      void **aslot;
      tree name = DECL_ASSEMBLER_NAME (node->symbol.decl);

      aslot = htab_find_slot_with_hash (assembler_name_hash, name,
					decl_assembler_name_hash (name),
					INSERT);
      gcc_assert (*aslot != node);
      node->symbol.next_sharing_asm_name = (symtab_node)*aslot;
      if (*aslot != NULL)
	((symtab_node)*aslot)->symbol.previous_sharing_asm_name = node;
      *aslot = node;
    }

}

/* Remove NODE from assembler name hash.  */

static void
unlink_from_assembler_name_hash (symtab_node node)
{
  if (assembler_name_hash)
    {
      if (node->symbol.next_sharing_asm_name)
	node->symbol.next_sharing_asm_name->symbol.previous_sharing_asm_name
	  = node->symbol.previous_sharing_asm_name;
      if (node->symbol.previous_sharing_asm_name)
	{
	  node->symbol.previous_sharing_asm_name->symbol.next_sharing_asm_name
	    = node->symbol.next_sharing_asm_name;
	}
      else
	{
	  tree name = DECL_ASSEMBLER_NAME (node->symbol.decl);
          void **slot;
	  slot = htab_find_slot_with_hash (assembler_name_hash, name,
					   decl_assembler_name_hash (name),
					   NO_INSERT);
	  gcc_assert (*slot == node);
	  if (!node->symbol.next_sharing_asm_name)
	    htab_clear_slot (assembler_name_hash, slot);
	  else
	    *slot = node->symbol.next_sharing_asm_name;
	}
    }
}


/* Add node into symbol table.  This function is not used directly, but via
   cgraph/varpool node creation routines.  */

void
symtab_register_node (symtab_node node)
{
  struct symtab_node_base key;
  symtab_node *slot;

  node->symbol.next = symtab_nodes;
  node->symbol.previous = NULL;
  if (symtab_nodes)
    symtab_nodes->symbol.previous = node;
  symtab_nodes = node;

  if (!symtab_hash)
    symtab_hash = htab_create_ggc (10, hash_node, eq_node, NULL);
  key.decl = node->symbol.decl;
  slot = (symtab_node *) htab_find_slot (symtab_hash, &key, INSERT);
  if (*slot == NULL)
    *slot = node;

  insert_to_assembler_name_hash (node);

  node->symbol.order = symtab_order++;

  ipa_empty_ref_list (&node->symbol.ref_list);
}

/* Make NODE to be the one symtab hash is pointing to.  Used when reshaping tree
   of inline clones.  */

void
symtab_insert_node_to_hashtable (symtab_node node)
{
  struct symtab_node_base key;
  symtab_node *slot;

  if (!symtab_hash)
    symtab_hash = htab_create_ggc (10, hash_node, eq_node, NULL);
  key.decl = node->symbol.decl;
  slot = (symtab_node *) htab_find_slot (symtab_hash, &key, INSERT);
  *slot = node;
}

/* Remove node from symbol table.  This function is not used directly, but via
   cgraph/varpool node removal routines.  */

void
symtab_unregister_node (symtab_node node)
{
  void **slot;
  ipa_remove_all_references (&node->symbol.ref_list);
  ipa_remove_all_refering (&node->symbol.ref_list);

  if (node->symbol.same_comdat_group)
    {
      symtab_node prev;
      for (prev = node->symbol.same_comdat_group;
	   prev->symbol.same_comdat_group != node;
	   prev = prev->symbol.same_comdat_group)
	;
      if (node->symbol.same_comdat_group == prev)
	prev->symbol.same_comdat_group = NULL;
      else
	prev->symbol.same_comdat_group = node->symbol.same_comdat_group;
      node->symbol.same_comdat_group = NULL;
    }

  if (node->symbol.previous)
    node->symbol.previous->symbol.next = node->symbol.next;
  else
    symtab_nodes = node->symbol.next;
  if (node->symbol.next)
    node->symbol.next->symbol.previous = node->symbol.previous;
  node->symbol.next = NULL;
  node->symbol.previous = NULL;

  slot = htab_find_slot (symtab_hash, node, NO_INSERT);
  if (*slot == node)
    {
      symtab_node replacement_node = NULL;
      if (symtab_function_p (node))
	replacement_node = (symtab_node)cgraph_find_replacement_node (cgraph (node));
      if (!replacement_node)
	htab_clear_slot (symtab_hash, slot);
      else
	*slot = replacement_node;
    }
  unlink_from_assembler_name_hash (node);
}

/* Return symbol table node associated with DECL, if any,
   and NULL otherwise.  */

symtab_node
symtab_get_node (const_tree decl)
{
  symtab_node *slot;
  struct symtab_node_base key;

  gcc_checking_assert (TREE_CODE (decl) == FUNCTION_DECL
		       || (TREE_CODE (decl) == VAR_DECL
			   && (TREE_STATIC (decl) || DECL_EXTERNAL (decl)
			       || in_lto_p)));

  if (!symtab_hash)
    return NULL;

  key.decl = CONST_CAST2 (tree, const_tree, decl);

  slot = (symtab_node *) htab_find_slot (symtab_hash, &key,
					 NO_INSERT);

  if (slot)
    return *slot;
  return NULL;
}

/* Remove symtab NODE from the symbol table.  */

void
symtab_remove_node (symtab_node node)
{
  if (symtab_function_p (node))
    cgraph_remove_node (cgraph (node));
  else if (symtab_variable_p (node))
    varpool_remove_node (varpool (node));
}

/* Return the cgraph node that has ASMNAME for its DECL_ASSEMBLER_NAME.
   Return NULL if there's no such node.  */

symtab_node
symtab_node_for_asm (const_tree asmname)
{
  symtab_node node;
  void **slot;

  if (!assembler_name_hash)
    {
      assembler_name_hash =
	htab_create_ggc (10, hash_node_by_assembler_name, eq_assembler_name,
			 NULL);
      FOR_EACH_SYMBOL (node)
	insert_to_assembler_name_hash (node);
    }

  slot = htab_find_slot_with_hash (assembler_name_hash, asmname,
				   decl_assembler_name_hash (asmname),
				   NO_INSERT);

  if (slot)
    {
      node = (symtab_node) *slot;
      return node;
    }
  return NULL;
}

/* Set the DECL_ASSEMBLER_NAME and update symtab hashtables.  */

void
change_decl_assembler_name (tree decl, tree name)
{
  symtab_node node = NULL;

  /* We can have user ASM names on things, like global register variables, that
     are not in the symbol table.  */
  if ((TREE_CODE (decl) == VAR_DECL
       && (TREE_STATIC (decl) || DECL_EXTERNAL (decl)))
      || TREE_CODE (decl) == FUNCTION_DECL)
    node = symtab_get_node (decl);
  if (!DECL_ASSEMBLER_NAME_SET_P (decl))
    {
      SET_DECL_ASSEMBLER_NAME (decl, name);
      if (node)
	insert_to_assembler_name_hash (node);
    }
  else
    {
      if (name == DECL_ASSEMBLER_NAME (decl))
	return;

      if (node)
	unlink_from_assembler_name_hash (node);
      if (TREE_SYMBOL_REFERENCED (DECL_ASSEMBLER_NAME (decl))
	  && DECL_RTL_SET_P (decl))
	warning (0, "%D renamed after being referenced in assembly", decl);

      SET_DECL_ASSEMBLER_NAME (decl, name);
      if (node)
	insert_to_assembler_name_hash (node);
    }
}

#include "gt-symtab.h"
