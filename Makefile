# 輸入 make 就可以編譯全部程式！
# 如果新增了新的.c檔，請放在source內，再把codes加上 source\<檔案名>.o 就可以了

#windows在cmd中的路徑必須用\，但C的#include就不用，超怪
codes := main.o source\SDL_StartAndEnd.o
dlls := SDL2.dll SDL2_image.dll
exe := main.exe

# 輸入任何東西都要檢查是否$(codes)的東西變過，之後刪除 .o 檔 (.dll在執行時也要被看見，所以必須噢.exe放於同一檔)
all: $(codes)
	gcc $(codes) $(dlls) -o $(exe)
	del $(codes) 


# 檢查 是否%.o的東西變過 時，就檢查%.c是否變過，有了話就編譯(沒link)
# $@是指 %.o 的檔案名(含副檔名)；$^是指 %.c 的檔案名(含副檔名)
%.o: %.c
	gcc -c $^ -o $@