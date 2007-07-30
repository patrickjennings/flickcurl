/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * flickcurl.h - Flickcurl API
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


#ifndef FLICKCURL_H
#define FLICKCURL_H


#ifdef __cplusplus
extern "C" {
#endif

/* needed for xmlDocPtr */
#include <libxml/tree.h>


/* Use gcc 3.1+ feature to allow marking of deprecated API calls.
 * This gives a warning during compiling.
 */
#if ( __GNUC__ == 3 && __GNUC_MINOR__ > 0 ) || __GNUC__ > 3
#ifdef __APPLE_CC__
/* OSX gcc cpp-precomp is broken */
#define FLICKCURL_DEPRECATED
#else
#define FLICKCURL_DEPRECATED __attribute__((deprecated))
#endif
#else
#define FLICKCURL_DEPRECATED
#endif

/*
 * Field data types
 */
typedef enum {
  VALUE_TYPE_NONE, /* empty field */
  VALUE_TYPE_PHOTO_ID, /* internal */
  VALUE_TYPE_PHOTO_URI, /* internal */
  VALUE_TYPE_UNIXTIME,
  VALUE_TYPE_BOOLEAN,
  VALUE_TYPE_DATETIME,
  VALUE_TYPE_FLOAT,
  VALUE_TYPE_INTEGER,
  VALUE_TYPE_STRING,
  VALUE_TYPE_URI,
  VALUE_TYPE_PERSON_ID, /* internal */
  VALUE_TYPE_LAST = VALUE_TYPE_PERSON_ID
} flickcurl_field_value_type;
  

/*
 * Fields of a flickcurl_photo*
 */
typedef enum {
  PHOTO_FIELD_none,
  PHOTO_FIELD_dateuploaded,
  PHOTO_FIELD_farm,
  PHOTO_FIELD_isfavorite,
  PHOTO_FIELD_license,
  PHOTO_FIELD_originalformat,
  PHOTO_FIELD_rotation,
  PHOTO_FIELD_server,
  PHOTO_FIELD_dates_lastupdate,
  PHOTO_FIELD_dates_posted,
  PHOTO_FIELD_dates_taken,
  PHOTO_FIELD_dates_takengranularity,
  PHOTO_FIELD_description,
  PHOTO_FIELD_editability_canaddmeta,
  PHOTO_FIELD_editability_cancomment,
  PHOTO_FIELD_geoperms_iscontact,
  PHOTO_FIELD_geoperms_isfamily,
  PHOTO_FIELD_geoperms_isfriend,
  PHOTO_FIELD_geoperms_ispublic,
  PHOTO_FIELD_location_accuracy,
  PHOTO_FIELD_location_latitude,
  PHOTO_FIELD_location_longitude,
  PHOTO_FIELD_owner_location,
  PHOTO_FIELD_owner_nsid,
  PHOTO_FIELD_owner_realname,
  PHOTO_FIELD_owner_username,
  PHOTO_FIELD_title,
  PHOTO_FIELD_visibility_isfamily,
  PHOTO_FIELD_visibility_isfriend,
  PHOTO_FIELD_visibility_ispublic,
  PHOTO_FIELD_secret,
  PHOTO_FIELD_originalsecret,
  PHOTO_FIELD_location_neighborhood,
  PHOTO_FIELD_location_locality,
  PHOTO_FIELD_location_region,
  PHOTO_FIELD_location_country,
  PHOTO_FIELD_LAST = PHOTO_FIELD_location_country,
} flickcurl_photo_field_type;


/* The main object type */
typedef struct flickcurl_s flickcurl;
  

/* Forward structure references */
struct flickcurl_s;
struct flickcurl_photo_s;


/**
 * flickcurl_arg:
 * @name: Argument name
 * @optional: boolean flag (non-0 true) if argument is optional
 * @description: description of argument (HTML)
 *
 * An API method argument.
 */
typedef struct flickcurl_arg_s {
  char* name;
  int optional;
  char *description;
} flickcurl_arg;


/**
 * flickcurl_method: 
 * @name: Method name
 * @needslogin: boolean flag (non-0 true) if method requires login
 * @description: description of method
 * @response: example response (HTML)
 * @explanation: explanation of example response or NULL if missing
 * @args: method arguments
 * @arg_count: number of arguments, may be 0
 *
 * An API method
 */
typedef struct flickcurl_method_s {
  char *name;
  int   needslogin;
  char *description;
  char *response;
  char *explanation;

  /* argument list */
  flickcurl_arg** args;
  int args_count;
  
} flickcurl_method;



/**
 * flickcurl_comment:
 * @name: Argument name
 * @optional: boolean flag (non-0 true) if argument is optional
 * @description: description of argument (HTML)
 *
 * An API method argument.
 */
typedef struct flickcurl_comment_s {
  char* id;
  char* author;
  char* authorname;
  int datecreate;
  char* permalink;
  char* text;
} flickcurl_comment;


/**
 * flickcurl_tag: 
 * @photo: Associated photo object if any
 * @id: tag identifier
 * @author: author (may be NULL)
 * @authornamae: author real name (may be NULL)
 * @raw: raw tag as user typed it (may be NULL, but if so @cooked must be not NULL)
 * @cooked: cooked tag (may be NULL, but if so @raw must not be NULL)
 * @machine_tag: boolean (non-0 true) if tag is a Machine Tag
 * @count: tag count in a histogram (or 0)
 *
 * A tag OR a posting of a tag about a photo by a user OR a tag in a histogram
 *
 * Most of these fields may be NULL, 0 for numbers
 * but not all.  Either @raw or @cooked MUST appear. 
 */
typedef struct flickcurl_tag_s {
  struct flickcurl_photo_s* photo;
  char* id;
  char* author;
  char* authorname;
  char* raw;
  char* cooked;
  int machine_tag;
  int count;
} flickcurl_tag;


typedef struct {
  char* string;
  flickcurl_photo_field_type integer;
  flickcurl_field_value_type type;
} flickcurl_photo_field;


/**
 * flickcurl_photo: 
 * @id: photo ID
 * @uri: photo page URI
 * @tags: array of tags (may be NULL)
 * @tags_count: size of tags array
 * @fields: photo fields
 *
 * A photo.
 *
 */
typedef struct flickcurl_photo_s {
  char *id;
  char *uri;
  
  flickcurl_tag** tags;
  int tags_count;
  
  flickcurl_photo_field fields[PHOTO_FIELD_LAST + 1];
} flickcurl_photo;


/**
 * flickcurl_license: 
 * @id: license ID
 * @url: license URL
 * @name: license short name
 *
 * A photo license.
 *
 */
typedef struct {
  /* license id */
  int id;
  /* license url or NULL if none */
  char *url;
  /* license name */
  char *name;
} flickcurl_license;


/**
 * flickcurl_contact:
 * @nsid: NSID
 * @username: user name
 * @iconserver:
 * @realname:
 * @is_friend:
 * @is_family:
 * @ignored:
 *
 * A contact.
 */
typedef struct flickcurl_contact_s {
  char *nsid;
  char *username;
  int iconserver;
  char *realname;
  int is_friend;
  int is_family;
  int ignored;
} flickcurl_contact;


/*
 * Types of photo context: relationship between photo and another item
 */
typedef enum {
  FLICKCURL_CONTEXT_NONE,
  FLICKCURL_CONTEXT_SET,  /* other thing is a set */
  FLICKCURL_CONTEXT_POOL, /* other thing is a pool */
  FLICKCURL_CONTEXT_PREV, /* other thing is a previous photo */
  FLICKCURL_CONTEXT_NEXT, /* other thing is a next photo */
  FLICKCURL_CONTEXT_LAST = FLICKCURL_CONTEXT_NEXT
} flickcurl_context_type;


typedef struct {
  flickcurl_context_type type;
  char* id;
  char* secret; /* may be NULL */
  int server;   /* may be 0 */
  int farm;     /* may be 0 */
  char* title;  /* may be NULL */
  char* url;    /* may be NULL */
  char* thumb;  /* may be NULL */
} flickcurl_context;


/*
 * Fields of a flickcurl_person*
 */
typedef enum {
  PERSON_FIELD_none,
  PERSON_FIELD_isadmin,               /* boolean */
  PERSON_FIELD_ispro,                 /* boolean */
  PERSON_FIELD_iconserver,            /* integer */
  PERSON_FIELD_iconfarm,              /* integer - not in API docs */
  PERSON_FIELD_username,              /* string */
  PERSON_FIELD_realname,              /* string */
  PERSON_FIELD_mbox_sha1sum,          /* string */
  PERSON_FIELD_location,              /* string */
  PERSON_FIELD_photosurl,             /* string */
  PERSON_FIELD_profileurl,            /* string */
  PERSON_FIELD_mobileurl,             /* string - not in API docs */
  PERSON_FIELD_photos_firstdate,      /* dateTime */
  PERSON_FIELD_photos_firstdatetaken, /* dateTime */
  PERSON_FIELD_photos_count,          /* integer */
  PERSON_FIELD_photos_views,          /* integer - not in API docs */
  PERSON_FIELD_LAST = PERSON_FIELD_photos_views
} flickcurl_person_field_type;


typedef struct {
  char* string;
  flickcurl_person_field_type integer;
  flickcurl_field_value_type type;
} flickcurl_person_field;
  

/**
 * flickcurl_person: 
 * @nsid: user NSID
 * @fields: person fields
 *
 * A flickr user.
 *
 */
typedef struct {
  char *nsid;

  flickcurl_person_field fields[PERSON_FIELD_LAST + 1];
} flickcurl_person;


/**
 * flickcurl_upload_params:
 * @photo_file: photo filename
 * @title: title or NULL
 * @description: description of photo (HTML) (or NULL)
 * @tags: space-separated list of tags (or NULL)
 * @is_public: is public photo boolean (non-0 true)
 * @is_friend: is friend photo boolean (non-0 true)
 * @is_family: is family photo boolean (non-0 true)
 * @safety_level: 1=safe, 2=moderate, 3=restricted
 * @content_type: 1=photo, 2=screenshot, 3=other/artwork
 *
 * Photo upload parameters
 */
typedef struct {
  const char* photo_file;
  const char *title;
  const char *description;
  const char *tags;  
  int is_public;
  int is_friend;
  int is_family;  
  int safety_level;
  int content_type;
} flickcurl_upload_params;


/**
 * flickcurl_upload_status:
 * @photoid: photo ID that was uploaded/replaced (upload only)
 * @secret: secret of photo uploaded (replace only)
 * @originalsecret: secret of original photo (replace only)
 * @ticketid: ticket ID for asynchronous upload (replace only)
 *
 * An upload response.
 *
 */
typedef struct {
  char *photoid;
  char *secret;
  char *originalsecret;
  char *ticketid;
} flickcurl_upload_status;


/**
 * flickcurl_search_params:
 * @user_id: The NSID of the user who's photo to search (or "me" or NULL).
 * @tags: A comma-delimited list of tags (or NULL)
 * @tag_mode: Either 'any' for an OR combination of tags, or 'all' for an AND combination. Defaults to 'any' if not specified (or NULL)
 * @text: Free text search (or NULL)
 * @min_upload_date: Minimum upload date as a unix timestamp (or NULL)
 * @max_upload_date: Maximum upload date as a unix timestamp (or NULL)
 * @min_taken_date: Minimum taken date in the form of a mysql datetime (or NULL)
 * @max_taken_date: Maximum taken date in the form of a mysql datetime (or NULL)
 * @license: Comma-separated list of photo licenses (or NULL)
 * @sort: The order in which to sort returned photos. Defaults to date-posted-desc. The possible values are: date-posted-asc, date-posted-desc, date-taken-asc, date-taken-desc, interestingness-desc, interestingness-asc, and relevance (or NULL)
 * @privacy_filter: Return photos only matching a certain privacy level.
 * @bbox: A comma-delimited list of 4 values defining the Bounding Box of the area that will be searched.
 * @accuracy: Recorded accuracy level of the location information.  Current range is 1-16 
 * @safe_search: Safe search setting: 1 safe, 2 moderate, 3 restricted.
 * @content_type: Content Type setting: 1 for photos only, 2 for screenshots only, 3 for 'other' only, 4 for all types. (or NULL)
 * @machine_tags: Machine tag search syntax 
 * @machine_tag_mode: Either 'any' for an OR combination of tags, or 'all' for an AND combination. Defaults to 'any' if not specified.
 * @group_id: The id of a group who's pool to search.  If specified, only matching photos posted to the group's pool will be returned. (or NULL)
 * @extras: A comma-delimited list of extra information to fetch for each returned record. Currently supported fields are: <code>license</code>, <code>date_upload</code>, <code>date_taken</code>, <code>owner_name</code>, <code>icon_server</code>, <code>original_format</code>, <code>last_update</code>, <code>geo</code>, <code>tags</code>, <code>machine_tags</code>. (or NULL)
 * @per_page: Number of photos to return per page. If this argument is omitted, it defaults to 100. The maximum allowed value is 500. (or NULL)
 * @page: The page of results to return. If this argument is omitted, it defaults to 1. (or NULL)
 *
 * Search parameters for &flickcurl_photos_search()
 */
typedef struct {
  const char* user_id;
  const char* tags;
  const char* tag_mode;
  const char* text;
  const char* min_upload_date;
  const char* max_upload_date;
  const char* min_taken_date;
  const char* max_taken_date;
  const char* license;
  const char* sort;
  const char* privacy_filter;
  const char* bbox;
  const char* accuracy;
  const char* safe_search;
  const char* content_type;
  const char* machine_tags;
  const char* machine_tag_mode;
  const char* group_id;
  const char* extras;
  const char* per_page;
  const char* page;
} flickcurl_search_params;
  

/* callback handlers */
typedef void (*flickcurl_message_handler)(void *user_data, const char *message);
typedef void (*flickcurl_tag_handler)(void *user_data, flickcurl_tag* tag);


/* library constants */
extern const char* const flickcurl_short_copyright_string;
extern const char* const flickcurl_copyright_string;
extern const char* const flickcurl_license_string;
extern const char* const flickcurl_home_url_string;
extern const char* const flickcurl_version_string;


/* library init - call once before creating anything */
void flickcurl_init(void);
/* library cleanup - call once before exit */
void flickcurl_finish(void);


/* flickcurl* object constructor */
flickcurl* flickcurl_new(void);

/* flickcurl* object destructor */
void flickcurl_free(flickcurl *fc);

/* flickcurl* object set methods */
void flickcurl_set_api_key(flickcurl* fc, const char *api_key);
void flickcurl_set_auth_token(flickcurl *fc, const char* auth_token);
void flickcurl_set_data(flickcurl *fc, void* data, size_t data_length);
void flickcurl_set_error_handler(flickcurl* fc, flickcurl_message_handler error_handler,  void *error_data);
void flickcurl_set_http_accept(flickcurl* fc, const char *value);
void flickcurl_set_proxy(flickcurl* fc, const char *proxy);
void flickcurl_set_request_delay(flickcurl *fc, long delay_msec);
void flickcurl_set_shared_secret(flickcurl* fc, const char *secret);
void flickcurl_set_sign(flickcurl *fc);
void flickcurl_set_tag_handler(flickcurl* fc,  flickcurl_tag_handler tag_handler, void *tag_data);
void flickcurl_set_user_agent(flickcurl* fc, const char *user_agent);
void flickcurl_set_write(flickcurl *fc, int is_write);
void flickcurl_set_xml_data(flickcurl *fc, xmlDocPtr doc);

/* flickcurl* object set methods */
const char* flickcurl_get_api_key(flickcurl *fc);
const char* flickcurl_get_shared_secret(flickcurl *fc);
const char* flickcurl_get_auth_token(flickcurl *fc);

/* other flickcurl class destructors */
void flickcurl_free_tag(flickcurl_tag *t);
void flickcurl_free_photo(flickcurl_photo *photo);
/* void flickcurl_free_license(flickcurl_person *license); */
void flickcurl_free_person(flickcurl_person *person);
void flickcurl_free_context(flickcurl_context *context);
void flickcurl_free_contexts(flickcurl_context** contexts);


/* utility methods */
/* get an image URL for a photo in some size */
char* flickcurl_photo_as_source_uri(flickcurl_photo *photo, const char c);
/* get labels for various field/types */
const char* flickcurl_get_photo_field_label(flickcurl_photo_field_type field);
const char* flickcurl_get_person_field_label(flickcurl_person_field_type field);
const char* flickcurl_get_field_value_type_label(flickcurl_field_value_type datatype);
const char* flickcurl_get_context_type_field_label(flickcurl_context_type type);


/* read a 'INI' style configuration file  */
typedef void (*set_config_var_handler)(void* userdata, const char* key, const char* value);
int read_ini_config(const char* filename, const char* application, void* user_data, set_config_var_handler handler);



/* Flickr API calls */

/* flickr.auth */
char* flickcurl_auth_checkToken(flickcurl* fc, const char* token);
char* flickcurl_auth_getFrob(flickcurl* fc);
char* flickcurl_auth_getFullToken(flickcurl* fc, const char* frob);
char* flickcurl_auth_getToken(flickcurl* fc, const char* frob);

/* flickr.groups */
flickcurl_context** flickcurl_groups_pools_getContext(flickcurl* fc, const char* photo_id, const char* group_id);

/* flickr.people */
char* flickcurl_people_findByEmail(flickcurl* fc, const char* email);
char* flickcurl_people_findByUsername(flickcurl* fc, const char* username);
flickcurl_person* flickcurl_people_getInfo(flickcurl* fc, const char* user_id);

/* flickr.photos */
int flickcurl_photos_addTags(flickcurl* fc, const char* photo_id, const char* tags);
int flickcurl_photos_delete(flickcurl* fc, const char* photo_id);
flickcurl_context** flickcurl_photos_getAllContexts(flickcurl* fc, const char* photo_id);
flickcurl_context** flickcurl_photos_getContext(flickcurl* fc, const char* photo_id);
flickcurl_photo* flickcurl_photos_getInfo(flickcurl *fc, const char* photo_id);
int flickcurl_photos_removeTag(flickcurl* fc, const char* tag_id);
flickcurl_photo** flickcurl_photos_search(flickcurl* fc, flickcurl_search_params* params);
int flickcurl_photos_setContentType(flickcurl* fc, const char* photo_id, int content_type);
int flickcurl_photos_setDates(flickcurl* fc, const char* photo_id, int date_posted, int date_taken, int date_taken_granularity);
int flickcurl_photos_setMeta(flickcurl* fc, const char* photo_id, const char* title, const char* description);
int flickcurl_photos_setPerms(flickcurl* fc, const char* photo_id, int is_public, int is_friend, int is_family, int perm_comment, int perm_addmeta);
int flickcurl_photos_setSafetyLevel(flickcurl* fc, const char* photo_id, int safety_level, int hidden);
int flickcurl_photos_setTags(flickcurl* fc, const char* photo_id, const char* tags);

/* flickr.contacts */
void flickcurl_free_contact(flickcurl_contact *contact_object);
void flickcurl_free_contacts(flickcurl_contact **contacts_object);
flickcurl_contact** flickcurl_contacts_getList(flickcurl* fc, const char* filter, int page, int per_page);
flickcurl_contact** flickcurl_contacts_getPublicList(flickcurl* fc, const char* user_id, int page, int per_page);

/* flickr.photos.comments */
void flickcurl_free_comment(flickcurl_comment *comment_object);
void flickcurl_free_comments(flickcurl_comment **comments_object);
char* flickcurl_photos_comments_addComment(flickcurl* fc, const char* photo_id, const char* comment_text);
int flickcurl_photos_comments_deleteComment(flickcurl* fc, const char* comment_id);
int flickcurl_photos_comments_editComment(flickcurl* fc, const char* comment_id, const char* comment_text);
flickcurl_comment** flickcurl_photos_comments_getList(flickcurl* fc, const char* photo_id);

/* flickr.photos.licenses */
flickcurl_license** flickcurl_photos_licenses_getInfo(flickcurl *fc);
flickcurl_license* flickcurl_photos_licenses_getInfo_by_id(flickcurl *fc, int id);

/* flickr.photosets */
flickcurl_context** flickcurl_photosets_getContext(flickcurl* fc, const char* photo_id, const char* photoset_id);

/* flickr.photosets.comments */
char* flickcurl_photosets_comments_addComment(flickcurl* fc, const char* photoset_id, const char* comment_text);
int flickcurl_photosets_comments_deleteComment(flickcurl* fc, const char* comment_id);
int flickcurl_photosets_comments_editComment(flickcurl* fc, const char* comment_id, const char* comment_text);
flickcurl_comment** flickcurl_photosets_comments_getList(flickcurl* fc, const char* photoset_id);
  
/* flickr.reflection */
void flickcurl_free_method(flickcurl_method *method);
char** flickcurl_reflection_getMethods(flickcurl* fc);
flickcurl_method* flickcurl_reflection_getMethodInfo(flickcurl* fc, const char* name);

/* flickr.tag */
flickcurl_tag** flickcurl_tags_getHotList(flickcurl* fc, const char* period, int tag_count);
flickcurl_tag** flickcurl_tags_getListPhoto(flickcurl* fc, const char* photo_id);
flickcurl_tag** flickcurl_tags_getListUser(flickcurl* fc, const char* user_id);
flickcurl_tag** flickcurl_tags_getListUserPopular(flickcurl* fc, const char* user_id, int pop_count);
flickcurl_tag** flickcurl_tags_getListUserRaw(flickcurl* fc, const char* tag);
flickcurl_tag** flickcurl_tags_getRelated(flickcurl* fc, const char* tag);

/* flickr.test */
int flickcurl_test_echo(flickcurl* fc, const char* key, const char* value);

/* flickr.urls */
char* flickcurl_urls_getGroup(flickcurl* fc, const char* group_id);
char* flickcurl_urls_getUserPhotos(flickcurl* fc, const char* user_id);
char* flickcurl_urls_getUserProfile(flickcurl* fc, const char* user_id);
char* flickcurl_urls_lookupGroup(flickcurl* fc, const char* url);
char* flickcurl_urls_lookupUser(flickcurl* fc, const char* url);

/* Upload API */
FLICKCURL_DEPRECATED flickcurl_upload_status* flickcurl_photos_upload(flickcurl* fc, const char* photo_file, const char *title, const char *description, const char *tags, int is_public, int is_friend, int is_family);
flickcurl_upload_status* flickcurl_photos_upload_params(flickcurl* fc, flickcurl_upload_params* params);
flickcurl_upload_status* flickcurl_photos_replace(flickcurl* fc, const char* photo_file, const char *photo_id, int async);
void flickcurl_free_upload_status(flickcurl_upload_status* status);
FLICKCURL_DEPRECATED void flickcurl_upload_status_free(flickcurl_upload_status* status);

#ifdef __cplusplus
}
#endif

#endif
