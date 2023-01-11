#ifndef __GW_NUMBER_H__
#define __GW_NUMBER_H__

struct name_number{
  int number;
  const char *name;
  tsn_err_e (*func)(void *raw, tsn_buffer_s *b);
};
typedef struct name_number name_number_s;

#endif
