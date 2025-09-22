# 輸入 make 就可以編譯全部程式！
# 如果新增了新的.c檔，因為有用 wildcard，放到 source 資料夾就好！
# 如果新增了新的.h檔，因為有用 wildcard，放到 include 資料夾就好！

# 處理 codes，把所有.c檔(by wildcard)，用 basename 去掉.c，再用 addsuffix 加上.o，就是我們要的所有.o檔了！
source := $(wildcard source/*.c)
source := $(basename $(source))
source := $(addsuffix .o, $(source)) 
codes := main.o $(source)
# dll 與 exe 也定義一下
dlls := SDL2.dll SDL2_image.dll SDL2_ttf.dll
ifeq ($(OS),Windows_NT)
	exe := main.exe
else
	exe := main.out
endif

# header 變動也要處理！用 wildcard 就可處理全部在 include 內的.h
headers := $(wildcard include/*.h)
# 這是 Windows 刪除的路徑(因為 windows 的 del 強迫要用 \ 當路徑，只能再改，用 notdir 只提出檔名，再用 addprefix 把新路徑加上去)
outs := $(notdir $(source))
outs := main.o $(addprefix source\, $(outs))

# 編譯指令也是要依作業系統不同而變，因-mwindows(用於隱藏 console 只能用於 windows，Mac 預設不會開啟 console 所以沒問題)-mwindows 
ifeq ($(OS),Windows_NT)
	compileCommand := gcc $(codes) $(dlls) -o $(exe)
else
	compileCommand := gcc $(codes) -lSDL2 -lSDL2_image -lSDL2_ttf -o $(exe)
endif

# del command 要依作業系統不同而變
ifeq ($(OS),Windows_NT)
	delCommand := del $(outs)
else
	delCommand := rm -rf $(codes)
endif

# 輸入任何東西都要檢查是否 $(codes) 的東西變過 (.dll 在執行時也要被看見，所以必須噢 .exe 放於同一檔)
all: $(codes)
	$(compileCommand)


# 檢查 是否% .o 的東西變過 時，就檢查 %.c 是否變過，有了話就編譯(沒link)
# $@ 是指 %.o 的檔案名(含副檔名)；$< 是指 %.c 的檔案名(含副檔名) (是指「第一個」右邊的項目)，有 header 動過就全部編譯一遍！紀錄 header 動過要編譯誰好麻煩！
%.o: %.c $(headers)
	gcc -c $< -o $@



# del 會刪掉 .o 檔，會使 makefile 找不到 .o，所以會再去 .c 找然後執行生出 .o，所以不能馬上執行並刪除，要快就不能自行刪除
# .PHONY：假裝有一個叫 clean 的檔案存在，所以如果用 make clean，就會執行 clean: 的指令
.PHONY: clean
clean:
	$(delCommand)
	