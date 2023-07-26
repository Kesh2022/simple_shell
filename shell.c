_strcpy.c




#include "shell.h"


/**
 * _strcpy - copies the string pointed to by src,
 * including the terminating null byte (\0),
 * to the buffer pointed to by dest
 * @dest: copy source to this buffer
 * @src: this is the source to copy
 * Return: copy of original source
 */


char *_strcpy(char *dest, char *src)
{
        int i, len;


        for (len = 0; src[len] != '\0'; len++)
                ;


        for (i = 0; i <= len; i++)
                dest[i] = src[i];


        return (dest);
}












_strdup.c


#include "shell.h"


/**
 * _strdup - returns a pointer to a newly allocated space in memory,
 * which contains a copy of the string given as a parameter
 * @str: string to duplicate
 * Return: pointer to duplicated string in allocated memory
 */
char *_strdup(char *str)
{
        char *duplicate_str;
        int i, len = 0;


        if (str == NULL) /* validate str input */
                return (NULL);


        while (*(str + len))
                len++;
        len++; /* add null terminator to length */


        duplicate_str = malloc(sizeof(char) * len); /* allocate memory */
        if (duplicate_str == NULL)
                return (NULL);


        i = 0;
        while (i < len)
        {
                *(duplicate_str + i) = *(str + i);
                i++;
        }


        return (duplicate_str);
}




_strtok.c




#include "shell.h"


/**
 * t_strlen - returns token's string length for mallocing
 * @str: a token
 * @pos: index position in user's command typed into shell
 * @delm: delimeter (e.g. " ");
 * Return: token length
 */
int t_strlen(char *str, int pos, char delm)
{
        int len = 0;


        while ((str[pos] != delm) && (str[pos] != '\0'))
        {
                pos++;
                len++;
        }
        return (len);
}


/**
 * t_size - returns number of delim ignoring continuous delim
 * @str: user's command typed into shell
 * @delm: delimeter (e.g. " ");
 * Return: number of delims so that (num token = delims + 1)
 */
int t_size(char *str, char delm)
{
        int i = 0, num_delm = 0;


        while (str[i] != '\0')
        {
                if ((str[i] == delm) && (str[i + 1] != delm))
                {
                        /* handle continuous delims */
                        num_delm++;
                }
                if ((str[i] == delm) && (str[i + 1] == '\0'))
                {
                        /*handle continuous delims after full command */
                        num_delm--;
                }
                i++;
        }
        return (num_delm);
}


/**
 * ignore_delm - returns a version of string without preceeding delims
 * @str: string
 * @delm: delimiter (e.g. " ")
 * Return: new string (e.g. "    ls -l" --> "ls -l")
 */
char *ignore_delm(char *str, char delm)
{
        while (*str == delm)
                str++;
        return (str);
}


/**
 * _str_tok - tokenizes a string and returns an array of tokens
 * @str: user's command typed into shell
 * @delm: delimeter (e.g. " ");
 * Return: an array of tokens (e.g. {"ls", "-l", "/tmp"}
 */
char **_str_tok(char *str, char *delm)
{
        int buffsize = 0, p = 0, si = 0, i = 0, len = 0, se = 0, t = 0;
        char **toks = NULL, d_ch;


        d_ch = delm[0];
        /* creates new version of string ignoring all delims infront*/
        str = ignore_delm(str, d_ch);
        /* malloc ptrs to store array of tokens (buffsize + 1), and NULL ptr */
        buffsize = t_size(str, d_ch);
        toks = malloc(sizeof(char *) * (buffsize + 2));
        if (toks == NULL)
                return (NULL);
        while (str[se] != '\0')        /* find string ending index */
                se++;
        while (si < se)
        { /* malloc lengths for each token ptr in array */
                if (str[si] != d_ch)
                {
                        len = t_strlen(str, si, d_ch);
                        toks[p] = malloc(sizeof(char) * (len + 1));
                        if (toks[p] == NULL)
                                return (NULL);
                        i = 0;
                        while ((str[si] != d_ch) && (str[si] != '\0'))
                        {
                                toks[p][i] = str[si];
                                i++;
                                si++;
                        }
                        toks[p][i] = '\0'; /* null terminate at end*/
                        t++;
                }
                /* handle repeated delimeters; increment ptr after ("ls __-l")*/
                if (si < se && (str[si + 1] != d_ch && str[si + 1] != '\0'))
                        p++;
                si++;
        }
        p++;
        toks[p] = NULL; /* set last array ptr to NULL */
        return (toks);
}




Env_linked_list.c


#include "shell.h"


/**
 * env_linked_list - creates a linked list from environmental variables
 * @env: environmental variables
 * Return: linked list
 */
list_t *env_linked_list(char **env)
{
        list_t *head;
        int i = 0;


        head = NULL;
        while (env[i] != NULL)
        {
                add_end_node(&head, env[i]);
                i++;
        }
        return (head);
}


/**
 * _env - prints environmental variables
 * @str: user's command into shell (i.e. "env")
 * @env: environmental variables
 * Return: 0 on success
 */
int _env(char **str, list_t *env)
{
        free_double_ptr(str); /* frees user input */
        print_list(env); /* prints env */
        return (0);
}




free_double_ptr.c


#include "shell.h"


/**
 * free_double_ptr - free malloced arrays
 * @str: array of strings
 */
void free_double_ptr(char **str)
{
        int i = 0;


        while (str[i] != NULL)
        {
                free(str[i]);
                i++;
        }
        free(str);
}


get_env.c


#include "shell.h"


/**
 * c_strdup - custom string duplication; excludes beginning bytes
 * @str: string to duplicate (e.g. environmental variable PATH=/bin:/bin/ls)
 * @cs: number of bytes to exclude (e.g. excludes "PATH=")
 * Return: string (e.g. /bin:/bin/ls)
 */
char *c_strdup(char *str, int cs)
{
        char *duplicate_str;
        int i, len = 0;


        if (str == NULL) /* validate str input */
                return (NULL);


        /* calculate len + null terminator to malloc */
        while (*(str + len))
                len++;
        len++;


        /* allocate memory but exclude environmental variable title (PATH) */
        duplicate_str = malloc(sizeof(char) * (len - cs));
        if (duplicate_str == NULL)
                return (NULL);


        i = 0;
        while (i < (len - cs))
        {
                *(duplicate_str + i) = *(str + cs + i);
                i++;
        }
        return (duplicate_str);
}


/**
 * get_env - finds and returns a copy of the requested environmental variable
 * @str: string to store it in
 * @env: entire set of environmental variables
 * Return: copy of requested environmental variable
 */
char *get_env(char *str, list_t *env)
{
        int j = 0, cs = 0;


        while (env != NULL)
        {
                j = 0;
                while ((env->var)[j] == str[j]) /* find desired env variable */
                        j++;
                if (str[j] == '\0' && (env->var)[j] == '=')
                        break;
                env = env->next;
        }


        while (str[cs] != '\0') /* find how many bytes in env variable title */
                cs++;
        cs++; /*counts 1 more for = sign*/
        return (c_strdup(env->var, cs)); /* make a copy of variable w/o title */
}




int_to_string.c




#include "shell.h"


/**
 * numlen - counts number of 0s in a tens power number
 * @n: number
 * Return: returns count of digits
 */
int numlen(int n)
{
        int count = 0;
        int num = n;


        while (num > 9 || num < -9)
        {
                num /= 10;
                count++;
        }
        return (count);
}
/**
 * int_to_string - turns an int into a string
 * @number: int
 * Return: returns the int as a string. returns NULL if failed
 */


char *int_to_string(int number)
{
        int digits, tens, i = 0, t = 0, x;
        char *res;


        digits = number;
        tens = 1;


        if (number < 0)
                t = 1;
        res = malloc(sizeof(char) * (numlen(digits) + 2 + t));
        if (res == NULL)
                return (NULL);
        if (number < 0)
        {
                res[i] = '-';
                i++;
        }
        for (x = 0; digits > 9 || digits < -9; x++)
        {
                digits /= 10;
                tens *= 10;
        }
        for (digits = number; x >= 0; x--)
        {
                if (digits < 0)
                {
                        res[i] = (digits / tens) * -1 + '0';
                        i++;
                }
                else
                {
                        res[i] = (digits / tens) + '0';
                        i++;
                }
                digits %= tens;
                tens /= 10;
        }
        res[i] = '\0';
        return (res);
}


linked_lists.c


#include "shell.h"


/**
 * print_list - _print linked list
 * @h: linked list
 * Return: size of linked list
 */
size_t print_list(list_t *h)
{
        list_t *c_list = h;
        int count = 0;
        int c = 0;


        if (h == NULL)
                return (0);
        while (c_list != NULL)
        {
                if (c_list->var == NULL)
                {
                        write(STDOUT_FILENO, "(nil)", 5);
                        write(STDOUT_FILENO, "\n", 1);
                }
                else
                {
                        c = 0;
                        while ((c_list->var)[c] != '\0')
                                c++;
                        write(STDOUT_FILENO, c_list->var, c);
                        write(STDOUT_FILENO, "\n", 1);
                }
                c_list = c_list->next;
                count++;
        }
        return (count);
}


/**
 * add_end_node - add node to end of linked list
 * @head: pointer to head of linked list
 * @str: data to new node
 * Return: pointer to new linked list
 */
list_t *add_end_node(list_t **head, char *str)
{
        list_t *new;
        list_t *holder;


        if (head == NULL || str == NULL)
                return (NULL); /* check if address of head is null */
        new = malloc(sizeof(list_t));
        if (new == NULL)
                return (NULL);


        new->var = _strdup(str);
        new->next = NULL;


        holder = *head;
        if (holder != NULL)
        {
                while (holder->next != NULL)
                {
                        holder = holder->next;
                }
                holder->next = new;
        }
        else
        {
                *head = new;
        }
        return (*head);
}


/**
 * delete_nodeint_at_index - removing node at index
 * @head: input head address
 * @index: input index
 * Return: 1 if success, -1 if fail
 */
int delete_nodeint_at_index(list_t **head, int index)
{
        list_t *n_head;
        list_t *holder;
        int count = 0;


        if (*head == NULL)
                return (-1);
        if (index == 0)
        {
                holder = (*head)->next;
                free((*head)->var);
                free(*head);
                *head = holder;
                return (1);
        }
        count = 1;
        n_head = *head;
        while (count < index)
        {
                if (n_head == NULL)
                        return (-1);
                n_head = n_head->next;
                count++;
        }
        holder = n_head->next;
        n_head->next = holder->next;
        free(holder->var);
        free(holder);
        return (1);
}


/**
 * free_linked_list - frees linked list
 * @list: linked list
 */
void free_linked_list(list_t *list)
{
        list_t *holder;


        while (list != NULL)
        {
                holder = list;
                list = list->next;
                free(holder->var);
                free(holder);
        }


}


main.c
#include "shell.h"


/**
 * main - creates a simple shell
 * @ac: argument count
 * @av: argument vectors
 * @env: environmental variables
 * Return: 0 on success
 */
int main(int ac, char **av, char **env)
{
        (void)ac;
        (void)av;


        prompt(env);


        return (0);
}




non_interactive.c


#include "shell.h"


/**
 * c_ignore - custom ignores spaces and newlines
 * (e.g. echo "ls\n ls" | ./a.out)
 * @str: envrionmental variables
 * Return: new string
 */
char *c_ignore(char *str)
{
        while (*str == ' ' || *str == '\n')
                str++;
        return (str);
}


/**
 * non_interactive - handles when user pipes commands into shell via pipeline
 * (e.g. echo "ls/nls -al/n" | ./a.out)
 * @env: envrionmental variables
 */
void non_interactive(list_t *env)
{
        size_t i = 0, n = 0;
        int command_line_no = 0, exit_stat = 0;
        char *command = NULL, *n_command = NULL, **n_line, **token;


        i = get_line(&command);
        if (i == 0)
        {
                free(command);
                exit(0);
        }
        n_command = command;
        command = c_ignore(command);
        n_line = _str_tok(command, "\n"); /* tokenize each command string */
        if (n_command != NULL)
                free(n_command);
        n = 0;
        while (n_line[n] != NULL)
        {
                command_line_no++;
                token = NULL; /* tokenize each command in array of commands */
                token = _str_tok(n_line[n], " ");
                exit_stat = built_in(token, env, command_line_no, n_line);
                if (exit_stat)
                {
                        n++;
                        continue;
                }
                exit_stat = _execve(token, env, command_line_no);
                n++;
        }
        free_double_ptr(n_line);
        free_linked_list(env);
        exit(exit_stat);
}
