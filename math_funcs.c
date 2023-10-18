#include "shell.h"

/**
 * _itoa - Converts an int into a string.
 *
 * @n: Int to be converted.
 *
 * Return: The converted number.
 */
char *_itoa(int n)
{
	char *buf = malloc(12);
	char *ptr = buf;
	int is_min = FALSE;
	int i_mask = 1000000000;
	int digit = 0;

	// Check for the minimum integer.
	if (n == INT_MIN)
	{
		*ptr = '-';
		ptr++;
		n = INT_MAX;
		is_min = TRUE;
	}

	// Check for negative integers.
	if (n < 0)
	{
		*ptr = '-';
		ptr++;
		n = -n;
	}

	// Skip leading zeros.
	while (i_mask > 9 && digit == 0)
	{
		digit = n / i_mask;
		n %= i_mask;
		i_mask /= 10;
	}

	// Write the first non-zero digit.
	if (digit != 0)
	{
		*ptr = digit + '0';
		ptr++;
	}

	// Write the remaining digits.
	while (i_mask > 9)
	{
		digit = n / i_mask;
		*ptr = digit + '0';
		ptr++;
		n %= i_mask;
		i_mask /= 10;
	}

	// If the number is negative, add a minus sign.
	if (is_min == TRUE)
	{
		n += 1;
	}

	// Write the last digit.
	*ptr = n + '0';
	ptr++;

	// Terminate the string.
	*ptr = '\0';

	return buf;
}

/**
 * _atoi - Converts a string into a number.
 *
 * @s: String to be converted.
 *
 * Return: The converted number.
 */
int _atoi(char *s)
{
	int n = 0;
	int sign = 1;
	int s_int;

	// Skip the equals sign if it is present at the beginning of the string.
	if (*s == '=' && *(s + 1) >= '0' && *(s + 1) <= '9')
	{
		sign = -1;
		s++;
	}

	// Iterate over the string and convert each character to an integer.
	while (*s != '\0')
	{
		if (*s >= '0' && *s <= '9')
		{
			s_int = *s - 48;
			if (sign == 1)
			{
				n = (n * 10) + s_int;
			}
			else
			{
				n = (n * 10) - s_int;
			}
		}
		else
		{
			return -1;
		}
		s++;
	}

	return n;
}
