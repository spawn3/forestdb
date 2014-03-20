/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2010 Couchbase, Inc
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef _JSAHN_HBTRIE_H
#define _JSAHN_HBTRIE_H

#include "common.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HBTRIE_MAX_KEYLEN FDB_MAX_KEYLEN

typedef size_t hbtrie_func_readkey(void *handle, uint64_t offset, void *buf);

typedef enum {
    HBTRIE_RESULT_SUCCESS,
    HBTRIE_RESULT_UPDATE,
    HBTRIE_RESULT_FAIL
} hbtrie_result;

struct btree_blk_ops;
struct btree_kv_ops;
struct hbtrie {
    uint8_t chunksize;
    uint8_t valuelen;
    uint32_t btree_nodesize;
    bid_t root_bid;
    void *btreeblk_handle;
    void *doc_handle;

    struct btree_blk_ops *btree_blk_ops;
    struct btree_kv_ops *btree_kv_ops;
    hbtrie_func_readkey *readkey;
};

struct hbtrie_iterator {
    struct hbtrie trie;
    struct list btreeit_list;
    void *curkey;
    size_t keylen;
};

int _hbtrie_reform_key(struct hbtrie *trie, void *rawkey, int rawkeylen, void *outkey);
void hbtrie_get_chunk(struct hbtrie *trie, void *key, int keylen, int chunkno, void *out);

void hbtrie_init(
            struct hbtrie *trie, int chunksize,     int valuelen,    int btree_nodesize, bid_t root_bid,
            void *btreeblk_handle, struct btree_blk_ops *btree_blk_ops,
            void *doc_handle, hbtrie_func_readkey *readkey);
void hbtrie_free(struct hbtrie *trie);

hbtrie_result hbtrie_iterator_init(
    struct hbtrie *trie, struct hbtrie_iterator *it, void *initial_key, size_t keylen);
hbtrie_result hbtrie_iterator_free(struct hbtrie_iterator *it);
hbtrie_result hbtrie_next(struct hbtrie_iterator *it, void *key_buf, size_t *keylen, void *value_buf);

hbtrie_result hbtrie_find(struct hbtrie *trie, void *rawkey, int rawkeylen, void *valuebuf);
hbtrie_result hbtrie_remove(struct hbtrie *trie, void *rawkey, int rawkeylen);
hbtrie_result hbtrie_insert(struct hbtrie *trie, void *rawkey, int rawkeylen,
            void *value, void *oldvalue_out);

#ifdef __cplusplus
}
#endif

#endif
