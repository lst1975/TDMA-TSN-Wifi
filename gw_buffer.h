#ifndef __GW_BUFFER_H__
#define __GW_BUFFER_H__

struct tsn_buffer{
	Unsigned8 *data;
	Unsigned8 *ptr;
	Unsigned32 size;
	Unsigned32 len;
};
typedef struct tsn_buffer tsn_buffer_s;

#define TSN_BUFFER_GO(b,len) { (b)->ptr += len; (b)->len += len; }
#define TSN_BUFFER_PTR(b) (b)->ptr
#define TSN_BUFFER_LEN(b) (b)->len

static inline void tsn_buffer_reinit(tsn_buffer_s *b)
{
	b->ptr   = b->data;
	b->len   = 0;
}

static inline void tsn_buffer_init(tsn_buffer_s *b, Unsigned8 *data, int size)
{
	b->ptr   = data;
	b->len   = 0;
	b->size  = size;
	b->data  = data;
}

static inline void tsn_buffer_set(tsn_buffer_s *b, Unsigned8 *data, int len)
{
	b->ptr   = data;
	b->len   = len;
	b->size  = len;
	b->data  = data;
}

static inline void tsn_buffer_put8(tsn_buffer_s *b, Unsigned8 v)
{
	*b->ptr++ = v;
	b->len ++;
}

static inline void tsn_buffer_put16(tsn_buffer_s *b, Unsigned16 v)
{
	*(Unsigned16*)b->ptr = TSN_htons(v);
	b->ptr += 2;
	b->len += 2;
}

static inline void tsn_buffer_put32(tsn_buffer_s *b, Unsigned32 v)
{
	*(Unsigned32*)b->ptr = TSN_htonl(v);
	b->ptr += 4;
	b->len += 4;
}

static inline void tsn_buffer_put64(tsn_buffer_s *b, Unsigned64 v)
{
	*(Unsigned32*)b->ptr = TSN_htonll(v);
	b->ptr += 8;
	b->len += 8;
}

static inline void tsn_buffer_putlen(tsn_buffer_s *b, Unsigned8 *data, int len)
{
	tsn_memcpy(b->ptr, data, len);
	b->ptr += len;
	b->len += len;
}

static inline void tsn_buffer_get8(tsn_buffer_s *b, Unsigned8 *v)
{
	*v = *b->ptr++;
	b->len--;
}

static inline void tsn_buffer_get16(tsn_buffer_s *b, Unsigned16 *v)
{
	*v = TSN_ntohs(*(Unsigned16*)b->ptr);
	b->ptr += 2;
	b->len -= 2;
}

static inline void tsn_buffer_get32(tsn_buffer_s *b, Unsigned32 *v)
{
	*v = TSN_ntohl(*(Unsigned32*)b->ptr);
	b->ptr += 4;
	b->len -= 4;
}

static inline void tsn_buffer_get64(tsn_buffer_s *b, Unsigned64 *v)
{
	*v = TSN_ntohll(*(Unsigned64*)b->ptr);
	b->ptr += 8;
	b->len -= 8;
}

static inline void tsn_buffer_getlen(tsn_buffer_s *b, Unsigned8 *data, int len)
{
	tsn_memcpy(data, b->ptr, len);
	b->ptr += len;
	b->len -= len;
}

#endif
