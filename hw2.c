/*hw2.c*/
/*Sitong Liu*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
1.s represent the function
2.len is the length of the function
3.result counts the numbers
4.var_len saves the length of result
5.operators saves the functions from left to rigfht
6.op_len saves the length of operators

For example，s is "x=y+z-1"
Then，len = 7
result = {"x","y","z","1"}
var_len = 4
operators = {'=','+','-'}
op_len = 3

*/
void split(char * s, int len, char vars[BUFSIZ >> 1][BUFSIZ >> 1], int *var_len, char operators[BUFSIZ >> 1], int *op_len) {
    int i = 0;
    *var_len = 0;
    *op_len = 0;
    int flag_equal = 0;
    while (i != len) {
        // Find the first word that is not an operator
        int flag = 0;
        while (i != len && flag == 0) {
            flag = 1;

            if (s[i] == '=' && flag_equal) {
                ++i;
                flag = 0;
            }

            if (s[i] == '+') {
                ++i;
                flag = 0;
            }

            if (s[i] == '-') {
                ++i;
                flag = 0;
            }
        }

        //Find anthor word that is not an operator, get the string between
        flag = 0;
        int j = i;
        while (j != len && flag == 0) {
            if (s[j] == '=') {
                if (!flag_equal) {
                    flag = 1;
                    flag_equal = 1;
                    operators[*op_len] = '=';
                    (*op_len)++;
                } else//multi equation,fail
                {
                    *var_len = 0;
                    *op_len = 0;
                    return;
                }
            }

            if (s[j] == '+') {
                flag = 1;
                operators[*op_len] = '+';
                (*op_len)++;
            }

            if (s[j] == '-') {
                flag = 1;
                operators[*op_len] = '-';
                (*op_len)++;
            }

            if (flag == 0) {
                ++j;
            }
        }

        //give the value to vars
        if (i != j) {
            int m;
            for (m = 0; m < j - i; m++) {
                vars[*var_len][m] = s[i + m];
            }
            vars[*var_len][j - i] = '\0';
            (*var_len)++;
            i = j;
        }
    }
}

char* trim(char *str, char ch) {
    int i = -1, j = strlen(str), cur = 0;
    char *fp = NULL;
    char *fp_temp = NULL;
    if (str == NULL) {
        return NULL;
    }

    while (str[++i] == ch) {

    }
    while (str[--j] == ch) {

    }
    for (; i <= j; ++i, ++cur) {
        str[cur] = str[i];
    }
    str[cur] = '\0';
    fp = fp_temp = str;
    while (*str) {
        if (*str != ch) {
            *fp_temp = *str;
            fp_temp++;
        } else {
            if (*(fp_temp - 1) != ch) {
                *fp_temp = *str;
                fp_temp++;
            }
        }
        str++;
    }
    *fp_temp = '\0';
    return fp;
}

//check the string whther is as the rule of c language
int is_alpha_string(char *str) {

    if (str == NULL) {
        return 0;
    }
    int end_space = 0;
    int len = strlen(str);

    int i;
    for (i = 0; i < len; i++) {
        if (isblank(str[i])) {
            continue;
        } else {
            break;
        }
    }
    if (i == len) {
        return 0;
    }

    if (!isalpha(str[i]) && str[i] != '_') {
        return 0;
    }

    for (i = i + 1; i < len; i++) {
        if (isblank(str[i])) {
            end_space = 1;
        }
        // illegal function like: ab,cd
        if (end_space && !isblank(str[i])) {
            return 0;
        }

        if (!isalnum(str[i]) && str[i] != '_' && !end_space) {
            return 0;
        }
    }
    return 1;
}

//whther the string is num
int is_num_string(char *str) {
    if (str == NULL) {
        return 0;
    }

    int end_space = 0;
    int len = strlen(str);

    int i;
    for (i = 0; i < len; i++) {
        if (isblank(str[i])) {
            continue;
        } else {
            break;
        }
    }
    if (i == len) {
        return 0;
    }

    int flag_dot = 0;
    for (i = i + 0; i < len; i++) {
        if (isblank(str[i])) {
            end_space = 1;
        }

        if (end_space && !isblank(str[i])) {
            return 0;
        }
        // contain 1'.' most
        if (str[i] == '.') {
            if (!flag_dot) {
                flag_dot = 1;
            } else {
                return 0;
            }
            continue;
        }

        if (!isdigit(str[i]) && !end_space) {
            return 0;
        }
    }
    return 1;
}

//clear 
void clear_screen() {
#ifdef WIN32    //windows
    system("cls");
#else           //linux or macOS
    system("clear");
#endif
}

int main(int argc, char ** argv) {
    printf("Please enter a valid C assignment statement:\n");
    // get the assignment and count the length
    char str[BUFSIZ];
    fgets(str, BUFSIZ, stdin);
    //gets(str);
    int len = strlen(str);
    int i;
    int flag_blank = 1;
    for (i = len - 1; i > 0; i--) {
        if (!isblank(str[i]) && str[i] != ';') {
            flag_blank = 1;
        }
        if (str[i] == ';') {
            if (flag_blank) {
                str[i] = '\0';
                flag_blank = 0;
                break;
            } else {
                printf("parse error\n");
                return 1;
            }

        }
    }
    if (flag_blank) {
        printf("parse error\n");
        return 1;
    }
    char vars[BUFSIZ >> 1][BUFSIZ >> 1];
    int var_len = 0;
    char operators[BUFSIZ >> 1];
    int op_len = 0;

    //split the assginment into operators and variebles
    split(str, len, vars, &var_len, operators, &op_len);

    // The first operator must be '='
    if (op_len < 2 || var_len != op_len + 1 || operators[0] != '=' || !is_alpha_string(vars[0])) {
        printf("parse error\n");
        return 1;
    } else {
        int i;

        // make sure everything follows the rule of c language
        for (i = 0; i < var_len; i++) {
            if (!is_alpha_string(vars[i]) && !is_num_string(vars[i])) {
                printf("parse error\n");
                return 1;
            }
        }

        printf("The MIPS pseudocode is:\n");

        //declare the values these variebles save
        // such as:
        //   add       $s0      $s1                 $s2
        char op[4], result[20], var0[BUFSIZ >> 1], var1[BUFSIZ >> 1];
        char ** map = (char **)malloc(sizeof(char *)*var_len);
        for (i = 0; i < var_len; i++) {
            map[i] = (char *)malloc(sizeof(char)*(BUFSIZ >> 1));
        }

        for (i = 0; i < var_len; i++) {
            strcpy(vars[i], trim(vars[i], ' '));
        }

        strcpy(map[0], vars[0]);
        for (i = 1; i < var_len; i++) {
            memset(map[i], '\0', BUFSIZ >> 1);
        }

        int last_temp = -1;
        int last_var = 0;

        // from 1 becasue 0 is =
        for (i = 1; i < op_len; i++) {
            // find the command
            // If operator is '+',then command is add
            if (operators[i] == '+') {
                sprintf(op, "add");
            } 
            else if (operators[i] == '-')//If operator is '-', then command is sub
            {
                sprintf(op, "sub");
            }


            // find target address


            // if there is no the last operator ,use the temporary register
            //last_temp++;
            if (op_len >= 3 && i < op_len - 1) {
                sprintf(result, "$t%d", last_temp + 1);
            } 
            else //Last operator's address is $s0
            {
                sprintf(result, "$s0");
            }

            // Find register for two variables


            //the first operator is s1 and s2
            if (i == 1) {
                if (is_alpha_string(vars[1])) {
                    int j = 0;
                    int flag_increase = 1;
                    while (strlen(map[j]) != 0) {
                        if (strcmp(map[j], vars[1]) == 0) {
                            sprintf(var0, "$s%d", j);
                            flag_increase = 0;
                            break;
                        }
                        j++;
                    }
                    if (flag_increase) {
                        //printf("$s%d", ++last_var);
                        sprintf(var0, "$s%d", ++last_var);
                        strcpy(map[j], vars[1]);
                    }
                } else if (is_num_string(vars[1])) {
                    sprintf(var0, "%g", atof(vars[1]));
                }

                if (is_alpha_string(vars[2])) {
                    int j = 0;
                    int flag_increase = 1;
                    while (strlen(map[j]) != 0) {
                        if (strcmp(map[j], vars[2]) == 0) {
                            sprintf(var1, "$s%d", j);
                            flag_increase = 0;
                            break;
                        }
                        j++;
                    }
                    if (flag_increase) {
                        sprintf(var1, "$s%d", ++last_var);
                        strcpy(map[j], vars[2]);
                    }

                } else if (is_num_string(vars[2])) {
                    sprintf(var1, "%g", atof(vars[i + 1]));
                }

            } else  //Other Variables must be $tx,$sy
            {
                // temporary registers have no more than ten
                if (last_temp >= 10) {
                    clear_screen();
                    fprintf(stderr, "error,no enough temp registers\n");
                    return 1;
                }

                // common register have 8，but $s0 is used to store result
                if (last_var >= 7) {
                    clear_screen();
                    fprintf(stderr, "error,no enough registers\n");
                    return 1;
                }

                //last temporary register
                sprintf(var0, "$t%d", last_temp);

                //$sy
                if (is_alpha_string(vars[i + 1])) {
                    int j = 0;
                    int flag_increase = 1;
                    while (strlen(map[j]) != 0) {
                        if (strcmp(map[j], vars[i + 1]) == 0) {
                            sprintf(var1, "$s%d", j);
                            flag_increase = 0;
                            break;
                        }
                        j++;
                    }
                    if (flag_increase) {
                        sprintf(var1, "$s%d", ++last_var);
                        strcpy(map[j], vars[i + 1]);
                    }
                } else if (is_num_string(vars[i + 1])) {
                    sprintf(var1, "%g", atof(vars[i + 1]));
                }
            }
            ++last_temp;
            //print out
            printf("%s %s,%s,%s\n", op, result, var0, var1);
        }
    }

    return 0;
}
