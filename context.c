/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * common.c - Flickcurl common functions
 *
 * Copyright (C) 2007, David Beckett http://purl.org/net/dajobe/
 * 
 * This file is licensed under the following three licenses as alternatives:
 *   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
 *   2. GNU General Public License (GPL) V2 or any newer version
 *   3. Apache License, V2.0 or any newer version
 * 
 * You may not use this file except in compliance with at least one of
 * the above three licenses.
 * 
 * See LICENSE.html or LICENSE.txt at the top of this package for the
 * complete terms and further detail along with the license texts for
 * the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <flickcurl.h>
#include <flickcurl_internal.h>


/* This is the element name and the label - lazy */
static const char* flickcurl_context_type_element[FLICKCURL_CONTEXT_LAST+2]={
  "---",
  "set",
  "pool",
  "prevphoto",
  "nextphoto",
  NULL
};


const char*
flickcurl_get_context_type_field_label(flickcurl_context_type type)
{
  if(type > FLICKCURL_CONTEXT_NONE && type <= FLICKCURL_CONTEXT_LAST)
    return flickcurl_context_type_element[(int)type];
  return NULL;
}


flickcurl_context**
flickcurl_build_contexts(flickcurl* fc, xmlDocPtr doc)
{
  flickcurl_context** contexts=NULL;
  xmlNodePtr xnp;
  xmlNodePtr node;
  int i;
  int count=0;
  
  contexts=(flickcurl_context**)calloc(sizeof(flickcurl_context**), 3);

  /* walk children elements of root element */
  xnp = xmlDocGetRootElement(doc);
  for(i=0, node=xnp->children;
      node;
      i++, node=node->next) {
    xmlAttr* attr;
    flickcurl_context* context;
    flickcurl_context_type type=FLICKCURL_CONTEXT_NONE;
    int j;
    
#if FLICKCURL_DEBUG > 1
    fprintf(stderr, "XML node name %s XML type %d\n", node->name, node->type);
#endif
    
    if(node->type != XML_ELEMENT_NODE)
      continue;

    for(j=0; j <= FLICKCURL_CONTEXT_LAST; j++) {
      if(!strcmp((const char*)node->name, flickcurl_context_type_element[j])) {
        type=(flickcurl_context_type)j;
        break;
      }
    }
    if(type == FLICKCURL_CONTEXT_NONE)
      continue;
    
    context=(flickcurl_context*)calloc(sizeof(flickcurl_context), 1);
    context->type=type;

    for(attr=node->properties; attr; attr=attr->next) {
      const char *attr_name=(const char*)attr->name;
      char *attr_value;
      
      attr_value=(char*)malloc(strlen((const char*)attr->children->content)+1);
      strcpy(attr_value, (const char*)attr->children->content);
      
      if(!strcmp(attr_name, "id"))
        context->id=attr_value;
      else if(!strcmp(attr_name, "secret"))
        context->secret=attr_value;
      else if(!strcmp(attr_name, "server")) {
        context->server=atoi(attr_value);
        free(attr_value);
      } else if(!strcmp(attr_name, "farm")) {
        context->farm=atoi(attr_value);
        free(attr_value);
      } else if(!strcmp(attr_name, "title"))
        context->title=attr_value;
      else if(!strcmp(attr_name, "url"))
        context->url=attr_value;
      else if(!strcmp(attr_name, "thumb"))
        context->thumb=attr_value;
    } /* for attributes */
    
#if FLICKCURL_DEBUG > 1
    fprintf(stderr, "context: id %s secret %s server %d farm %d title '%s'\n  url '%s'\n  thumb '%s'\n",
            context->id, 
            (context->secret ? context->secret : "NULL"),
            context->server, context->farm,
            (context->title ? context->title : "NULL"),
            context->url,
            context->thumb
            );
#endif
    
    contexts[count++]=context;
  } /* for nodes */

  contexts[count]=NULL;

  return contexts;
}
