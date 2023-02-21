# For Programmers
## How to use Git & Github
- Online Guidence:
  - https://kopu.chat/git%E6%96%B0%E6%89%8B%E5%85%A5%E9%96%80%E6%95%99%E5%AD%B8-part-1/
  - https://kopu.chat/git%e6%96%b0%e6%89%8b%e5%85%a5%e9%96%80%e6%95%99%e5%ad%b8-part-2/
  - https://w3c.hexschool.com/git/7ca21e02
  - https://www.w3schools.com/git/git_remote_branch.asp?remote=github
  - https://docs.github.com/en/enterprise-server@3.7/get-started/using-git/pushing-commits-to-a-remote-repository
### **Initialize Git**
( in project's root folder )
```
git init
git add .
git commit -m "Initialize project"
```
- Command Meaning:
1. Creat repository **master** ( the main branch )
2. ready the files to be committed ( **.** means all files)
3. Commit files! Create revision called "Initialize project" (**-m** stands for **m**essage)
### **Branches**
- Creat branch
  ```
  git branch new_function
  git checkout new_function
  ```
  1. Creat a branch called **new_function**
  2. Move now working branch to **new_function**
- Merge branch
  ```
  git checkout master
  git merge new_function
  ```
  1. Move now working branch to **master**
  2. Merge **new_function** to **master**
- Delete branch 
  ```
  git branch -d
  ```
### **Checking Git**
- Check out what **branch** am I and which file did I modify ( still not commit ) 
    ```
    git status
    ```
- Check out all **commit**
    ```
    git log
    ```
- Check out all **branches** ( and what branch we are in )
    ```
    git branch -a
    ```
### **First Upload to Github**
```
git remote add ProgrammingFinalProject <the url>
git push -u ProgrammingFinalProject master
```
1. Add a Remote Repo called **ProgrammingFinalProject** (by URL)
2. Push a local branch named **master** to remote branch named **master**
   - **master** you gave in the command means **target romote branch name**
   - Git will find local branch have the same name with **master** as stuff to push
   - if you push a new branch that Github don't have, it will create it 
   - `-u` set the local **master** branch as a remote **master** branch ( main branch, or **upstream** ) so all changes will be started from here, beforehand are all the same ( **SUPER UNSURE** ) 
   - PS: I'm still not very sure all of the correctness of this information



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


###


# For Users 
- 