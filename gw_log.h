#ifndef _GW_LOG_H_
#define _GW_LOG_H_

const char *TSN_err2str(int err);
void ___TSN_string(const char *type, const char *fmt, ...);

#define TSN_error(fmt, ...) ___TSN_string("Error", fmt, ##__VA_ARGS__)
#define TSN_event(fmt, ...) ___TSN_string("Event", fmt, ##__VA_ARGS__)
#define TSN_warn(fmt, ...)  ___TSN_string("Warn",  fmt, ##__VA_ARGS__)
#define TSN_debug(fmt, ...) ___TSN_string("Debug", fmt, ##__VA_ARGS__)

void tsn_print_sockaddr(struct sockaddr *sa);

#endif
