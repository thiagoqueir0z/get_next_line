/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:13:12 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/04 15:34:08 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Looks for a newline character in the given linked list.

int	found_newline(t_list *stash)
{
	int	i;
	t_list	current;

	if (stash == NULL)
		return (0);
	
	current = stash;
	while (current)
	{
		i = 0;
		while (current->content[i])
		{
			if (current->content[i] == '\0')
				return (1);
			i++;
		}
		current = current->next;
	}
	return (0);
}

// Returns a pointer to the last node in stash

t_list	*ft_lst_get_last(t_list *stash)
{
	t_list	*current;

	current = stash;

	if (current == NULL)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return(current);	
}

// Frees the entire stash

void	free_stash(t_list *stash)
{
	t_list *next;
	while (stash)
	{
		next = stash->next;
		if (stash->content)
			free(stash->content);
		free(stash);
		stash = next;
	}
}





// Returns length of a string

int	ft_strlen (const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (i);
}


