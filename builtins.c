#include "shell.h"

int status;

/**
 * _set_or_unset_env - sets or unsets an environmental variable.
 *
 * @name: Name of the variable.
 * @value: Value to set the variable to, or NULL to unset the variable.
 * @to_unset: Whether to unset the variable.
 *
 * Return: 0 on success.
 */
int _set_or_unset_env(const char *name, const char *value, bool to_unset)
{
	char **env_ptr;
	char *buffer;
	int len;

	buffer = str_concat((char *)name, "=");
	len = list_len(environ, buffer);
	free(buffer);

	if (len == -1)
	{
		// The environment variable does not exist.
		if (to_unset)
		{
			write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
			status = 2;
			return SKIP_FORK;
		}
		else
		{
			// Create the new environment variable.
			char **new_environ = array_cpy(environ, list_len(environ, NULL) + 1);
			new_environ[list_len(environ, NULL) - 1] = str_concat((char *)name, "=");
			new_environ[list_len(environ, NULL)] = NULL;
			free_array(environ);
			environ = new_environ;
		}
	}
	else
	{
		env_ptr = environ + len;
		if (to_unset)
		{
			// Unset the environment variable.
			free(*env_ptr);
			while (*(env_ptr + 1) != NULL)
			{
				*env_ptr = *(env_ptr + 1);
				env_ptr++;
			}
			*env_ptr = NULL;
		}
		else
		{
			// Set the environment variable.
			free(*env_ptr);
			*env_ptr = str_concat((char *)name, "=");
		}
	}

	status = 0;
	return SKIP_FORK;
}

/**
 * _setenv - sets and environmental variable
 * @name: name of the variable
 * @value: value to set the variable to
 *
 * Return: 0 on success
 */
int _setenv(const char *name, const char *value)
{
	return _set_or_unset_env(name, value, false);
}

/**
 * _unsetenv - deletes an environmental variable
 * @name: name of variable
 *
 * Return: 0 if successful
 */
int _unsetenv(const char *name)
{
	return _set_or_unset_env(name, NULL, true);
}

/**
 * change_dir - changes the current working directory
 * @name: name of directory to change to
 *
 * Return: 0 if successful
 */
int change_dir(char *name)
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
		home = get_array_element(environ, "HOME=");
		if (home == NULL)
		{
			status = 2;
			err_message("cd", name);
			return SKIP_FORK;
		}

		home += 5;

		i = chdir((const char *)home);
		if (i != -1)
			_setenv("PWD", (const char *)home);
	}
	else if (str_compare("-", name, MATCH) == TRUE)
	{
		pwd = get_array_element(environ, "OLDPWD=");
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
			write(STDOUT_FILENO, pwd, _strlen(pwd));
			write(STDOUT_FILENO, "\n", 1);
