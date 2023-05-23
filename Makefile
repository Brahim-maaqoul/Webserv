# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nhanafi <nhanafi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/23 22:47:45 by nhanafi           #+#    #+#              #
#    Updated: 2023/05/21 01:12:47 by nhanafi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98
SRC = main configfile/Location configfile/Config configfile/ServerPort utils/trim utils/remove request/Request Mimepars Server/Server\
	Server/Socket utils/isdir utils/generatehtml utils/get_date_time

INC = include

HEADER = include/Config.hpp include/Location.hpp include/Mimepars.hpp include/Request.hpp include/Server.hpp include/ServerPort.hpp include/Socket.hpp include/utils.hpp

ODIR = obj

OBJ = $(addprefix $(ODIR)/, $(SRC:=.o))

NAME = Server

all: $(NAME)

$(NAME): $(OBJ)
	@echo $(OBJ)
	@echo $(ODIR)/%.o
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(ODIR)/%.o: src/%.cpp $(HEADER)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

clean:
	rm -rf $(ODIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : re fclean clean all