#include "opl/src/h/system.h"
#include "opl/src/h/graphics.h"
#include "opl/src/h/util.h"
#include "opl/src/h/sound.h"
#include "opl/src/h/input.h"
#include "opl/src/h/net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct identifier_data
{
    char **strs;
    s32 strs_size;
} identifier_data;
identifier_data ctor_identifier_data()
{
    identifier_data i;
    memset(&i,0,sizeof(identifier_data));
    return i;
}
typedef struct ovp
{
    //identifiers and identifier_data index the same
    char **identifiers;
    s32 identifiers_size;
    
    identifier_data *identifiers_data;
}ovp; 

identifier_data *ovp_get(ovp *o,char *name)
{
    s32 i=0;
    for(; i<o->identifiers_size; i++)
    {
        if(strcmp(o->identifiers[i],name))
        {
            return &o->identifiers_data[i]; 
        }
    }
    return NULL;    
}

static void ovp_error(char *err_str,char *context)
{
    printf("%s\nContext:\n%s\n",err_str,context);
    exit(-1);
} 
static void parse_words(ovp *o, char *str, s32 *i)
{
   
    #define d_push_word o->identifiers_data[o->identifiers_size-1].strs_size+=1;\
    o->identifiers_data[o->identifiers_size-1].strs = \
        realloc(o->identifiers_data[o->identifiers_size-1].strs,\
            sizeof(char*) * o->identifiers_data[o->identifiers_size-1].strs_size);\
    o->identifiers_data[o->identifiers_size-1].strs[\
        o->identifiers_data[o->identifiers_size-1].strs_size-1]\
            =word;
             
    char *word=malloc(1);
    word[0]=0;
   
    for(;str[*i] && str[*i]==' ';(*i)++);//read whitespace before word
    for(;str[*i] && str[*i]!=' ';(*i)++)
    {
        if(str[*i]=='\n')
        {
            d_push_word
            return;
        }
        //read word
        char *temp=word;
        char *temp2=malloc(2);
        temp2[0]=str[*i];
        temp2[1]=0;
        word=str_cat(word,temp2);
        free(temp);
        free(temp2);
    }
    
    if(!str[*i])
    {
        d_push_word
        return;
    }
    
    d_push_word  
    parse_words(o,str,i);   
}
ovp *ctor_ovp(char *ovp_str)
{
    s32 ovp_index=0;
    
    ovp *result=malloc(sizeof(ovp));
    
    result->identifiers_size=0;
    result->identifiers=NULL;
    
    result->identifiers_data=NULL;
    
    for(;ovp_str[ovp_index];ovp_index++)
    {
        //the outer root only runs after newline, to look for another identifier
        //otherwise inner loops parse the tokens they excpect to follow
        
        char *identifier=malloc(1);
        identifier[0]=0;
        
        if(ovp_str[ovp_index]!=' ')
        {
            if(ovp_str[ovp_index]=='#')
            {
                //@bug for now just read until after newline
                for(;ovp_str[ovp_index]!='\n';ovp_index++);
                continue;
            }
            
            //found identifier, parse it
            for(;ovp_str[ovp_index];ovp_index++)
            {
                if(ovp_str[ovp_index]==':'||ovp_str[ovp_index]==' ')
                {
                    //should have a valid identifier now
                    if(strlen(identifier)<1)
                    {
                        ovp_error("No valid identifier exists.",identifier);
                    }
                    
                    for(;ovp_str[ovp_index]==' ';ovp_index++);//read whitespace after identifier
    
                    if(ovp_str[ovp_index]==':')
                    {
                        //add the identifier
                        result->identifiers_size+=1;
                        result->identifiers=realloc(result->identifiers,sizeof(char*)*result->identifiers_size);
                        result->identifiers[result->identifiers_size-1]=identifier;
        
                        //add the identifier_data structure
                        result->identifiers_data=realloc(
                            result->identifiers_data,
                            result->identifiers_size * sizeof(identifier_data));
                        result->identifiers_data[result->identifiers_size-1]=
                            ctor_identifier_data();
                        
                        ovp_index++;
                        for(;ovp_str[ovp_index]&&ovp_str[ovp_index]==' ';ovp_index++);//read whitespace after colon
                        
                        parse_words(result, ovp_str, &ovp_index);
                        //parse words returns when ovp_str[ovp_index]=='\n' || eof || eos
                        
                        //log some stuff
                        char *ident=result->identifiers[result->identifiers_size-1];
                        printf("\n%d\n%s\n",result->identifiers_size,ident);
                        for(s32 x=0; x<result->identifiers_data[result->identifiers_size-1].strs_size; x++)
                        {
                            printf("%s\n",result->identifiers_data[result->identifiers_size-1].strs[x]);
                        }
                        
                        break;
                    }
                    else
                    {//after whitespace, there was no colon
                        ovp_error("Expected colon after identifier.",identifier);
                    }
                }
                else if(ovp_str[ovp_index]!='\n')
                {
                    //if its a valid character append it to identifier
                    //@todo str_append function
                    char *temp=identifier;
                    char *temp2=malloc(2);
                    temp2[0]=ovp_str[ovp_index];
                    temp2[1]=0;
                    identifier=str_cat(identifier,temp2);
                    free(temp);
                    free(temp2);
                }
            }
        }
    }
    
    return result;
}
void dtor_ovp(ovp *o)
{
    //@todo
}

int main()
{
    ovp *result=ctor_ovp(malloc_file_cstr("../../ode/bin/res/syntax_c.var"));
    return 0;
}