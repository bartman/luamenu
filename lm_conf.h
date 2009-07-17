#ifndef LM_CONF_H
#define LM_CONF_H

typedef char * lm_conf_string_t;
typedef unsigned long lm_conf_number_t;

struct lm_config {
	lm_conf_string_t prompt;
	lm_conf_number_t lines;
};

extern struct lm_config conf;

#endif // LM_CONF_H
