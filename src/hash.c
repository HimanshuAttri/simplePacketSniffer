#include <stdint.h>
#include "hash.h"
#include "ps_stats.h"

static void ht_entry_init(ht_entry_t * entry) {
    entry->key = 0;
    l3l4_quin_init(&entry->value);
    ps_stats_init(&entry->stats);
}

static void ht_bucket_init(ht_bucket_t *bucket) {
    bucket->entries = 0;
    bucket->filled = 0;
    bucket->collisions = 0;
    ht_entry_init(&bucket->entry);
    bucket->next = (ht_entry_t *)0; 
}

void ht_init(ht_table_t *ht) {
    uint32_t i;

    ht->entries = 0;   // number of entries in table
    ht->collisions = 0; // number of table collisions

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        ht_bucket_init(&ht->bucket[i]);
    }
}

void ht_print(ht_table_t *ht) {
}

ht_ret_t ht_add(ht_table_t *ht, l3l4_quin_t *quin, uint16_t packet_len) {
    ht_ret_t ht_ret = ht_ret_ok;

    return ht_ret;
}


