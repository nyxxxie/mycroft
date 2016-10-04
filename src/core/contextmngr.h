#ifndef MYCROFT_CONTEXTMGR_H
#define MYCROFT_CONTEXTMGR_H

typedef struct {
    mc_ctx_t* ctx_list;
    uint32_t  ctx_amt;
} mct_ctx_list_t;

mc_ctxmgr_add_ctx(mc_ctx_list_t* list, mc_ctx_t* ctx);
mc_ctxmgr_remove_ctx(mc_ctx_list_t* list, uint32_t index);
mc_ctxmgr_get_ctx(mc_ctx_list_t* list, uint32_t index);

#endif // MYCROFT_CONTEXTMGR_H
