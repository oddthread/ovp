#ifndef OVP_H
#define OVP_H

#include "oul/src/h/oul.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct identifier_data
{
    char **strs;
    s32 strs_size;
} identifier_data;

typedef struct ovp
{
    //identifiers and identifier_data index the same
    char **identifiers;
    s32 identifiers_size;
    
    identifier_data *identifiers_data;
} ovp;

void dtor_ovp(ovp *o);
ovp *ctor_ovp(char *ovp_str);

#endif