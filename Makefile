# 輸入 make 就可以編譯全部程式！
# 如果新增了新的.c檔，請放在source內，再把codes加上 source\<檔案名>.o 就可以了

#windows在cmd中的路徑必須用\，但C的#include就不用，超怪
codes := main.o source\SDL_StartAndEnd.o source\blockDataBase_Position.o source\blockDataBase_Texture.o
dlls := SDL2.dll SDL2_image.dll
exe := main.exe

# 輸入任何東西都要檢查是否$(codes)的東西變過，之後刪除 .o 檔 (.dll在執行時也要被看見，所以必須噢.exe放於同一檔)
all: $(codes)
	gcc $(codes) $(dlls) -o $(exe)


# 檢查 是否%.o的東西變過 時，就檢查%.c是否變過，有了話就編譯(沒link)
# $@是指 %.o 的檔案名(含副檔名)；$^是指 %.c 的檔案名(含副檔名)
%.o: %.c
	gcc -c $^ -o $@


# del 會刪掉 .o 檔，會使 makefile 找不到 .o，所以會再去 .c 找然後執行生出 .o，所以不能馬上執行並刪除，要快就不能自行刪除
# .PHONY：假裝有一個叫 clean 的檔案存在，所以如果用 make clean，就會執行clean:的指令
.PHONY: clean
clean:
	del $(codes) 
# 原本是rm -rf (強迫刪除)，要換成window