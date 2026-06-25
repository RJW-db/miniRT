#include "parsing.h"

//	Static Functions
static int	*group_numbers(const char prefix);
static bool	one_member_group(char **line);
static bool	three_member_group(char **line);

bool	check_line(char *line, const char prefix, uint8_t nbr_of_groups)
{
	uint8_t		group_index;
	const int	*prefix_nbr = group_numbers(prefix);
	static bool	(*group_ptr[2])(char **) = {
		one_member_group,
		three_member_group
	};

	group_index = 0;
	while (*line != '\0' && *line != '#')
	{
		if (ft_isnum(line) == false)
			return (false);
		if (group_ptr[((*prefix_nbr) == 3)](&line) == false)
			return (false);
		++group_index;
		++prefix_nbr;
		line += skip_spaces(line);
	}
	if (prefix != 'L')
		return (group_index == nbr_of_groups);
	return (group_index == 3 || group_index == 5);
}

static int	*group_numbers(const char prefix)
{
	static int	groups[6][5] = {
		{1, 3, 0, 0, 0},	// 'A'
		{3, 3, 1, 0, 0},	// 'C'
		{3, 1, 3, 1, 3},	// 'L'
		{3, 1, 3, 0, 0},	// 'sp'
		{3, 3, 3, 0, 0},	// 'pl'
		{3, 3, 1, 1, 3}		// 'cy'
	};

	return (groups[find_char("ACLspcy", prefix)]);
}

static bool	one_member_group(char **line)
{
	char	*tmp;

	tmp = *line;
	tmp += skip_signed_digits(tmp);
	if (*tmp == '.')
	{
		++tmp;
		if (ft_isnum(tmp) == true)
			tmp += skip_signed_digits(tmp);
		else
			return (false);
	}
	*line = tmp;
	return (true);
}

static bool	three_member_group(char **line)
{
	uint8_t	iterate_twice;
	char	*tmp;

	tmp = *line;
	tmp += skip_signed_digits(tmp);
	iterate_twice = 0;
	while (iterate_twice < 2)
	{
		if (one_member_group(&tmp) == false)
			return (false);
		if (tmp[0] == ',' && tmp[1] != '.')
			++tmp;
		else
			return (false);
		if (tmp[0] == '\0' || tmp[0] == '\n')
			return (false);
		++iterate_twice;
	}
	if (one_member_group(&tmp) == false)
		return (false);
	*line = tmp;
	return (true);
}
