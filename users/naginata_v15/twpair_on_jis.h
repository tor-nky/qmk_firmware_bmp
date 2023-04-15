#ifndef USONJIS_H
#define USONJIS_H

void us2jis_on(void);
void us2jis_off(void);
bool us2jis_state(void);
bool twpair_on_jis(uint16_t, keyrecord_t *);

#endif
