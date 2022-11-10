#ifndef WORKSHOP
#define WORKSHOP

typedef struct workshop *Workshop;

typedef struct users{
    char *username;
    char *name;
    char *birth_day;
    char *account_creation; 
    char *pay_method;
    char *account_status;
}Users; 

Users init_user(const char *str);
char* get_username(Users u);
char* get_user_info(Users u);

#endif