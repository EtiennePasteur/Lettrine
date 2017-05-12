## paths
SRCDIR :=	.
INCDIR :=	.
OUTDIR :=	.

## compilation options
CXX :=		g++
INCFLAGS :=	$(addprefix -I,$(INCDIR))
CXXFLAGS :=	-W -Wall -Wextra -pedantic -std=c++14 \
		$(INCFLAGS)

## link options
LINKER :=	$(CXX)
LDFLAGS :=
LDLIBS :=	boost_system boost_filesystem pthread
LDLIBS :=	$(addprefix -l, $(LDLIBS))

## binary options
NAME :=		$(OUTDIR)/lettrine
SRC :=		main.cpp
SRC :=		$(addprefix $(SRCDIR)/,$(SRC))
OBJ :=		$(SRC:.cpp=.o)


#
# build rules
#

all:		$(NAME)

$(NAME):	$(OBJ)
		$(LINKER) -o $@ $^ $(LDFLAGS) $(LDLIBS)
		@printf "=== $@ BUILD COMPELTE ===\n\n"

%.o:		%.cpp
		$(CXX) -c $(CXXFLAGS) -o $@ $<


#
# clean rules
#

RM :=		@$(RM) -v

clean:;		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all


#
# special rules
#

.PHONY:		all clean fclean re
