# 給程式設計師
## 如何使用 Git & Github
- 參考資料：
  - https://kopu.chat/git%E6%96%B0%E6%89%8B%E5%85%A5%E9%96%80%E6%95%99%E5%AD%B8-part-1/
  - https://kopu.chat/git%e6%96%b0%e6%89%8b%e5%85%a5%e9%96%80%e6%95%99%e5%ad%b8-part-2/
  - https://w3c.hexschool.com/git/7ca21e02
  - https://www.w3schools.com/git/git_remote_branch.asp?remote=github
  - https://docs.github.com/en/enterprise-server@3.7/get-started/using-git/pushing-commits-to-a-remote-repository
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
git remote add ProgrammingFinalProject <專案URL>
git push -u ProgrammingFinalProject master
```
1. 新增 Remote Repo 叫做 **ProgrammingFinalProject** (by Github 上開的 URL)
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
  1. 創立叫 **new_function** 分支
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
    git log --graph --oneline
    ```
    `--oneline`：讓每次的 commit 只顯示一行訊息

- 檢視所有 **branches** + 現在工作的 **branch** 
    ```
    git branch
    ```
### **上傳、下載**
- 

### **Github 多人合作**
- **共用 Remote Repo** (才能直接 push)：根據 [官方教學](https://docs.github.com/en/account-and-profile/setting-up-and-managing-your-personal-account-on-github/managing-access-to-your-personal-repositories/inviting-collaborators-to-a-personal-repository) ，去 **Settings** > **Access** > **Collaborators**，就可看到 **Add People**，用名字搜尋，即可加入他人一起共用 Repo
- ***更新功能流程***：
  1. 把 








***

## How to Markdown
### **Grammer**
- next line: type \\n twice, or space twice and \\n
- `one line code exmaple`
- **bold example**
- *tilted example*
- ***bold and tilted example***
- table example: 
    a|b|c|d
    -|-|-|-
    1|2|3|10
    4|5|6|22
- code segment exmple:
    ```c
    int main()
    {
        printf("hello！");
        return 0;
    }   
    ```
- check box exmple:
  - [ ] 1 + 1 = 3
  - [X] 1 + 1 = 2
- list example
  - Zelda
    - Breath of the wild
    - Tears of the kindom
  - Mario
    - Oddessy
    - Galaxy 1 & 2
- new area(?) example:
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
    - PS: I'm still not very sure how to use this function
### **Common Rules**
- Chinese: https://github.com/sparanoid/chinese-copywriting-guidelines


# For Users 
- 