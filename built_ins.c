#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 1024

int status;

/**
 * set_env - sets an environmental variable
 * @name: name of the variable
 * @value: value to set the variable to
 *
 * Return: 0 on success
 */
int set_env(const char *name, const char *value)
{
	char **new_environment;
	char *buffer;
	char *buf_tmp;
	char *element_ptr;
	int len;

	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		status = 2;
		return 1;
	}

	buffer = strdup(name);
	buffer = strcat(buffer, "=");

	element_ptr = getenv(buffer);

	buf_tmp = strdup(buffer);
	buf_tmp = strcat(buf_tmp, value);
	free(buffer);
	buffer = buf_tmp;

	if (element_ptr == NULL)
	{
		len = 0;
		while (environ[len] != NULL)
			len++;

		new_environment = malloc((len + 2) * sizeof(char *));
		for (int i = 0; i < len; i++)
			new_environment[i] = strdup(environ[i]);
		new_environment[len] = strdup(buffer);
		new_environment[len + 1] = NULL;

		free(environ);
		environ = new_environment;
		return 1;
	}

	len = 0;
	while (environ[len] != NULL)
	{
		if (strncmp(environ[len], name, strlen(name)) == 0)
		{
			free(environ[len]);
			environ[len] = strdup(buffer);
			return 1;
		}
		len++;
	}

	status = 0;
	return 1;
}

/**
 * unset_env - deletes an environmental variable
 * @name: name of variable
 *
 * Return: 0 if successful
 */
int unset_env(const char *name)
{
	char **env_ptr;
	char *buffer;
	int len;

	buffer = strdup(name);
	buffer = strcat(buffer, "=");
	len = 0;
	while (environ[len] != NULL)
	{
		if (strncmp(environ[len], buffer, strlen(buffer)) == 0)
		{
			free(environ[len]);
			env_ptr = &environ[len];
			while (*(env_ptr + 1) != NULL)
			{
				*env_ptr = *(env_ptr + 1);
				env_ptr++;
			}
			*env_ptr = NULL;
			return 1;
		}
		len++;
	}

	write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
	status = 2;
	return 1;
}

/**
 * change_directory - changes the current working directory
 * @name: name of directory to change to
 *
 * Return: 0 if successful
 */
int change_directory(char *name)
{
	char *home;
	char *pwd;
	char old_path_buffer[PATH_MAX];
	char new_path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;
	int i;

	getcwd(old_path_buffer, buf_size);

	if (name == NULL)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			status = 2;
			fprintf(stderr, "cd: no home directory\n");
			return 1;
		}

		i = chdir(home);
		if (i != -1)
			set_env("PWD", home);
	}
	else if (strcmp("-", name) == 0)
	{
		pwd = getenv("OLDPWD");
		if (pwd == NULL)
		{
			status = 2;
			fprintf(stderr, "cd: no previous directory\n");
			return 1;
		}

		i = chdir(pwd);
		if (i != -1)
		{
			printf("%s\n", pwd);
			set_env("PWD", pwd);
		}
	}
	else if (name != NULL)
	{
		i = chdir(name);
		if (i != -1)
			set_env("PWD", getcwd(new_path_buffer, buf_size));
	}

	if (i == -1)
	{
		status = 2;
		fprintf(stderr, "cd: no such file or directory: %s\n", name);
		return 1;
	}

	status = 0;
	set_env("OLDPWD", old_path_buffer);

	return 1;
}

/**
 * alias_command - handles command aliases
 * @args: arguments from command line
 * @to_free: indicate if aliases need to be freed (exiting shell)
 *
 * Return: 1 if exiting, 0 if the command is not "Sure! Here's a simple UNIX command line interpreter implemented in Python:

```python
import os

while True:
    command = input('$ ')
    try:
        os.system(command)
    except FileNotFoundError:
        print('Command not found')