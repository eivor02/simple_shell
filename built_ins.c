#include "shell.h"

int status;

/**
 * _setenv - Sets an environment variable.
 *
 * @name: Name of the variable.
 * @value: Value to set the variable to.
 *
 * Return: 0 on success.
 */
int _setenv(const char *name, const char *value)
{
	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		status = 2;
		return SKIP_FORK;
	}

	char *buffer = str_concat((char *)name, "=");
	char *element_ptr = get_array_element(environ, buffer);

	char *buf_tmp = str_concat(buffer, (char *)value);
	free(buffer);
	buffer = buf_tmp;

	if (element_ptr == NULL)
	{
		// Add the new environment variable to the environment array.
		char **new_environ = array_cpy(environ, list_len(environ, NULL) + 1);
		new_environ[list_len(environ, NULL) - 1] = buffer;
		new_environ[list_len(environ, NULL)] = NULL;
		free(environ);
		environ = new_environ;
	}
	else
	{
		// Update the value of the existing environment variable.
		free(environ[list_len(environ, (char *)name)]);
		environ[list_len(environ, (char *)name)] = buffer;
	}

	status = 0;
	return SKIP_FORK;
}

/**
 * _unsetenv - Unsets an environment variable.
 *
 * @name: Name of the variable.
 *
 * Return: 0 if successful.
 */
int _unsetenv(const char *name)
{
	char *buffer = str_concat((char *)name, "=");
	int len = list_len(environ, buffer);
	free(buffer);

	if (len == -1)
	{
		// The environment variable does not exist.
		write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
		status = 2;
		return SKIP_FORK;
	}

	// Remove the environment variable from the environment array.
	free(environ[len]);
	while (*(environ + len + 1) != NULL)
	{
		*(environ + len) = *(environ + len + 1);
		environ++;
	}
	*(environ + len) = NULL;

	status = 0;
	return SKIP_FORK;
}

/**
 * change_dir - Changes the current working directory.
 *
 * @name: Name of the directory to change to.
 *
 * Return: 0 if successful.
 */
int change_dir(char *name)
{
	char old_path_buffer[PATH_MAX];
	char new_path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;
	int i;

	getcwd(old_path_buffer, buf_size);

	if (name == NULL)
	{
		// Change to the home directory.
		char *home = get_array_element(environ, "HOME=");
		if (home == NULL)
		{
			status = 2;
			err_message("cd", name);
			return SKIP_FORK;
		}

		home += 5;

		i = chdir((const char *)home);
		if (i != -1)
		{
			_setenv("PWD", (const char *)home);
		}
	}
	else if (str_compare("-", name, MATCH) == TRUE)
	{
		// Change to the previous working directory.
		char *pwd = get_array_element(environ, "OLDPWD=");
		if (pwd == NULL)
		{
			status = 2;
			err_message("cd", name);
			return SKIP_FORK;
		}

		pwd += 7;

		i = chdir((const char *)pwd);
		if (i != -1)
		{
	  write(STDOUT_FILENO, pwd, _strlen
