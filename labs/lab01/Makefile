CC := gcc
CFLAGS = -Wall -Wreturn-local-addr -Wno-unused-variable

DIR := $(abspath ./)
SRCS := $(wildcard $(DIR)/*.c -or $(DIR)/*.cc)

EX ?= ex1

SRC = $(foreach src, $(SRCS), $(if $(findstring $(EX), $(src)), $(src), ))
DST = $(notdir $(basename $(SRC)))

$(EX):
	@$(CC) $(CFLAGS) -o $(DIR)/build/$(DST) $(SRC)
	@$(DIR)/build/$(DST)

git:
	git add .
	git commit -m "Finish lab01"