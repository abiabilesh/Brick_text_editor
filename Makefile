PROJECTDIR = $(CURDIR)
TARGETDIR = $(PROJECTDIR)/build

#Naming the executable....
EXECUTABLE = brick
INCLUDES = $(CURDIR)/include

DIR = core system file 
#Creating a list of source directories for all the folders within the project)
SOURCEDIR += $(CURDIR)
SOURCEDIR += $(foreach dir, $(DIR), $(addprefix $(PROJECTDIR)/,$(dir)))

#Creating a list of build directories for each of the source file within the project
OBJECTDIR += $(TARGETDIR)
OBJECTDIR += $(foreach dir, $(DIR), $(addprefix $(TARGETDIR)/,$(dir)))


#Creating a list of source files from the source directories
SOURCES += $(foreach dir, $(SOURCEDIR), $(wildcard $(dir)/*.c))

#Creating a list of object files from the source files
OBJECTS += $(subst $(PROJECTDIR), $(TARGETDIR), $(SOURCES:.c=.o))

# Define dependencies files for all objects
DEPS = $(OBJECTS:.o=.d)

#Adding compiler info and flags for the build
CC = gcc
RM = rm -rf 
RMDIR = rm -rf 
MKDIR = mkdir -p
ERRIGNORE = 2>/dev/null
SEP=/

CC_FLAGS = -g -Wall \
	   -Werror=implicit-function-declaration

PTHREADS = -lpthread
VERBOSE = TRUE
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
    HIDE =  
else
    HIDE = @
endif

define generateRules
$(1)/%.o : %.c
	@echo Building $$@ from $$<
	$(HIDE)$(CC) -c $(CC_FLAGS) -I $(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<)
endef


.PHONY: all directories clean 

#Default rule for building the complete project
all: directories $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(HIDE)echo Linking $@
	$(HIDE) $(CC)  $(OBJECTS) -o $(TARGETDIR)/$(EXECUTABLE) $(PTHREADS)

#Generating rules
$(foreach objectdir, $(OBJECTDIR), $(eval $(call generateRules, $(objectdir))))

#Remove all objects, dependencies and executable files generated during the build
clean:
	@echo Cleaning the build directory as per requested...
	$(HIDE) $(RMDIR) $(OBJECTDIR)
	@echo Cleaning done!!

directories:
	@echo Creating directories for the output...
	$(HIDE) $(MKDIR) $(OBJECTDIR)
	@echo Done Creating build directories...

