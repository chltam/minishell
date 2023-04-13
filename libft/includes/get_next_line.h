/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:22:58 by cbadura           #+#    #+#             */
/*   Updated: 2023/03/23 16:33:29 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 12
# endif
# define MAX_SIZE 100000

char	*get_next_line(int fd);

size_t	ft_strlen(const char *s);
char	*ft_strcat(char *dest, char *src);
char	*ft_strcpy(char	*dest, char	*src);
char	*ft_strchr_gnl(const char *s, int c);
void	*ft_memset(void *b, int c, size_t len);

#endif
