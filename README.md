# For Users 
## How to Run this program?
1. First off, download the whole project file.
2. You can change the picture in `block_picture` to get your favorite image as a block in game
     - the image must have an English name otherwise the game will fail to open it (but the game still be able to run).
3. The final step different from platform
   - Windows:
     - just run `main.exe` and enjoy the game!
   - Linux and Mac:
     - make sure you have the [gcc compiler](https://gcc.gnu.org/)
     - open the terminal in your system, 
     - use `cd` command to go to this folder
     - run `make`, you will get a program named `main.out`
     - run `main.out` and enjoy the game!
# 給程式設計師
## 如何使用 Git & Github
- 參考資料：
  - https://kopu.chat/git%E6%96%B0%E6%89%8B%E5%85%A5%E9%96%80%E6%95%99%E5%AD%B8-part-1/
  - https://kopu.chat/git%e6%96%b0%e6%89%8b%e5%85%a5%e9%96%80%e6%95%99%e5%ad%b8-part-2/
  - https://w3c.hexschool.com/git/7ca21e02
  - https://www.w3schools.com/git/git_remote_branch.asp?remote=github
  - https://docs.github.com/en/enterprise-server@3.7/get-started/using-git/pushing-commits-to-a-remote-repository
  - https://www.maxlist.xyz/2020/05/03/git-reset-checkout/
  - https://www.freecodecamp.org/news/gitignore-file-how-to-ignore-files-and-folders-in-git/
### **簡介**
> 我只是大略懂，還是**上網查** or **ChatGPT** 比較清楚！
- Git 是**版本管理工具**，當你加了新功能，不小心把程式搞砸時，就可以用 Git 「**回朔**」，不用備分一堆「第 X 版」的程式，還可以「**合併**」兩個不同版本程式，除了讓開發時可以 分成除錯部分與新功能部份 等可能性，還可以用於多人合作！
  - 我是學在終端機下指令，但好像不是必須，所以以下設定可能會因人而異
  - 下載教學：https://w3c.hexschool.com/git/3f9497cd
    - 我沒勾 Git Explorer Intergration，因為之前都沒用到
    - default editor 我用 Notepad++，因為他說不要用預設的 Vim
    - Terminal Emulator 我用 cmd.exe，因為不想多載 MSYS2 (Minimal SYStem 2，很多工具的組合包) 的 MinTTY，之後覺得 cmd 難用再換
  - 第一次下載完需要設定你是誰：
    ```
    git config --global user.name "<暱稱>"
    git config --global user.email <Email帳號>
    ```
    - `--global`：指「所有在此電腦的 Git 專案」都是此設定
    - 如果想取名「Joe」，那指令為：`git config --global user.name "Joe"`

- Github 是**上傳Git專案的平台**，第一次上傳後就包含所有版本，可以供任何人下載至他的本地端Git，做好更新後可再上傳，達到「**遠端分工、分享**」專案的效果
  - 請去 [Github官網](https://github.com/) 註冊一個帳號吧！
- 常用 cmd.exe ( windows 的命令提示字元、終端機) 的指令：
  - `cd ..`：把目前 cmd 所在的路徑退一個資料夾
  - `cd C_programming`在想要修改的專案資料夾路徑下，下指令對其修改
---
### **在新專案開始用 Git**
```
git init
git add .
git commit -m "Initialize project"
```
- 指令意義:
  1. 創立 repository (簡稱repo)，並有分支 (branch) 名為 **master**
  2. 掃描所有檔案，準備好已變動的檔案 準備 committ ( **.** 是指「所有」的意思)
  3. Commit (紀錄至 Git)！ 創立 revision (版本) 叫做 "Initialize project" (**-m** 是指 **m**essage)
- 所以之後只要重複 2. 3.，就可以提交新的 revision
### **第一次上傳 Github**
```
git remote add github <專案URL>
git push -u github master
```
1. 新增 Remote Repo 叫做 **github (可亂取)** (by Github 上開的 URL)
2. 把 **master** 叫的 local branch 更新到叫 **master** 的 remote branch 上
   - Git 會嘗試找到叫 **master** 的 local branch ，沒找到會停止 push
   - 如果在 remote 沒有叫 **master** 的 branch，會創立一個
   - `-u` 把已推完的 remote branch **master** 視為 upstream，之後只要下 `git push`，不用再下更多參數，就可以對直接對此 upstream 更新 (**超不確定**) 
   - PS: 我非常不確定，請看 [Git官方介紹](https://git-scm.com/docs/git-push) 應該比較好，也有很多例子可看
- 所以之後只要執行 2. (可不加 `-u`)，就可以把更新 push 到 Remote Repo
### **第一次從 Github 上抓下 Git 專案**
```
git clone <專案的 URL>
cd <專案名>
git remote rename origin <新名字>
```
- 指令意義:
  1. 把整個專案複製到終端機所在資料夾中，預設名稱為 Github 上的專案名
  2. 因為 remote repo 是各別專案不同的，所以要先到專案中才行
  3. 因預設 remote 名是 origin (與第一個人 push 時用的 remote 名無關)，所以可用此重新命名 remote 

### **分支 (branch)**
- 簡介：
  - 在一個分支上做更新 (commit)，更新完後再合併分支，可避免主要分支被搞砸，還可以把功能分開開發，是分工的好工具

- 創立 branch
  ```
  git branch new_function
  git checkout new_function
  ```
  1. 用目前所在的版本，創立叫 **new_function** 分支
  2. 把現在正在編輯的分支，移動到 **new_function**
- 合併 (Merge) branch
  ```
  git checkout master
  git merge new_function
  ```
  1. 把現在正在編輯的分支，移動到 **master**
  2. 把 **new_function** 合併到 **master** ( **new_function** 分支仍存在)
- 刪除叫 **branch_name** 的 branch 
  ```
  git branch -d branch_name
  ```
### **檢視一些 Git 東西**
- 檢視現在工作的 **branch** + 修改過，但未 commmit 的檔案
    ```
    git status
    ```
- 圖示化 (`--graph`) 所有的 **commit**
    ```
    git log --graph --oneline --all
    ```
    `--oneline`：讓每次的 commit 只顯示一行訊息   
    `--all`：有些 commit 可能被省略，要求全部顯示
    ```
    gitk --all
    ```
    這可以直接開啟叫做 gitk 的程式，上面就有漂亮的圖示了 (`--all` 也是要求全部顯示的意思)，在 commit 量很多時都用這個！

- 檢視所有 **branches** + 現在工作的 **branch** 
    ```
    git branch
    ```
### **上傳、下載**
- **Git 更新版本**
  ```
  git add .
  git commit -m "Changed XXX"
  ```
  這筆 commit 會給**目前的 branch**
- **Git 版本回朔**
  ```
  git checkout HEAD~1
  git branch -m <原本 branch 名> <要丟棄的 branch 名>
  git branch <原本 branch 名>
  git checkout <原本 branch 名>
  git branch -D <要丟棄的 branch 名>
  ```
  - 指令意義：
    1. 把目前的檔案變成上**1**個 commit (`HEAD~1`的**1**) (可以 `HEAD~1` 改成 `566b300` 等在 `git log` 會出現的版本號，就可以回復到指定版本)
    2. 因為 1. 的步驟會把「原本 branch 的尾」與「HEAD (目前所在版本)」分離，如果再對此 commit ，會產生新的 commit路徑，同**新增「未命名的 branch」**，所以為了把名稱換到新的 branch 上，要把舊的 branch 重新命名成是寫錯的 (`-m` 是 **m**ove，所以實際上是 創了一個新名字的 branch，把舊名字的 branch 移到此，並刪除)
    3. 在 **目前的版本開一個正確的 branch**
    4. **把 HEAD 與正確的 branch 連結**，有新的 commit 時 branch 頭才會一起前進
    5. 之後 (代表做完 1. 到 4. 就可以正常 commit 了)，如果確定真的用不到要丟棄的 branch，就刪掉 (`-D` 是指 未 Merge 的 branch 也刪除) ！
       - 如果一開始就很確定錯的版本用不到，可用 `git reset --hard <想留的版本號>` ，一行強制**刪除想留的版本之後的所有 commit** ，`--hard` 不加會使想刪除的變動改到想留的版本，所以須加)
  - 此需要在所有變動都已 commit 之下才能做，不然變動會被消失
- **上傳 Github (會上傳 branch 所有歷史 commit)**
  ```
  git push <取的 Remote Repo 名> <要上傳的 branch 名>
  ```
- **Github 上下載 (會下載 branch 所有歷史 commit)**
  ```
  git pull <取的 Remote Repo 名> <要下載的 Remote branch 名>
  ```
  - 與 push 不同，他會直接把下載的東西 Merge 到**目前個工作分支**
  - 可用 `git pull <取的 Remote Repo 名> <Remote branch名>:<Local branch名>`來指定要 pull 給的目標 local 分支
### **Git 忽略檔案**
- 像是 `.vscode` 這種依編譯器不同而異的、`.exe` 這種只要一編譯就會有的、我們裝的 `SDL2.dll` 這種 Windows獨有的 (但我們專案小平台跨少，而且 .dll 綁 SDL2版本，所以留 .dll 好了)，其實都不值得紀錄，應該被忽略 ([gitignore 模板](https://github.com/github/gitignore))
- 用 `.gitignore` 檔來忽略他們，使他們不會被記錄！
-  `.gitignore` 是文字檔，副檔名為 `.gitignore` ，只要輸入路徑位置，就可忽略路徑下的所有檔案
   -  ex: `/text.txt`：忽略專案自料夾中的 `text.txt`
   -  ex: `/text/text.txt`：忽略專案自料夾中 `text` 資料夾內的 `text.txt`
   -  ex: `/*.md`：忽略專案自料夾中所有 `.md` 檔 (不含資料夾內的 `.md`)
   -  ex: `!/test/example.md`：不要忽略專案自料夾中 `text` 資料夾內的 `example.md`

### **Github 多人合作**
- **共用 Remote Repo** (才能直接 push)：根據 [官方教學](https://docs.github.com/en/account-and-profile/setting-up-and-managing-your-personal-account-on-github/managing-access-to-your-personal-repositories/inviting-collaborators-to-a-personal-repository) ，去 **Settings** > **Access** > **Collaborators**，就可看到 **Add People**，用名字搜尋，即可加入他人一起共用 Repo
- ***更新功能流程***：
  1. 確認自己 local 的 master 與 Remote 的 master 同步，對 local 的 master 開分支 (當然是在最新德版本上)，叫做自己的名字，如「子捷」：`git branch 子捷`
  2. 在「子捷」branch 上，可以做任意更新、任意 commit，此時可以把 local 的 子捷branch 推到 Remote 的 子捷branch，也可以不推 (`git push <Remote> 子捷`)
  3. 做好一個功能了！想要給大家看！就把 local 的 子捷branch 推到 Remote 的 子捷branch：`git push <Remote> 子捷`
  4. 去 Github，找到 **branches** > **Active branches**，應可找到叫「子捷」的 branch，請按旁邊的 **New Pull Request**，發出「我想合併到 master 的訊息」
  5. **？需要一個人測試 ok 並留言？** (測試者在自己的 local master 新增 子捷branch，然後 pull Remote 的 子捷branch，就可測試)
  6. 測試通過，在 Github 的 **Pull Request** 按 **Merge Pull Request** > **Confirm Pull Request**，成功把 Remote 的 子捷branch 與 Remote master 合併
  7. 所有人，把新的 Remote master pull到自己的 local master：`git pull <Remote> master:master`
  8. 非此功能修改者，把新的 local master merge 到自己正在寫 branch 中，並手動處理、更改其中可能的衝突：`git merge master`，就完成大家同步了！

***

## 如何寫 Markdown (用於README.md)
- 我是用 Visual Studio Code 的 `Markdown All in One` 擴充東西，就可以用 VScode 開 .md 檔，並有快速鍵編輯 + 預覽 .md 檔
### **語法**
  換行: 打兩次 \\n, 或 空格兩次 + \\n
- `one line code exmaple`
- **bold example**
- *tilted example*
- ***bold and tilted example***
- 表格範例: 
    a|b|c|d
    -|-|-|-
    1|2|3|10
    4|5|6|22
- 程式片段例子 (`c` 可換成 .md 支援的程式語言名字):
    ```c
    int main()
    {
        printf("hello！");
        return 0;
    }   
    ```
- 勾選框框例子:
  - [ ] 1 + 1 = 3
  - [X] 1 + 1 = 2
- list 例子:
  - Zelda
    - Breath of the wild
    - Tears of the kindom
  - Mario
    - Oddessy
    - Galaxy 1 & 2
- 額外區域(?) 例子:
    > Lowly Tarnished,   
    Playing as a Lord   
    >>I command thee **knee**!
    >
    >I am the king that's all of Golden!  
    >- Axe equiped
    >- Creepy smile readied  
    >
    > *the battle begin*
     
    >Ah~, trusest of dragon   
    lend me thy strength...  
    >```c
    >Goldrick = Goldrick - arm + dragon;
    >```
    >Forefarthers, one and all...
    >> `Bear withness!`
    >
    >*Goldrick's power is now UNPARALLEDED*
    - PS: 我不太確定如何使用這個
### **常見共通準則**
- 中文: https://github.com/sparanoid/chinese-copywriting-guidelines

***
## **顯示圖片 in C (SDL_2)**
- SDL2：Simple DirectMedia Layer，總之就是能顯示視窗、圖片的C函式庫！
- SDL2_image：SDL 的擴充，原本只能顯示 bmp 格式的圖片，有了這個擴充就幾乎所有圖片格式都可以顯示
### **下載 (Windows + MinGW)**
1. 首先是 **SDL2**，去 [官網](https://github.com/libsdl-org/SDL/releases/tag/release-2.26.5) 下載 `SDL2-devel-2.26.5-mingw.zip` 或 `SDL2-devel-2.26.5-mingw.tar.gz` ，都一樣是壓縮檔
2. 打開壓縮檔，到 `x86_64-w64-mingw32` (大家應該都是 64 bits 吧) > `include`，裡面只有叫 `SDL2` 的資料夾，那裡有所有的 header file，所以把 `SDL2` 貼到與我們的 `main.c` 同一個資料夾中
3. 到剛剛的 `x86_64-w64-mingw32` > `bin`，有一個叫 `SDL2.dll` 的東西，是他們把一坨 .c 檔編譯成一個檔案，只要有 linker 連接我們的 `main.c` 就能用，所以也把他貼到與我們的 `main.c` 同一個資料夾中
4. 下載完了！試試以下範例：
    ```c
    #include <stdio.h>
    #include "SDL2/SDL.h"
    #undef main //必須加！SDL很奇怪地把 main 用 Macro 定義了，導致 int main 會被修掉，所以要 undef 掉

    int main(int argc, char** argv) {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError()); //stderr是專門輸出錯誤訊息的地方，而fprintf可以把訊息print到非stdio的地方
            return 1;
        }

        // Create window
        SDL_Window* window = SDL_CreateWindow("SDL Window Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
        if (!window) {
            fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }

        // Wait for user to quit
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        // Destroy window
        SDL_DestroyWindow(window);

        // Quit SDL
        SDL_Quit();

        return 0;
    }
    ```
    要編譯的時候，要告訴 gcc 把 SDL.dll 與 main.c 連接，所以要用 `gcc main.c SDL2.dll -o main.exe`，當作同時編譯這兩個東西 (單獨編譯時 linker 找不到 SDL.dll，需要輸入更多指令，好複雜，求簡單放棄編譯效率，直接再編一次)，就會 link 在一起，成功顯示視窗！
  5. 如果跟我一樣用 VScode 而且懶了打指令，可以去 `.vscode` 的 `task.json` 中，在 `args` 區中的 `"${file}",` 下一行多加一個 `"SDL2.dll",` 的項，就會按執行紐就直接編譯 + 執行！  
  6. 接下來是 **SDL2_image**，步驟就和前面幾乎一模一樣，先去 [官網](https://github.com/libsdl-org/SDL_image/releases) 載好 `SDL2_image-devel-2.6.3-mingw.zip`
  7. 找到 `x86_64-w64-mingw32` > `include` > `SDL2` 裡有唯一一個 header，把他也丟到 2. 創的 SDL2 資料夾中
  8. 在 `x86_64-w64-mingw32` > `bin`，把裡面的 `SDL2_image.dll` 丟到 main.c 同一個資料夾中
  9. 下載完成！試試看更複雜的：
      ```c
        #include <stdio.h>
        #include "./SDL2/SDL.h"
        #include "./SDL2/SDL_image.h"
        #undef main

        int main(int argc, char** argv) {
            // Initialize SDL
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
                return 1;
            }

            // Initialize SDL_image
            if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
                fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
                SDL_Quit();
                return 1;
            }

            // Create window
            SDL_Window* window = SDL_CreateWindow("SDL Image Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
            if (!window) {
                fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
                IMG_Quit();
                SDL_Quit();
                return 1;
            }

            // Create renderer
            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer) {
                fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                return 1;
            }

            // Load image as texture
            SDL_Texture* imageTexture = IMG_LoadTexture(renderer, "image.png");
            if (!imageTexture) {
                fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                return 1;
            }

            // Clear renderer
            SDL_RenderClear(renderer);

           // Set the destination rectangle
          SDL_Rect destRect;
          destRect.x = 100;  // x-coordinate of the destination rectangle
          destRect.y = 100;  // y-coordinate of the destination rectangle
          destRect.w = 200;  // width of the destination rectangle
          destRect.h = 200;  // height of the destination rectangle

          // Copy texture to renderer
          SDL_RenderCopy(renderer, imageTexture, NULL, &destRect);

            // Update renderer
            SDL_RenderPresent(renderer);

            // Wait for user to quit
            SDL_Event event;
            while (SDL_WaitEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    break;
                }
            }

            // Destroy texture, renderer, and window
            SDL_DestroyTexture(imageTexture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);

            // Quit SDL_image and SDL
            IMG_Quit();
            SDL_Quit();

            return 0;
        }
      ```
      這個就可以在指定位置中顯示圖片 (要與 main.exe 同一個資料夾)，同樣，編譯的指令就變成 `gcc main.c SDL2.dll SDL2_image.dll -o main.exe`  
  10. **全部載完了**！如果一樣很懶，就在剛剛 `task.json` 加 `"SDL2.dll",` 的地方再加一行 `"SDL2_image.dll",` 就好！
  11. 但因為我們會分成多個檔案開發，所以需要 `Makefile`，在下面的 **專案架構說明** 有寫到如何使用

## **專案架構說明**
- `block_picture` 資料夾：放入要顯示方塊的圖片 (png)
- `include` 資料夾：放所有 **header** 的資料夾，所以如果要使用在其中的 header，請用 `#include "./include/<header 名字>.h"`，像是我把常用的 Macro 與 函式庫丟入 `basicSetting.h` 中後，之後只要 `#include "./include/basicSetting.h"` 就可以用裡面的東西
- `SDL2` 資料夾：放所有 **SDL2 相關的 header**，像是在 `basicSetting.h` 中用的 `#include "../SDL2/SDL.h"` 就用到了這裡的 header
- `source` 資料夾：放所有 **除了 main.c 以外的 .c 檔**，寫完之後如何編譯請看 `Makefile` 的說明 
- `.gitignore`：詳情使用方式已在 **如何使用 Git & Github** 更新，我只忽略了 `.vscode` 與 `*.exe` (也就是唯一的 `main.exe`)
- `main.c`、`main.exe`：主程式與最後的可執行程式
- `Makefile`：
  - 老師教的那個！
  - 如何在 Windows 執行 Makefile：
    參考 [這篇](https://medium.com/@s815651/%E5%9C%A8windows%E4%B8%8B%E4%BD%BF%E7%94%A8makefile-5dc8b4e6ec90) 教學，把 mingGW 中 `mingw32-make` 的名字命名成 `make` (預設下載在 `C:\Program Files\mingw64\bin` 中)，就可以用 make 指令！
  - 再魔改助教的 Makefile (可以打開看看內容)，主要是改了 **.c的位置** (在 `soruce\*.c` ) 與**刪除 .o 的方式** (`del`)，讓他一個指令就 檢查所有變動的 .c 檔，編譯變動的 .c 成 .o，把 .o 與 .dll 全部 link 起來，最後刪除 .o！
  - 所以，只要在 cmd 輸入 `make`，就會**編譯所有已變動的 .c 檔**！然後在輸入 `./main.exe` 就可以**執行我們的程式**！
- `README.md`：你現在正在看的東西
- `SDL2_image.dll`、`SDL2.dll`：是**已編譯好的 SDL 函式庫**，我們的 `.exe` 要執行必須要用他們，所以需與 `.exe` 放於同一層資料夾中

- **新增 SDL_TFF**：請依照之前安裝 SDL_image 的步驟，從 [這裡](https://github.com/libsdl-org/SDL_ttf/releases) 安裝 SDL_TFF，此是 SDL 顯示文字的擴充套件，支援 `.tff` 檔 (TrueTypeFormat)，會比用圖片顯示文字方便 (不確定效能是否比較好，但這個 `.dll` 不知為何如此巨大)
## Minecraft 2D 素材 (160 x 160)
- https://minecraft.fandom.com/wiki/List_of_block_textures
## Flow Chart 繪製工具：
- https://app.diagrams.net/ (線上、免費！)