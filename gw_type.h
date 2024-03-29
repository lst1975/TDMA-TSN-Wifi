/**************************************************************************************
 *               TDMA Time-Sensitive-Network Wifi V1.0.1
 * Copyright (C) 2022 Songtao Liu, 980680431@qq.com.  All Rights Reserved.
 **************************************************************************************
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN ALL
 * COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. WHAT'S MORE, A DECLARATION OF 
 * NGRTOS MUST BE DISPLAYED IN THE FINAL SOFTWARE OR PRODUCT RELEASE. NGRTOS HAS 
 * NOT ANY LIMITATION OF CONTRIBUTIONS TO IT, WITHOUT ANY LIMITATION OF CODING STYLE, 
 * DRIVERS, CORE, APPLICATIONS, LIBRARIES, TOOLS, AND ETC. ANY LICENSE IS PERMITTED 
 * UNDER THE ABOVE LICENSE. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF 
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO 
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES 
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *
 **************************************************************************************
 *                              
 *                    https://github.com/lst1975/TDMA-TSN-Wifi
 *                              
 **************************************************************************************
 */
#ifndef _GW_TYPE_H_
#define _GW_TYPE_H_

#define __TSN_INTERFACE
#define __TSN_UNUSED __attribute__((unused))
#define __TSN_ALIGN(x) __attribute__ ((aligned (x)))

#define __TSN_ALIGN_default 128
#define ____TSN_ALIGN___ __attribute__ ((aligned (__TSN_ALIGN_default)))

static inline uint64_t __htonll(uint64_t val) 
{
  return (((uint64_t) htonl(val)) << 32) + htonl(val >> 32);
}

static inline uint64_t __ntohll(uint64_t val) 
{
  return (((uint64_t) ntohl(val)) << 32) + ntohl(val >> 32);
}

#define TSN_ntohs   ntohs
#define TSN_ntohl   ntohl
#define TSN_ntohll  __ntohll
#define TSN_htons   htons
#define TSN_htonl   htonl
#define TSN_htonll  __htonll

typedef uint8_t  KeyData[16];
typedef uint8_t  BitField8[1];
typedef uint8_t  BitField16[2];
typedef uint8_t  BitField24[3];
typedef uint8_t  Unsigned8;
typedef uint16_t Unsigned16;
typedef uint8_t  Unsigned24[3];
typedef uint32_t Unsigned32;
typedef uint8_t  Unsigned40[5];
typedef uint8_t  Unsigned48[6];
typedef uint64_t Unsigned64;
typedef uint8_t  Unsigned80[10];
typedef uint8_t  Unsigned128[16];
typedef uint64_t TimeData;
typedef unsigned long tsn_ulong;
typedef long tsn_slong;
typedef tsn_slong tsn_mibid_t;
typedef Unsigned8 tsn_handle;
typedef Unsigned128 KeyData;

typedef unsigned int tsn_uint_t;
typedef int tsn_int_t;
typedef time_t tsn_msec_t;
typedef long tsn_err_e;
typedef int tsn_socket_t;

enum tsn_boolean{
  TSN_FALSE=0,
  TSN_TRUE=1,
};
typedef enum tsn_boolean tsn_boolean_e;
#define TSN_SUCCESS TSN_TRUE
#define TSN_FAILURE TSN_FALSE

#define TsnFloat_TYPE_single_float 0
#define TsnFloat_TYPE_double_float 1
#define TsnFloat_TYPE_invalid      2 
struct _TsnFloat{
  int64_t integerPart;
  uint64_t fractionPart;
  uint64_t type;
  double   value;
};
typedef struct _TsnFloat TsnFloat;

struct _SingleFloat{
  uint32_t SN:1;
  uint32_t E:8;
  uint32_t F:23;
};
typedef struct _SingleFloat SingleFloat;
#define SingleFloatNaN 0x7FA00000

static inline void 
SINGLE_FLOAT_to_tsn_float(TsnFloat *v, SingleFloat f)
{
  if (*((uint32_t*)&f) == SingleFloatNaN)
  {
    v->type = TsnFloat_TYPE_invalid;
    return;
  }
  v->integerPart = f.E;
  if (f.SN)
    v->integerPart = -f.E;
  v->fractionPart = f.F;
  v->value = v->integerPart + v->fractionPart/0x800000;
}

static inline void 
tsn_float_to_SINGLE_FLOAT(TsnFloat *v, SingleFloat f)
{
  if (v->type == TsnFloat_TYPE_invalid)
  {
    *((uint32_t*)&f) = SingleFloatNaN;
    return;
  }
  f.SN = (v->integerPart < 0);
  f.E = f.SN ? -v->integerPart : v->integerPart;
  f.F = v->fractionPart;
}

static inline void 
__float_to_tsn_FLOAT(double v, TsnFloat *f)
{
  f->type = TsnFloat_TYPE_single_float;
  f->value = v;
  f->integerPart  = (int64_t)v;
  f->fractionPart = (uint64_t)((v-(int64_t)v)*0x800000);
}

static inline void 
__single_to_DOUBLE_FLOAT(double v, SingleFloat f)
{
  f.SN = v < 0;
  f.E = f.SN ? (uint32_t)-v : (uint32_t)v;
  f.F = (uint32_t)((v-(int32_t)v)*0x800000);
}

struct _DoubleFloat{
  uint64_t SN:1;
  uint64_t E:11;
  uint64_t F:52;
};
typedef struct _DoubleFloat DoubleFloat;
#define DoubleFloatNaN 0x7FA0000000000000

static inline void 
DOUBLE_FLOAT_to_tsn_float(TsnFloat *v, DoubleFloat f)
{
  if (*((uint64_t*)&f) == DoubleFloatNaN)
  {
    v->type = TsnFloat_TYPE_invalid;
    return;
  }
  v->integerPart = f.SN ? -f.E : f.E;
  v->fractionPart = f.F;
  v->value = v->integerPart + v->fractionPart/0x10000000000000;
}

static inline void 
tsn_float_to_DOUBLE_FLOAT(TsnFloat *v, DoubleFloat f)
{
  if (v->type == TsnFloat_TYPE_invalid)
  {
    *((uint64_t*)&f) = DoubleFloatNaN;
    return;
  }
  f.SN = (v->integerPart < 0);
  f.E  = f.SN ? -v->integerPart : v->integerPart;
  f.F  = v->fractionPart;
}

static inline void 
__double_to_tsn_FLOAT(double v, TsnFloat *f)
{
  f->type = TsnFloat_TYPE_double_float;
  f->value = v;
  f->integerPart  = (int64_t)v;
  f->fractionPart = (uint64_t)((v-(int64_t)v)*0x10000000000000);
}

static inline void 
__double_to_DOUBLE_FLOAT(double v, DoubleFloat f)
{
  f.SN = v < 0;
  f.E = f.SN ? (uint64_t)-v : (uint64_t)v;
  f.F = (uint64_t)((v-(int64_t)v)*0x10000000000000);
}

struct _OctetString{
  uint16_t length;
  uint8_t data[0];
};
typedef struct _OctetString OctetString;

struct _BitsString{
  uint16_t length;
  uint8_t data[0];
};
typedef struct _BitsString BitsString;

enum{
  DATA_TYPE_KeyData,
  DATA_TYPE_TimeData,
  DATA_TYPE_Unsigned8,
  DATA_TYPE_Unsigned16,
  DATA_TYPE_Unsigned24,
  DATA_TYPE_Unsigned32,
  DATA_TYPE_Unsigned40,
  DATA_TYPE_Unsigned48,
  DATA_TYPE_Unsigned64,
  DATA_TYPE_Unsigned80,
  DATA_TYPE_Unsigned128,
  DATA_TYPE_SingleFloat,
  DATA_TYPE_DoubleFloat,
  DATA_TYPE_OctetString,
  DATA_TYPE_BitsString,
  DATA_TYPE_BitField8,
  DATA_TYPE_BitField16,
  DATA_TYPE_BitField24,
  DATA_TYPE_List,
  DATA_TYPE_Struct,
  DATA_TYPE_MAX,
};

union __tsn_value_type{
  void         *value_List;
  void         *value_Attrs;
  Unsigned8     value_Unsigned8;
  Unsigned16    value_Unsigned16;
  Unsigned24    value_Unsigned24;
  Unsigned32    value_Unsigned32;
  Unsigned40    value_Unsigned40;
  Unsigned48    value_Unsigned48;
  Unsigned64    value_Unsigned64;
  Unsigned80    value_Unsigned80;
  Unsigned8     value_BitField8;
  Unsigned16    value_BitField16;
  Unsigned24    value_BitField24;
  TimeData      value_TimeData;
  KeyData       value_KeyData;
  OctetString   value_OctetString;
  BitsString    value_BitsString;
  double        value_SingleFloat;
  double        value_DoubleFloat;
};
typedef union __tsn_value_type tsn_value_type_u;

static inline tsn_boolean_e
tsn_data_is_equal(int type, void *value1, void *value2, size_t len)
{
  if (type < 0 || type >= DATA_TYPE_MAX)
    return TSN_FALSE;
  switch (type)
  {
    case DATA_TYPE_TimeData:
      return *(TimeData*)value1 == *(TimeData*)value2;
    case DATA_TYPE_BitField8:
    case DATA_TYPE_Unsigned8:
      return *(Unsigned8*)value1 == *(Unsigned8*)value2;
    case DATA_TYPE_BitField16:
    case DATA_TYPE_Unsigned16:
      return *(Unsigned16*)value1 == *(Unsigned16*)value2;
    case DATA_TYPE_BitField24:
    case DATA_TYPE_Unsigned24:
      return !tsn_memcmp(value1, value2, 3);
    case DATA_TYPE_Unsigned32:
      return *(Unsigned32*)value1 == *(Unsigned32*)value2;
    case DATA_TYPE_Unsigned40:
      return !tsn_memcmp(value1, value2, 5);
    case DATA_TYPE_Unsigned48:
      return !tsn_memcmp(value1, value2, 6);
    case DATA_TYPE_Unsigned64:
      return *(Unsigned64*)value1 == *(Unsigned64*)value2;
    case DATA_TYPE_Unsigned80:
      return !tsn_memcmp(value1, value2, 10);
    case DATA_TYPE_KeyData:
    case DATA_TYPE_Unsigned128:
      return !tsn_memcmp(value1, value2, 16);
    case DATA_TYPE_SingleFloat:
      return !tsn_memcmp(value1, value2, sizeof(SingleFloat));
    case DATA_TYPE_DoubleFloat:
      return !tsn_memcmp(value1, value2, sizeof(DoubleFloat));
    case DATA_TYPE_List:
    case DATA_TYPE_Struct:
      return !tsn_memcmp(value1, value2, len);
    case DATA_TYPE_OctetString:
      {
      OctetString *b1 = (OctetString *)value1;
      OctetString *b2 = (OctetString *)value1;
      if (b1->length != b2->length)
        return TSN_FALSE;
      return !tsn_memcmp(b1->data, b2->data, b1->length);
      }
    case DATA_TYPE_BitsString:
      {
      BitsString *b1 = (BitsString *)value1;
      BitsString *b2 = (BitsString *)value1;
      if (b1->length != b2->length)
        return TSN_FALSE;
      return !tsn_memcmp(b1->data, b2->data, b1->length);
      }
    default:
      return TSN_FALSE;
  }
}

#define Unsigned64Max 0xFFFFFFFFFFFFFFFF
#define Unsigned32Max 0xFFFFFFFF
#define Unsigned16Max 0xFFFF
#define Unsigned8Max  0xFF
#define TimeDataMax   Unsigned64Max
#define _D8M          Unsigned8Max
#define Unsigned24Max  { _D8M, _D8M, _D8M }
#define Unsigned40Max  { _D8M, _D8M, _D8M, _D8M, _D8M }
#define Unsigned48Max  { _D8M, _D8M, _D8M, _D8M, _D8M, _D8M }
#define Unsigned80Max  { _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M }
#define Unsigned128Max { _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M, _D8M }
#define KeyDataMax Unsigned128Max

#define TSN_FALSE 0
#define TSN_TRUE (!TSN_FALSE)

#define __NENTS(a) (sizeof(a)/sizeof((a)[0]))

#define tsn_offsetof(TYPE, MEMBER)          ((size_t) &((TYPE*)0)->MEMBER)
#define tsn_container_of(ptr, type, member) (type *)((char *)(ptr) - tsn_offsetof(type,member) )

#define tsn_log(type, fmt, ...) (void)(0)
#define tsn_assert(con) if (!(con)) { while(1) { TSN_error("Assert failed"); } } 

typedef tsn_err_e (*tsn_primative_f)(void *);
struct tsn_primative{
  void *data;
  tsn_primative_f request;
  tsn_primative_f indication;
  tsn_primative_f response;
  tsn_primative_f confirm;
};
typedef struct tsn_primative tsn_primative_s;

#endif
