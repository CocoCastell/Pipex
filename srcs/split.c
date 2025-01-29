/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/29 14:14:05 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft_b/includes/libft.h"
#include <stdio.h>

size_t  ft_strlcpy(char *dest, const char *src, size_t size)
  {
     size_t  i;
	 size_t  len;
 
      len = 0;
      i = 0;
     while (src[len] != '\0')
          len++;
	 if (size > 0)
     {
          while (src[i] != '\0' && i < size - 1)
          {
             dest[i] = src[i];
             i++;
          }
          dest[i] = '\0';
      }
      return (len);
  }


void	find_quotes(char *command)
{
	char	*first_quote;
	char	*second_quote;
	int		str_length;
	char	*string_to_copy;
	
	first_quote = NULL;
	second_quote = NULL;
	string_to_copy = NULL;
	while (*command != '\0')
	{
		if (*command == '\'' && first_quote == NULL)
		{
			first_quote = command;
			command++;
		}
		if (*command == '\'' && second_quote == NULL)
		{
			second_quote = command;
			break ;
		}
		command++;
	}
	if (*command == '\0')
	{
		printf("empty string\n");
		return ;
	}
	str_length = second_quote - first_quote;
	string_to_copy = malloc(sizeof(char *) *str_length + 1);
	ft_strlcpy(string_to_copy, ++first_quote, str_length);
	printf("str: %s\n", string_to_copy);
}

void	temp(char *command)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while(command[i] != '\0')
	{
		if (command[i] == '\'')
			count++;
		i++;
	}
	if(count == 1 || count > 2)
		return ;
	if (count == 2)
	{
		find_quotes(command);
	}
}

int	main()
{
	char *command = "grep 'bon dia'";
	temp(command);
	return 0;
}
