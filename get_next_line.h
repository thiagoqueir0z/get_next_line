/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:14:44 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/04 13:32:51 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE

# ifndef BUFFER_SIZE
# define BUFFER_SIZE
# endif

#include <unistd.h>
#include <stdlib.h>

typedef struct s_list
{
	int		fd;
	char		*content;
	struct s_list	*next;
}	t_list;

int	ft_strlen(const char *str);
t_list	*ft_lst_get_last(t_list *stash);
int	found_newline(t_list *stash);
void	free_stash(t_list *stash);
int	ft_strlen(const char *str);
t_list	*ft_lst_get_last(t_list *stash);
int	found_newline(t_list *stash);
void	free_stash(t_list *stash);
char	*get_next_line(int fd);

# endif
