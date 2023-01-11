#ifndef _GW_STATE_H__
#define _GW_STATE_H__

#define TSN_FieldDevice_MAX 1024

enum tsn_dmap_state{
	DMAP_STATE_init,
	DMAP_STATE_active,
	DMAP_STATE_join,
	DMAP_STATE_join_authenticating,
	DMAP_STATE_join_allocatingShortAddress,
	DMAP_STATE_operation,
	DMAP_STATE_resource_allocation,
	DMAP_STATE_leave,
	DMAP_STATE_end
};
typedef enum tsn_dmap_state tsn_dmap_state_e;

enum tsn_dmap_trigger{
	DMAP_TRIGGER_T0,
	DMAP_TRIGGER_T1,
	DMAP_TRIGGER_T1_auth,
	DMAP_TRIGGER_T1_addr,
	DMAP_TRIGGER_T2,
	DMAP_TRIGGER_T3,
	DMAP_TRIGGER_T4,
	DMAP_TRIGGER_T5,
	DMAP_TRIGGER_T6,
	DMAP_TRIGGER_T7,
	DMAP_TRIGGER_T8,
	DMAP_TRIGGER_T9,
	DMAP_TRIGGER_T10,
	DMAP_TRIGGER_T11,
	DMAP_TRIGGER_T12,
	DMAP_TRIGGER_T13,
	DMAP_TRIGGER_T14,
	DMAP_TRIGGER_T15,
	DMAP_TRIGGER_T16,
	DMAP_TRIGGER_MAX,
};
typedef enum tsn_dmap_trigger tsn_dmap_trigger_e;

enum ad_state{
	AD_STATE_init,
	AD_STATE_idle,
	AD_STATE_s1,
	AD_STATE_s2,
	AD_STATE_s3,
	AD_STATE_s4,
	AD_STATE_s5,
	AD_STATE_s6,
	AD_STATE_s7,
	AD_STATE_s8,
};

#endif
