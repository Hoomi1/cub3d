/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuuki <cyuuki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 19:33:43 by cyuuki            #+#    #+#             */
/*   Updated: 2021/04/19 20:02:18 by cyuuki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdlib.h>
#include "get_next_line.h"
#include <unistd.h>
#include <fcntl.h>
#include "ft_struct.h"
#include "../libft11/libft.h"
#include <stdio.h>
#include <math.h>
#define OBJ " 102WESN"
#define OBJJ "02WESN"

void	ft_colorf(t_all *all)
{
	//t_colors f_c;
	int r;
	int g;
	int b;

	r = all->colors.fbits_one << 1;
	g = all->colors.fbits_two << 8;
	b = all->colors.fbits_three << 0;
	all->colors.fbits_color = r | g | b;
}
void	ft_colorc(t_all *all)
{
	//t_colors c_c;
	int r;
	int g;
	int b;

	r = all->colors.cbits_one << 1;
	g = all->colors.cbits_two << 8;
	b = all->colors.cbits_three << 0;
	all->colors.cbits_color = r | g | b;
	//printf("%x", c_c.cbits_color);
}

void	my_mlx_pixel_put(t_win *data, int x, int y, int color)
{
	char	*dst;
	dst = data->addr + (y * data->line_l + x * (data->bpp / 8));
	*(unsigned int*)dst = color;
}

void	direction_player(char sym, t_all *dote)
{
	if (sym == 'N')
	{
		dote->plr.direction = 3 * M_PI_2;
	}
	if (sym == 'S')
	{
		dote->plr.direction = M_PI_2;
	}
	if (sym == 'W')
	{
		dote->plr.direction = M_PI;
	}
	if (sym == 'E')
	{
		dote->plr.direction = 0;
	}
}

void exit_error()
{
	write(2, "Error\n",6);
	write(2,"Error in the parser\n", 20);
}

int		my_pix_get(t_win *data, int x, int y)
{
	char	*dst;
	dst = data->addr + (y * data->line_l + x * (data->bpp / 8));
	return *(unsigned int*)dst;
}

void	ft_swap(t_all *all, int i, int j)
{
	float x;
	float y;
	float codirdis;

	codirdis = all->obj[i].distance;
	x = all->obj[i].x;
	y = all->obj[i].y;
	all->obj[i].distance = all->obj[j].distance;
	all->obj[i].x = all->obj[j].x;
	all->obj[i].y = all->obj[j].y;
	all->obj[j].distance = codirdis;
	all->obj[j].x = x;
	all->obj[j].y = y;
}


void	ft_sortingspr(int count, t_all *all)
{
	int i;
	int j;
	int max;
	int flag;

	i = 0;
	while (i < count)
	{
		max = all->obj[i].distance;
		flag = -1;
		j = i;
		while (++j < count)
		{
			if (all->obj[j].distance > max)
			{
				max = all->obj[j].distance;
				flag = j;
			}
		}
		if (flag != -1)
			ft_swap(all, i, flag);
		i++;
	}
}

void draw_sprite1(t_all *all, float x, float y, float dist)
{
	float teta;
	// float gamma;
	int scr_size;
	float i;
	float j;

	teta = atan2(y - all->plr.y, x - all->plr.x);
	while (teta - all->plr.direction > M_PI)
		teta -= 2 * M_PI;
	while (teta - all->plr.direction < -M_PI)
		teta += 2 * M_PI;
	if (2 * (teta - all->plr.direction) < -M_PI_2 || 2 * (teta - all->plr.direction) > M_PI_2)
		return ;
	scr_size = (int)(all->height / dist);
		all->coordin.proj_we = -cos(2 * (teta - all->plr.direction) + M_PI_2) * all->width + all->width / 2 - scr_size / 2;
	all->coordin.proj_height = all->height / 2 - scr_size / 2;
	all->coordin.ix = all->coordin.proj_we;
	while (all->coordin.ix < scr_size + all->coordin.proj_we)
	{
		if (all->coordin.ix >= 0 && all->coordin.ix < all->width && all->plr.c[(int)all->coordin.ix] >= dist)
		{
			i = (all->coordin.ix - all->coordin.proj_we) / ((float)scr_size / all->imgs.img_w);
			all->coordin.jy = all->coordin.proj_height;
			while (all->coordin.jy < scr_size + all->coordin.proj_height)
			{
				if (all->coordin.jy >= 0 && all->coordin.jy < all->height)
				{
					j = (all->coordin.jy - all->coordin.proj_height) / ((float)scr_size / all->imgs.img_h);
					all->coordin.color = my_pix_get(&all->imgs, i, j);
					if (all->coordin.color != 0)
						my_mlx_pixel_put(&all->win, all->coordin.ix, all->coordin.jy, all->coordin.color);
			}
			all->coordin.jy++;
			}
		}
		all->coordin.ix++;
	}
}

void	ft_parses_map2(int y, int x, t_all *all)
{
	static int count;
	float dx;
	float dy;


	all->obj[count].x = x + 0.5;
	all->obj[count].y = y + 0.5;
	// all->obj[count].dx = all->obj[count].x - all->plr.x;
	// all->obj[count].dy = all->obj[count].y - all->plr.y;
	//all->obj[count].dir =  pow(dx, 2) + pow(dy, 2);
	//draw_sprite1(all, all->obj[count].x, all->obj[count].y);
	// printf("%f, %f\n", all->plr.x, all->plr.y);
	// printf("%f, %f\n", all->obj[count].x, all->obj[count].y);
	// printf("!!!!!!!%f, %f\n", all->obj[count].dx, all->obj[count].dy);
	count++;
	all->count = count;
}

void	ft_parses_map(int i, int j, char sym, t_all *len)
{
	if ((sym == 'W' || sym == 'N' || sym == 'E' || sym == 'S'))
	{
		direction_player(sym, len);
		len->plr.x = j + 0.5;
		len->plr.y = i + 0.5;
		len->plr.flag = len->plr.flag + 1;
	}
	if (len->plr.flag == 1)
	{
		printf("error");
		return ;
	}
	if (sym == '2')
	{
		len->plr.place_two++;
	}
	//printf("%f", len->plr->y);
	//printf("%d", app.place_two);
	//printf("%c\n", *map[0]);
}

char	*ft_spacex(char *line, int max)
{
	int		size;
	char	*str;
	int i;

	i = 0;
	str = NULL;
	size = ft_strlen(line);
	str = (char *)malloc(1 + max);
	ft_memcpy(str, line, size);
	while (size < max)
	{
		str[size] = ' ';
		size++;
	}
	str[max] = '\0';
	return (str);
}

void	make_map3(t_all *len)
{
	int		i;
	int		j;
	char	sym;

	i = -1;
	j = -1;
	len->obj = (t_map *)malloc(sizeof(t_map) * len->plr.place_two);
	while (++i < len->y)
	{
		while (++j < ft_strlen(len->map[i]))
		{
			sym = len->map[i][j];
			if (ft_strrchr("2", sym))
			{

				ft_parses_map2(i, j, len);
			}
		}
		j = -1;
	}
}

void	make_map2(t_all *len)
{
	int		i;
	int		j;
	char	sym;

	j = -1;
	i = -1;
	while (++i < len->y)
	{
		while (++j < ft_strlen(len->map[i]))
		{
			sym = len->map[i][j];
			if (!(ft_strrchr(OBJ, sym)))
			{
				printf("error");//ошибка
				break ;
			}
			if (ft_strrchr(OBJJ, sym))
				(((len->map[i - 1][j] != ' ') && (len->map[i + 1][j] != ' ')
				&& (len->map[i][j + 1] != ' ') && (len->map[i][j - 1] != ' '))
				? ft_parses_map(i, j, len->map[i][j], len) : printf("error"));
		}
		j = -1;
	}
	make_map3(len);
}

char	**make_map(t_list **head, int size, t_all *len, int max)
{
	//char	**map;
	int		i;
	int		j;
	char	sym;
	char	*line;
	//t_plr	plr;
	t_list	*tmp;

	len->map = ft_calloc(size + 1, sizeof(char *));
	i = -1;
	j = -1;
	tmp = *head;
	len->plr.flag = -1;
	len->plr.place_two = 0;
	while (tmp)
	{
		line = tmp->content;
		line = ft_spacex(line, max);
		len->map[++i] = line;
		tmp = tmp->next;
		len->y = i;
	}
	//i = -1;
	make_map2(len);
	len->plr.c = malloc(sizeof(float) * len->width);
	// while (++i < len->y)
	// {
	// 	while (++j < ft_strlen(len->map[i]))
	// 	{
	// 		sym = len->map[i][j];
	// 		if (!(ft_strrchr(OBJ, sym)))
	// 		{
	// 			printf("error");//ошибка
	// 			//printf("|%c|", sym);
	// 			break ;
	// 		}
	// 		if (ft_strrchr(OBJJ, sym))
	// 		{
	// 			//printf("|%c|", sym);
	// 			(((len->map[i - 1][j] != ' ') && (len->map[i + 1][j] != ' ')
	// 			&& (len->map[i][j + 1] != ' ') && (len->map[i][j - 1] != ' '))
	// 			? ft_parses_map(i, j, len->map[i][j], len) : printf("error"));
	// 			//printf("%f, %f", dot.x, dot.y);
	// 		}

	// 	}
	// 	//printf("|%s|\n", len->map[i]);
	// 	//printf("\n");
	// 	j = -1;
	// 		//ft_putendl_fd(map[i]);
	// }
	// //printf("%d\n", len->plr.place_two);
	// i = -1;
	// j = -1;
	// len->obj = (t_map *)malloc(sizeof(t_map) * len->plr.place_two);
	// // len->obj = '\0';
	// while (++i < len->y)
	// {
	// 	while (++j < ft_strlen(len->map[i]))
	// 	{
	// 		sym = len->map[i][j];
	// 		if (ft_strrchr("2", sym))
	// 		{

	// 			ft_parses_map2(i, j, len);
	// 		}
	// 	}
	// 	j = -1;
	// }
//	printf("%f\n", len->obj.x);
	return (len->map);
}

void	ft_parser_r(t_all *len, char *map)
{
	char *str;

	len->len_r = 1;
	len->sum = len->len_r;
	map++;
	len->width = ft_atoi(map);
	str = ft_itoa(len->width);
	while (*str != '\0')
	{
		map++;
		str++;
	}
	map++;
	len->height = ft_atoi(map);
	if (len->width < 1 || len->height < 1)
		return ;// тут ошибку надо написать
	((len->w_width < len->width) ? len->width = len->w_width
	: len->width);
	((len->w_height < len->height) ? len->height = len->w_height
	: len->height);
}

void	ft_parser_no(t_all *len, char *map)
{
	len->len_no = 1;
	len->sum = len->sum + len->len_no;
	map = map + 2;
	len->textur_no = *ft_split(map, ' ');
	printf("|%s|\n", len->textur_no);
	if (!(len->imgno.img = mlx_xpm_file_to_image(len->win.mlx,
	len->textur_no, &len->imgno.img_w, &len->imgno.img_h)))
		printf("%s\n", len->imgno.img);//return ;//ошибка какая-то
	len->imgno.addr = mlx_get_data_addr(len->imgno.img,
	&len->imgno.bpp, &len->imgno.line_l, &len->imgno.en);
}

void	ft_parser_so(t_all *len, char *map)
{
	len->len_so = 1;
	len->sum = len->sum + len->len_so;
	map = map + 2;
	len->textur_so = *ft_split(map, ' ');
	if (!(len->imgso.img = mlx_xpm_file_to_image(len->win.mlx,
	len->textur_so, &len->imgso.img_w, &len->imgso.img_h)))
		return ;//ошибка какая-то
	len->imgso.addr = mlx_get_data_addr(len->imgso.img,
	&len->imgso.bpp, &len->imgso.line_l, &len->imgso.en);
}

void	ft_parser_we(t_all *len, char *map)
{
	if (len->len_we != -1)
		return ; // ошибка
	len->len_we = 1;
	len->sum = len->sum + len->len_we;
	map = map + 2;
	len->textur_we = *ft_split(map, ' ');
	if (!(len->imgwe.img = mlx_xpm_file_to_image(len->win.mlx,
	len->textur_we, &len->imgwe.img_w, &len->imgwe.img_h)))
		return ;//ошибка какая-то
	len->imgwe.addr = mlx_get_data_addr(len->imgwe.img,
	&len->imgwe.bpp, &len->imgwe.line_l, &len->imgwe.en);
}

void	ft_parser_ea(t_all *len, char *map)
{
	if (len->len_ea != -1)
		return ;// ошибка
	len->len_ea = 1;
	len->sum = len->sum + len->len_ea;
	map = map + 2;
	len->textur_ea = *ft_split(map, ' ');
	if (!(len->imgea.img = mlx_xpm_file_to_image(len->win.mlx,
	len->textur_ea, &len->imgea.img_w, &len->imgea.img_h)))
		return ;//ошибка какая-то
	len->imgea.addr = mlx_get_data_addr(len->imgea.img,
	&len->imgea.bpp, &len->imgea.line_l, &len->imgea.en);
}

void	ft_parser_s(t_all *len, char *map)
{
	if (len->len_s != -1)
		return ; // ошибка
	len->len_s = 1;
	len->sum = len->sum + len->len_s;
	map++;
	len->textur_s = *ft_split(map, ' ');
	if (!(len->imgs.img = mlx_xpm_file_to_image(len->win.mlx,
	len->textur_s, &len->imgs.img_w, &len->imgs.img_h)))
		return ;//ошибка какая-то
	len->imgs.addr = mlx_get_data_addr(len->imgs.img,
	&len->imgs.bpp, &len->imgs.line_l, &len->imgs.en);
}

void	ft_fpars(t_all *len)
{
	if (len->colors.fbits_one > 255 ||
	len->colors.fbits_two > 255 || len->colors.fbits_three > 255)
		return ;// ошибка
	if (len->colors.fbits_one < 0 ||
	len->colors.fbits_two < 0 || len->colors.fbits_three < 0)
		return ;//ошибка
}

void	ft_parser_f(t_all *len, char *map)
{
	char *str;

	if (len->len_f != -1)
		return ; //ошибка
	len->len_f = 1;
	len->sum = len->sum + len->len_f;
	map++;
	len->colors.fbits_one = ft_atoi(map);
	str = ft_strchr(map, ',');
	map = str;
	map++;
	len->colors.fbits_two = ft_atoi(map);
	str = ft_strchr(map, ',');
	map = str;
	map++;
	len->colors.fbits_three = ft_atoi(map);
	str = "";
	str = ft_strchr(map, ',');
	if (str != '\0')
		return ;//ошибка
	ft_fpars(len);
	// if (len->colors.fbits_one > 255 ||
	// len->colors.fbits_two > 255 || len->colors.fbits_three > 255)
	// 	return ;// ошибка
	// if (len->colors.fbits_one < 0 ||
	// len->colors.fbits_two < 0 || len->colors.fbits_three < 0)
	// 	return ;//ошибка
	ft_colorf(len);
}

void	ft_cpars(t_all *len)
{
	if (len->colors.cbits_one > 255 ||
	len->colors.cbits_two > 255 || len->colors.cbits_three > 255)
		return ;// ошибка
	if (len->colors.cbits_one < 0 ||
	len->colors.cbits_two < 0 || len->colors.cbits_three < 0)
		return ;//ошибка
}

void	ft_parser_c(t_all *len, char *map)
{
	char *str;

	if (len->len_c != -1)
		return ;//ошибка
	len->len_c = 1;
	len->sum = len->sum + len->len_f;
	map++;
	len->colors.cbits_one = ft_atoi(map);
	str = ft_strchr(map, ',');
	map = str;
	map++;
	len->colors.cbits_two = ft_atoi(map);
	str = ft_strchr(map, ',');
	map = str;
	map++;
	len->colors.cbits_three = ft_atoi(map);
	str = "";
	str = ft_strchr(map, ',');
	if (str != '\0')
		return ;//ошибка
	ft_cpars(len);
	// if (len->colors.cbits_one > 255 ||
	// len->colors.cbits_two > 255 || len->colors.cbits_three > 255)
	// 	return ;// ошибка
	// if (len->colors.cbits_one < 0 ||
	// len->colors.cbits_two < 0 || len->colors.cbits_three < 0)
	// 	return ;//ошибка
	ft_colorc(len);
}

void	ft_parser_map(t_all *len, char *map)
{
	char *str;

	str = NULL;
	if (*map == ' ')
	{
		while (*map != '\0')
		{
			if (*map == '1')
			{
				len->flag = 1;
				break ;
			}
			map++;
		}
	}
	if (*map == '1')
		len->flag = 1;
}

void	ft_parses(char *map, t_all *len)
{
	char	*str;
	//t_imgs ms_img;

	mlx_get_screen_size(len->win.mlx, &len->w_width, &len->w_height);
	// printf("%c\n", *map);
	// printf("%d", len->len_r);
	if (*map == 'R' && len->len_r == -1)
	{
		ft_parser_r(len, map);
		// len->len_r = 1;
		// len->sum = len->len_r;
		// map++;
		// len->width = ft_atoi(map);
		// str = ft_itoa(len->width);
		// while (*str != '\0')
		// {
		// 	map++;
		// 	str++;
		// }
		// map++;
		// len->height = ft_atoi(map);
		// if (len->width < 1 || len->height < 1)
		// 	return ;// тут ошибку надо написать
		// ((len->w_width < len->width) ? len->width = len->w_width
		// : len->width);
		// ((len->w_height < len->height) ? len->height = len->w_height
		// : len->height);
		//  printf("1!!!!!!!!!!!!\n");
		//printf("%d____%d", znach.width, znach.height);
	}
	else if ((*map == 'N' && *(map + 1) == 'O') && len->len_no == -1)
	{
		ft_parser_no(len, map);
		// len->len_no = 1;
		// //len->imgno.mlx = mlx_init();
		// len->sum = len->sum + len->len_no;
		// map = map + 2;
		// len->textur_no = *ft_split(map, ' ');
		// printf("|%s|\n", len->textur_no);
		// if (!(len->imgno.img = mlx_xpm_file_to_image(len->win.mlx,
		// len->textur_no, &len->imgno.img_w, &len->imgno.img_h)))
		// 	printf("%s\n", len->imgno.img);//return ;//ошибка какая-то
		// len->imgno.addr = mlx_get_data_addr(len->imgno.img,
		// &len->imgno.bpp, &len->imgno.line_l, &len->imgno.en);

	}
	else if ((*map == 'S' && *(map + 1) == 'O') && len->len_so == -1)
	{
		ft_parser_so(len, map);
		// len->len_so = 1;
		// len->sum = len->sum + len->len_so;
		// map = map + 2;
		// len->textur_so = *ft_split(map, ' ');
		// if (!(len->imgso.img = mlx_xpm_file_to_image(len->win.mlx,
		// len->textur_so, &len->imgso.img_w, &len->imgso.img_h)))
		// 	return ;//ошибка какая-то
		// len->imgso.addr = mlx_get_data_addr(len->imgso.img,
		// &len->imgso.bpp, &len->imgso.line_l, &len->imgso.en);
		// //printf("3!!!!!!!!!!!!\n");
	}
	else if (*map == 'W' && *(map + 1) == 'E')
	{
		ft_parser_we(len, map);
		// if (len->len_we != -1)
		// 	return ; // ошибка
		// len->len_we = 1;
		// len->sum = len->sum + len->len_we;
		// map = map + 2;
		// len->textur_we = *ft_split(map, ' ');
		// if (!(len->imgwe.img = mlx_xpm_file_to_image(len->win.mlx,
		// len->textur_we, &len->imgwe.img_w, &len->imgwe.img_h)))
		// 	return ;//ошибка какая-то
		// len->imgwe.addr = mlx_get_data_addr(len->imgwe.img,
		// &len->imgwe.bpp, &len->imgwe.line_l, &len->imgwe.en);
		// //printf("4!!!!!!!!!!!!\n");
	}
	else if (*map == 'E' && *(map + 1) == 'A')
	{
		ft_parser_ea(len, map);
		// if (len->len_ea != -1)
		// 	return ;// ошибка
		// len->len_ea = 1;
		// len->sum = len->sum + len->len_ea;
		// map = map + 2;
		// len->textur_ea = *ft_split(map, ' ');
		// if (!(len->imgea.img = mlx_xpm_file_to_image(len->win.mlx,
		// len->textur_ea, &len->imgea.img_w, &len->imgea.img_h)))
		// 	return ;//ошибка какая-то
		// len->imgea.addr = mlx_get_data_addr(len->imgea.img,
		// &len->imgea.bpp, &len->imgea.line_l, &len->imgea.en);
		//printf("5!!!!!!!!!!!!\n");
	}
	else if (*map == 'S')
	{
		ft_parser_s(len, map);
		// if (len->len_s != -1)
		// 	return ; // ошибка
		// len->len_s = 1;
		// len->sum = len->sum + len->len_s;
		// map++;
		// len->textur_s = *ft_split(map, ' ');
		// if (!(len->imgs.img = mlx_xpm_file_to_image(len->win.mlx,
		// len->textur_s, &len->imgs.img_w, &len->imgs.img_h)))
		// 	return ;//ошибка какая-то
		// len->imgs.addr = mlx_get_data_addr(len->imgs.img,
		// &len->imgs.bpp, &len->imgs.line_l, &len->imgs.en);
		// //printf("6!!!!!!!!!!!!\n");
	}
	else if (*map == 'F')
	{
		ft_parser_f(len, map);
	// 	if (len->len_f != -1)
	// 		return ; //ошибка
	// 	len->len_f = 1;
	// 	len->sum = len->sum + len->len_f;
	// 	map++;
	// 	len->colors.fbits_one = ft_atoi(map);
	// 	str = ft_strchr(map, ',');
	// 	map = str;
	// 	//printf("1@@@%s\n", map);
	// 	map++;
	// 	len->colors.fbits_two = ft_atoi(map);
	// 	str = ft_strchr(map, ',');
	// 	map = str;
	// 	//printf("1@@@%s\n", map);
	// 	map++;
	// 	len->colors.fbits_three = ft_atoi(map);
	// 	str = "";
	// 	str = ft_strchr(map, ',');
	// 	if (str != '\0')
	// 		return ;//ошибка
	// 	if (len->colors.fbits_one > 255 ||
	// 	len->colors.fbits_two > 255 || len->colors.fbits_three > 255)
	// 		return ;// ошибка
	// 	if (len->colors.fbits_one < 0 ||
	// 	len->colors.fbits_two < 0 || len->colors.fbits_three < 0)
	// 		return ;//ошибка
	// 	ft_colorf(len);
	// // 	printf("7!!!!!!!!!!!!\n");
	// // 	printf("%d\n", f_colors.fbits_one);
	// // 	printf("%d\n", f_colors.fbits_two);
	// // 	printf("%d\n", f_colors.fbits_three);
	}
	else if (*map == 'C')
	{
		ft_parser_c(len, map);
		// if (len->len_c != -1)
		// 	return ;//ошибка
		// len->len_c = 1;
		// len->sum = len->sum + len->len_f;
		// map++;
		// len->colors.cbits_one = ft_atoi(map);
		// str = ft_strchr(map, ',');
		// map = str;
		// //printf("1@@@%s\n", map);
		// map++;
		// len->colors.cbits_two = ft_atoi(map);
		// str = ft_strchr(map, ',');
		// map = str;
		// //printf("1@@@%s\n", map);
		// map++;
		// len->colors.cbits_three = ft_atoi(map);
		// str = "";
		// str = ft_strchr(map, ',');
		// if (str != '\0')
		// 	return ;//ошибка
		// if (len->colors.cbits_one > 255 ||
		// len->colors.cbits_two > 255 || len->colors.cbits_three > 255)
		// 	return ;// ошибка
		// if (len->colors.cbits_one < 0 ||
		// len->colors.cbits_two < 0 || len->colors.cbits_three < 0)
		// 	return ;//ошибка
		// ft_colorc(len);
	}

	else if (len->sum == 8 /*&& (*map == '1' || *map == ' ')*/)
	{
		ft_parser_map(len, map);
		// str = NULL;
		// if (*map == ' ')
		// {
		// 	while (*map != '\0')
		// 	{
		// 		if (*map == '1')
		// 		{
		// 			len->flag = 1;
		// 			break ;
		// 		}
		// 		map++;
		// 	}
		// }
		// if (*map == '1')
		// {
		// 	len->flag = 1;
		// }
	}

	//printf("%d\n", len->flag);
	// else if (len->sum > 8)
	// {
	// 	exit(1);
	// }
}

// int		my_pix_get(t_win *data, int x, int y)
// {
// 	char	*dst;
// 	dst = data->addr + (y * data->line_l + x * (data->bpp / 8));
// 	return *(unsigned int*)dst;
// }

void	my_bit_images_no(t_all *all, float start, int y, float h)
{
	float pix;

	pix = h / all->imgno.img_h;
	all->y_tex = ((float)y - start) / pix;
}
void	my_bit_images_so(t_all *all, float start, int y, float h)
{
	float pix;

	pix = h / all->imgso.img_h;
	all->y_tex = ((float)y - start) / pix;
}

void	my_bit_images_we(t_all *all, float start, int y, float h)
{
	float pix;

	pix = h / all->imgwe.img_h;
	all->y_tex = ((float)y - start) / pix;
}

void	my_bit_images_ea(t_all *all, float start, int y, float h)
{
	float pix;

	pix = h / all->imgea.img_h;
	all->y_tex = ((float)y - start) / pix;
}



void	wall_3d(t_all *all, float h, float i, float x_w, float y_h)
{
	float	start;
	float	end;
	int		y;
	int color;
	int k;

	k = 0;
	y = 0;
	start = all->height / 2 - h / 2;
	end = all->height / 2 + h / 2;
	while (y < start)
	{
		my_mlx_pixel_put(&all->win, i, y, all->colors.fbits_color);
		y++;
	}
	while (y < all->height && y < end)
	{
		if (all->plr.angel < 0)
			all->plr.angel += 2 * M_PI;
		if (all->plr.angel > 2 * M_PI)
			all->plr.angel -= 2 * M_PI;
		if (all->flagno == -4)
		{
			//printf("%f\n", all->plr.direction);
			if (all->plr.angel >= 0 && all->plr.angel <= M_PI)
			{
				x_w = x_w - (int)x_w;
				all->x_tex = x_w * all->imgso.img_w;
				my_bit_images_so(all, start, y, h);
				color = my_pix_get(&all->imgso, all->x_tex, all->y_tex);
				my_mlx_pixel_put(&all->win, i, y, color);
				//my_mlx_pixel_put(&all->win, i, y, 0xFFFF00);
			}
			else if (all->plr.angel >= M_PI || all->plr.angel <= 2 * M_PI)
			{
				x_w = x_w - (int)x_w;
				all->x_tex = x_w * all->imgno.img_w;
				my_bit_images_no(all, start, y, h);
				color = my_pix_get(&all->imgno, all->x_tex, all->y_tex);
				my_mlx_pixel_put(&all->win, i, y, color);
				//my_mlx_pixel_put(&all->win, i, y, 0x0000FF);
			}
		}
		else if (all->flagno == 4)
		{
			if (all->plr.angel >= M_PI_2 && all->plr.angel <= 3 * M_PI_2)
			{
				y_h = y_h - (int)y_h;
				all->x_tex = y_h * all->imgwe.img_w;
				my_bit_images_we(all, start, y, h);
				color = my_pix_get(&all->imgwe, all->x_tex, all->y_tex);
				my_mlx_pixel_put(&all->win, i, y, color);
				//my_mlx_pixel_put(&all->win, i, y, 0xCC0000);
			}
			else if (all->plr.angel >= 3 * M_PI_2 || all->plr.angel <= 9.42)
			{
				y_h = y_h - (int)y_h;
				all->x_tex = y_h * all->imgea.img_w;
				my_bit_images_ea(all, start, y, h);
				color = my_pix_get(&all->imgea, all->x_tex, all->y_tex);
				my_mlx_pixel_put(&all->win, i, y, color);
				//my_mlx_pixel_put(&all->win, i, y, 0x00FF66);
			}
		}

		//my_mlx_pixel_put(&all->win, i, y, 0xFF0000);
		y++;
	}
	//printf("%f\n", all->plr.angel);
	while (y < all->height)
	{
		my_mlx_pixel_put(&all->win, i, y, all->colors.cbits_color);
		y++;
	}

}



void		plr_luch(t_all *all, float x_p, float y_p)
{
	//float fov;
	float x;
	float y;
	float c;
	float step;
	int i;
	float a;

	all->plr.c[i] = 0;
	all->fov = 0.5F;
	i = 0;
	x = 0;
	all->plr.angel = all->plr.direction - all->fov / 2;
	step = all->fov / all->width;
	while (i < all->width)
	{
		all->plr.c[i] = 0;
		x = x_p + all->plr.c[i] * cos(all->plr.angel);
		y = y_p + all->plr.c[i] * sin(all->plr.angel);
		// while (all->map[(int)y][(int)x] != '1')
		// {
		// 	x = x_p + c * cos(angel);
		// 	y = y_p + c * sin(angel);
		// 	c += 0.005F;
		// 	my_mlx_pixel_put(&all->win, x, y, 0xFFFF00);
		// }
		while (1)
		{
			x = x_p + all->plr.c[i] * cos(all->plr.angel);
			if (all->map[(int)y][(int)x] == '1') // W E
			{
				all->flagno = 4;
				break ;
			}
			y = y_p + all->plr.c[i] * sin(all->plr.angel);
			if (all->map[(int)y][(int)x] == '1') // S N
			{
				all->flagno = -4;
				break ;
			}
			all->plr.c[i] += 0.005F;
			my_mlx_pixel_put(&all->win, x, y, 0xFFFF00);
		}
		all->plr.angel += step;
		if (all->plr.c[i] > 0)
			a = all->height / (all->plr.c[i] * cos(all->plr.angel - all->plr.direction));
		wall_3d(all, a, i, x, y);
			i++;
	}
	int k = 0;
	int g = 0;
	if(all->count > 1)
	{
	while(++g < all->count)
		all->obj[g].distance = sqrt(pow(all->plr.x - all->obj[g].x, 2) + pow(all->plr.y - all->obj[g].y, 2));
	ft_sortingspr(all->count, all);
	while(++k < all->count)
	{
//		a = all->height / (all->plr.c[i] * cos(all->plr.angel - all->plr.direction));

		draw_sprite1(all, all->obj[k].x, all->obj[k].y, all->obj[k].distance);
	}
	}
	else
	{
		all->obj[k].distance = sqrt(pow(all->plr.x - all->obj[k].x, 2) + pow(all->plr.y - all->obj[k].y, 2));
		ft_sortingspr(all->count, all);
		draw_sprite1(all, all->obj[k].x, all->obj[k].y, all->obj[k].distance);
	}
	//printf("%f", a);
	//return (c);
}

int		key(int keycode, t_all *all)
{
	printf("%f, %f\n", all->plr.x, all->plr.y);
	int j;
	int i;
	float c;

	i = -1;
	j = -1;
	if (keycode == 123)
	{
		all->plr.direction -= 0.1;
		while (++i < (all->y + 1) * 1)
		{
			while (++j < ft_strlen(all->map[i / 1]) * 1)
			{
				if (all->map[i / 1][j / 1] == '1')
				{
					my_mlx_pixel_put(&all->win, j, i, 0x00FF0000);
				}
				else
					my_mlx_pixel_put(&all->win, j, i, 0x00000000);
			}
			j = -1;
		}
	}
	if (keycode == 124)
	{
		all->plr.direction += 0.1;
		while (++i < (all->y + 1) * 1)
		{
			while (++j < ft_strlen(all->map[i / 1]) * 1)
			{
				if (all->map[i / 1][j / 1] == '1')
				{
					my_mlx_pixel_put(&all->win, j, i, 0x00FF0000);
				}
				else
					my_mlx_pixel_put(&all->win, j, i, 0x00000000);
			}
			j = -1;
		}
	}
	if (keycode == 13)
	{
		while (++i < (all->y + 1) * 1)
		{
			while (++j < ft_strlen(all->map[i / 1]) * 1)
			{
				if (all->map[i / 1][j / 1] == '1')
				{
					my_mlx_pixel_put(&all->win, j, i, 0x00FF0000);
				}
				else
					my_mlx_pixel_put(&all->win, j, i, 0x00000000);
			}
			j = -1;
		}
		all->plr.y += sin(all->plr.direction);
		all->plr.x += cos(all->plr.direction);
		if (all->map[(int)all->plr.y][(int)all->plr.x] != '1')
		{
			my_mlx_pixel_put(&all->win, all->plr.x * 1,
		all->plr.y * 1, 0x000FF00);
		}
		else
		{
			all->plr.y -= sin(all->plr.direction);
			all->plr.x -= cos(all->plr.direction);
			my_mlx_pixel_put(&all->win,
			all->plr.x * 1, all->plr.y * 1, 0x000FF00);
		}
	}
	if (keycode == 1)
	{
		while (++i < (all->y + 1) * 1)
		{
			while (++j < ft_strlen(all->map[i / 1]) * 1)
			{
				if (all->map[i / 1][j / 1] == '1')
				{
					my_mlx_pixel_put(&all->win, j, i, 0x00FF0000);
				}
				else
					my_mlx_pixel_put(&all->win, j, i, 0x00000000);
			}
			j = -1;
		}
		all->plr.y -= sin(all->plr.direction);
		all->plr.x -= cos(all->plr.direction);
		if (all->map[(int)all->plr.y][(int)all->plr.x] != '1')
			my_mlx_pixel_put(&all->win, all->plr.x * 1,
		all->plr.y * 1, 0x000FF00);
		else
		{
			all->plr.y += sin(all->plr.direction);
			all->plr.x += cos(all->plr.direction);
			my_mlx_pixel_put(&all->win,
			all->plr.x * 1, all->plr.y * 1, 0x000FF00);
		}
	}
	if (keycode == 0)
	{
		while (++i < (all->y + 1) * 1)
		{
			while (++j < ft_strlen(all->map[i / 1]) * 1)
			{
				if (all->map[i / 1][j / 1] == '1')
				{
					my_mlx_pixel_put(&all->win, j, i, 0x00FF0000);
				}
				else
					my_mlx_pixel_put(&all->win, j, i, 0x00000000);
			}
			j = -1;
		}
		all->plr.x += sin(all->plr.direction);
		all->plr.y -= cos(all->plr.direction);
		if (all->map[(int)all->plr.y][(int)all->plr.x] != '1')
		{
			my_mlx_pixel_put(&all->win,
			all->plr.x * 1, all->plr.y * 1, 0x000FF00);
		}
		else
		{
			all->plr.x -= sin(all->plr.direction);
			all->plr.y += cos(all->plr.direction);
			my_mlx_pixel_put(&all->win,
			all->plr.x * 1, all->plr.y * 1, 0x000FF00);
		}

	}
	if (keycode == 2)
	{
		while (++i < (all->y + 1) * 1)
		{
			while (++j < ft_strlen(all->map[i / 1]) * 1)
			{
				if (all->map[i / 1][j / 1] == '1')
				{
					my_mlx_pixel_put(&all->win, j, i, 0x00FF0000);
				}
				else
					my_mlx_pixel_put(&all->win, j, i, 0x00000000);
			}
			j = -1;
		}
		all->plr.x -= sin(all->plr.direction);
		all->plr.y += cos(all->plr.direction);
		if (all->map[(int)all->plr.y][(int)all->plr.x] != '1')
			my_mlx_pixel_put(&all->win,
		all->plr.x * 1, all->plr.y * 1, 0x000FF00);
		else
		{
			all->plr.x += sin(all->plr.direction);
			all->plr.y -= cos(all->plr.direction);
			my_mlx_pixel_put(&all->win,
			all->plr.x * 1, all->plr.y * 1, 0x000FF00);
		}
	}
	plr_luch(all, all->plr.x, all->plr.y);
	//wall_3d(all, c);
	//mlx_put_image_to_window(all->win.mlx, all->win.win, all->win.img, 0, 0);
	mlx_put_image_to_window(all->win.mlx, all->win.win, all->win.img, 0, 0);
	return (0);
}

void	draw_map(t_all *map)
{
	int j;
	int x;

	j = -1;
	x = -1;
	map->win.mlx = mlx_init();
	map->win.win = mlx_new_window(map->win.mlx, map->width, map->height, "cub3D");
	map->win.img = mlx_new_image(map->win.mlx, map->width, map->height);
	map->win.addr = mlx_get_data_addr(map->win.img,
	&map->win.bpp, &map->win.line_l, &map->win.en);

	// while (++x < (map->y + 1) * 1)
	// {
	// 	while (++j < ft_strlen(map->map[x / 1]) * 1)
	// 	{
	// 		if (map->map[x / 1][j / 1] == '1')
	// 		{
	// 			my_mlx_pixel_put(&map->win, j, x, 0x00FF0000);
	// 		}
	// 		if (map->map[x / 1][j / 1] == 'N')
	// 		{
	// 			my_mlx_pixel_put(&map->win, j, x, 0x000FF00);
	// 		}
	// 		else if (map->map[x / 1][j / 1] == 'S')
	// 		{
	// 			my_mlx_pixel_put(&map->win, j, x, 0x000FF00);
	// 		}
	// 		if (map->map[x / 1][j / 1] == 'W')
	// 		{
	// 			my_mlx_pixel_put(&map->win, j, x, 0x000FF00);
	// 		}
	// 		if (map->map[x / 1][j / 1] == 'E')
	// 		{
	// 			my_mlx_pixel_put(&map->win, j, x, 0x000FF00);
	// 		}
	// 		if (map->map[x / 1][j / 1] == '2')
	// 		{
	// 			my_mlx_pixel_put(&map->win, j, x, 0xCC0000);
	// 		}
	// 	}
	// 	j = -1;
	// }
	plr_luch(map, map->plr.x, map->plr.y);
	mlx_hook(map->win.win, 2, 1L << 0, key, map);
	mlx_put_image_to_window(map->win.mlx, map->win.win, map->win.img, 0, 0);
	mlx_loop(map->win.mlx);
}

void		ft_col(int max, t_all *len, t_list *head, char *line)
{
	while (get_next_line(len->fd, &line))
	{
		if (ft_strlen(line) > max)
			max = ft_strlen(line);
		ft_lstadd_back(&head, ft_lstnew(line));
	}
}

void	ft_param(t_all *len)
{
	len->flag = 0;
	len->len_r = -1;
	len->len_we = -1;
	len->len_no = -1;
	len->len_ea = -1;
	len->len_s = -1;
	len->len_so = -1;
	len->len_c = -1;
	len->len_f = -1;
}

void	drawbmp(t_all *all,int width,int height, t_drwbmp *bmp)
{
	bmp->file_ps = (4 - (all->width * 3) % 4) % 4;
	bmp->size_d = all->width * all->height * 3 + all->height * bmp->file_ps;
	bmp->file = ft_calloc(14,1);
	bmp->file[0] = 'B';
	bmp->file[1] = 'M';
	
}

void	new_bmp(t_all *all)
{
	t_drwbmp bmp;

	bmp.open = open("screen_cub.bmp", O_CREAT | O_WRONLY | O_TRUNC,
		S_IREAD | S_IWRITE);
	plr_luch(all, all->plr.x, all->plr.y);
	drawbmp(all, all->width, all->height, &bmp);
}

int		main(int argc, char **argv)
{
	t_list *head;
	t_all	len;
	int max;
	char *line;

	len.win.mlx = mlx_init();
	ft_param(&len);
	len.fd = open(argv[1], O_RDONLY);
	line = NULL;
	head = NULL;
	while (get_next_line(len.fd, &line))
	{
		len.str_first = line;
		ft_parses(line, &len);
		if (len.flag == 1)
			break ;
	}
	if (len.flag == 0)
		printf("error");
	if (ft_strlen(len.str_first) > max)
		max = ft_strlen(len.str_first);
	ft_lstadd_front(&head, ft_lstnew(len.str_first));
	ft_col(max, &len, head, line);
	// while (get_next_line(len.fd, &line))
	// {
	// 	if (ft_strlen(line) > max)
	// 		max = ft_strlen(line);
	// 	ft_lstadd_back(&head, ft_lstnew(line));
	// }
	make_map(&head, ft_lstsize(head), &len, max);

	draw_map(&len);
if (argc == 3)
	{
		new_bmp(&len);
	}
}
