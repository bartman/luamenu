#ifndef LM_CONF_H
#define LM_CONF_H

struct lm_config {
	char *prompt;
	unsigned int lines;
};

extern struct lm_config conf;

#endif // LM_CONF_H
