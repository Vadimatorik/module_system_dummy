ifndef MODULE_SYSTEM_DUMMY_OPTIMIZATION
	MODULE_SYSTEM_DUMMY_OPTIMIZATION = -g3 -O0
endif

MODULE_SYSTEM_DUMMY_CPP_FILE			:= $(shell find module_system_dummy/ -maxdepth 5 -type f -name "*.cpp" )
MODULE_SYSTEM_DUMMY_DIR					:= $(shell find module_system_dummy/ -maxdepth 5 -type d -name "*" )
MODULE_SYSTEM_DUMMY_PATH				:= $(addprefix -I, $(MODULE_SYSTEM_DUMMY_DIR))
MODULE_SYSTEM_DUMMY_OBJ_FILE			:= $(addprefix build/obj/, $(MODULE_SYSTEM_DUMMY_CPP_FILE))
MODULE_SYSTEM_DUMMY_OBJ_FILE			:= $(patsubst %.cpp, %.o, $(MODULE_SYSTEM_DUMMY_OBJ_FILE))
MODULE_SYSTEM_DUMMY_OBJ_FILE			:= $(patsubst %.c, %.o, $(MODULE_SYSTEM_DUMMY_OBJ_FILE))

build/obj/module_system_dummy/%.o:	module_system_dummy/%.cpp
	@echo [CPP] $<
	@mkdir -p $(dir $@)
	@$(CPP) $(CPP_FLAGS) $(DEFINE_PROJ) $(MODULE_SYSTEM_DUMMY_PATH) $(MODULE_SYSTEM_DUMMY_OPTIMIZATION) -c $< -o $@


PROJECT_PATH			+= $(MODULE_SYSTEM_DUMMY_PATH)
PROJECT_OBJ_FILE		+= $(MODULE_SYSTEM_DUMMY_OBJ_FILE)