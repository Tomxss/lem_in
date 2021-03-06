











#include "../../includes/lem_in.h"

static void	error_in_cross_check(t_lemin *l)
{
	free(l->sum);
	ft_memdel((void**)&l->string_file);
	ft_free_double_array((void**)l->f);
	ft_free_double_array((void**)l->dup);
	ft_free_double_array((void**)l->pipes);
	free_lst_name(l);
	print_error(ft_putendl_fd,
			"\033[091mError During Allocation\033[0m",
			STDERR_FILENO, l);
}

static void	cut_paths(t_lemin *l, int room, int *level)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < l->nb_rooms)
	{
		if (l->pipes[room][i] == 1)
		{
			j = i;
			while (++j < l->nb_rooms)
			{
				if (l->pipes[room][j] == 1)
				{
					if (l->pipes[i][j] == 1 && level[room] != level[i] &&
							i != l->room_start && i != l->room_end &&
							j != l->room_start && j != l->room_end)
					{
						l->pipes[i][j] = 0;
						l->pipes[j][i] = 0;
					}
				}
			}
		}
	}
}

static void	init_cross_check(t_lemin *l, t_queue **queue, int room_start)
{
	int	i;

	i = 0;
	l->visited = (bool*)malloc(sizeof(bool) * (unsigned long)l->nb_rooms);
	if (!l->visited)
		error_in_cross_check(l);
	l->level = (int*)malloc(sizeof(int) * (unsigned long)l->nb_rooms);
	if (!l->level)
		error_in_cross_check(l);
	while (i < l->nb_rooms)
	{
		l->level[i] = -1;
		l->visited[i++] = 0;
	}
	if (*queue == NULL)
		*queue = (t_queue*)malloc(sizeof(t_queue));
	if (!*queue)
		error_in_cross_check(l);
	(*queue)->id = room_start;
	(*queue)->next = NULL;
}

static void	compose_queue(t_lemin *l, t_queue **queue, t_queue **begin, int *k)
{
	int	j;

	j = 0;
	while (j < l->nb_rooms)
	{
		if (l->pipes[(*begin)->id][j] == 1 && l->visited[j] == 0)
		{
			(*queue)->next = (t_queue*)malloc(sizeof(t_queue));
			(*queue) = (*queue)->next;
			(*queue)->next = NULL;
			(*queue)->id = j;
			l->visited[(*queue)->id] = 1;
			l->level[(*queue)->id] = *k;
		}
		j++;
	}
	if ((*begin)->next)
		*begin = (*begin)->next;
	if (l->level[(*begin)->id] == *k)
		(*k)++;
}

void		cross_check(t_lemin *l, int room_start)
{
	int		k;
	int		end;
	t_queue	*queue;
	t_queue	*begin;
	t_queue	*begin_begin;

	k = 0;
	end = 0;
	queue = NULL;
	init_cross_check(l, &queue, room_start);
	l->level[room_start] = k++;
	begin = queue;
	begin_begin = begin;
	end = (room_start == l->room_start) ? l->room_end : l->room_start;
	while (begin && begin->id != end)
	{
		l->visited[begin->id] = 1;
		cut_paths(l, begin->id, l->level);
		compose_queue(l, &queue, &begin, &k);
	}
	free(l->visited);
	free(l->level);
	free_queue(begin_begin);
}
