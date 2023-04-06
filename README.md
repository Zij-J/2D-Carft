# 給程式設計師
## 如何使用 Git & Github
- 參考資料：
  - https://kopu.chat/git%E6%96%B0%E6%89%8B%E5%85%A5%E9%96%80%E6%95%99%E5%AD%B8-part-1/
  - https://kopu.chat/git%e6%96%b0%e6%89%8b%e5%85%a5%e9%96%80%e6%95%99%e5%ad%b8-part-2/
  - https://w3c.hexschool.com/git/7ca21e02
  - https://www.w3schools.com/git/git_remote_branch.asp?remote=github
  - https://docs.github.com/en/enterprise-server@3.7/get-started/using-git/pushing-commits-to-a-remote-repository
  - https://www.maxlist.xyz/2020/05/03/git-reset-checkout/
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
- 刪除現在正在編輯的 branch 
  ```
  git branch -d
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
    這可以直接開啟叫做 gitk 的程式，上面就有漂亮的圖示了 (`--all` 也是要求全部顯示的意思)！

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
### **顯示圖片 in C**
- SDL2？ChatGTP給的範例：
- example of how to modify the SDL_Rect structure to place the image at the position (100, 100) in the window:
  ```c
  #include <SDL2/SDL.h>

  int main(int argc, char* argv[]) {
      SDL_Window* window = NULL;
      SDL_Surface* surface = NULL;
      SDL_Surface* image = NULL;

      // Initialize SDL
      if (SDL_Init(SDL_INIT_VIDEO) < 0) {
          printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
          return 1;
      }

      // Create window
      window = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
      if (window == NULL) {
          printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
          return 1;
      }

      // Load image
      image = SDL_LoadBMP("image.bmp");
      if (image == NULL) {
          printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
          return 1;
      }

      // Create surface and blit the image onto it at position (100, 100)
      surface = SDL_GetWindowSurface(window);
      SDL_Rect dest_rect = {100, 100, image->w, image->h};
      SDL_BlitSurface(image, NULL, surface, &dest_rect);
      SDL_UpdateWindowSurface(window);

      // Wait for window to close
      SDL_Event e;
      while (1) {
          if (SDL_PollEvent(&e) && e.type == SDL_QUIT) {
              break;
          }
      }

      // Cleanup
      SDL_FreeSurface(image);
      SDL_DestroyWindow(window);
      SDL_Quit();
      return 0;
  }
  ```


# For Users 
- 