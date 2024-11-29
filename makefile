# ----------------------------
# Makefile Options
# ----------------------------

NAME = SNAKE
DESCRIPTION = "snek eat appl"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz
MAKE_GFX = cd $(GFXDIR) && $(CONVIMG)

# ----------------------------

include $(shell cedev-config --makefile)
