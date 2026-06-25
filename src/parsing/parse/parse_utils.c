#include "parsing.h"
#include "mathRT.h"

/**
 * @brief Skips till it encounters a new numerical value.
 * @param str Ptr in string to skip from.
 * @return char* to the first character of the next value in str,
 * or \0 if it encountered no whitespace.
 * @note If the given *str is pointing to a value (digit, period or minus),
 * it will also skip that until it finds the next separate value.
 */
char	*nxtv(char *str)
{
	while (*str && ft_isspace(*str) == false && *str != ',')
		++str;
	while (*str && *str != '.' && *str != '-' && !ft_isdigit(*str))
		++str;
	return (str);
}

/**
 * @brief Skips till it encounters a new numerical value.
 * @param str Address of ptr in string to skip from.
 * @return char* to the first character of the next value in str,
 * or \0 if it encountered no whitespace.
 * @note If the given *str is pointing to a value (digit, period or minus),
 * it will also skip that until it finds the next separate value.
 * @note Also changes the original ptr given under the address **str!
 */
char	*nxtvp(char **str)
{
	char	*c;

	c = *str;
	while (*c && ft_isspace(*c) == false && *c != ',')
		++c;
	while (*c && *c != '.' && *c != '-' && !ft_isdigit(*c))
		++c;
	*str = c;
	return (c);
}

float	rt_atof(const char *str)
{
	float	num;
	int		i;
	int		sign;
	int		place;

	num = 0.0F;
	i = 0;
	sign = 1;
	while (ft_isdigit(str[i]) == false || str[i] == '.')
		++i;
	if (i > 0 && str[i - 1] == '-')
		sign = -1;
	while (str[i] != '\0' && ft_isdigit(str[i]) == true)
		num = (num * 10.0F) + (str[i++] - '0');
	if (str[i] != '.')
		return (num * (float)sign);
	++i;
	place = 1;
	while (str[i] != '\0' && ft_isdigit(str[i]) == true)
		num += (float)(str[i++] - '0') / (float)pow(10.0, (double)(place++));
	return (num * (float)sign);
}

bool	validate_orientation(t_vec4 *or, char **line)
{
	uint8_t	i;

	i = 0;
	while (i <= 2)
	{
		(*or)[i] = rt_atof(nxtvp(line));
		if ((*or)[i] < -1.0F || (*or)[i] > 1.0F)
		{
			return (false);
		}
		++i;
	}
	(*or)[i] = 0.0F;
	*or = vnorm(*or);
	return (true);
}

bool	validate_and_normalize_color(t_vec4 *color, char **line)
{
	int32_t	color_value;
	uint8_t	i;

	i = 0;
	while (i <= 2)
	{
		color_value = atoi32(nxtvp(line));
		if (color_value < 0 || color_value > 255)
			return (false);
		(*color)[i] = (float)color_value / 255.0F;
		++i;
	}
	(*color)[i] = 1.0F;
	return (true);
}
