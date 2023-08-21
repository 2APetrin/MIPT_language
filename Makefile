FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
ENUM  = -Wswitch-enum


frontend_files  = frontend/main.cpp frontend/source/tokenizer/tokenizer.cpp frontend/source/main_frontend/frontend.cpp frontend/source/tree/log_tree.cpp frontend/source/tree/tree.cpp frontend/source/get/get.cpp file_work/file_work.cpp
middleend_files = middleend/main.cpp middleend/source/create_ast/create_ast.cpp file_work/file_work.cpp frontend/source/main_frontend/frontend.cpp frontend/source/tree/log_tree.cpp frontend/source/tree/tree.cpp frontend/source/get/get.cpp middleend/source/simplifier/ast_simplify.cpp middleend/source/ast_to_assembly/ast_to_assembly.cpp
cpu_files       = backend/main.cpp backend/source/cpu/cpu.cpp backend/source/cpu/stack.cpp file_work/file_work.cpp


mipt_language: frontend middleend backend main_mipt_language.cpp
	g++ main_mipt_language.cpp -o mipt_language

frontend: $(frontend_files)
	g++ $(FLAGS) $(frontend_files) -o obj/front

middleend: $(middleend_files)
	g++ $(FLAGS) $(middleend_files) -o obj/middle

backend: $(cpu_files) backend/source/asm/asm.cpp
	g++ $(FLAGS) $(cpu_files) backend/source/asm/asm.cpp -o obj/back


# ================ clean ================

clean_obj:
	@echo "Cleaning obj"
	rm front middle back

clean_temp:
	@echo "Cleaning temp"
	rm -rf temp/*

clean_img:
	@echo "Cleaning dumps"
	rm -rf frontend/logs/images/*
	rm -rf middleend/logs/images/*

clean: clean_temp clean_img clean_obj
