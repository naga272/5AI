## Chapter 3: Our First Branch

In this chapter, we'll create a new branch named "feature/hello-world" and a simple Python program to print "Hello, World". Finally, we'll merge this feature branch into the master branch.

### 3.1 Creation a new branch

1. In your terminal, navigate to the repository directory:
   ```bash
   cd path/to/First-Git_exercise
   ```

2. Create a new branch named "feature/hello-world":
   ```bash
   git checkout -b feature/hello-world
   ```

   This command creates and switches to a new branch.

Alternatively, we can first create the branch and then switch to that branch.

```
git branch feature/hello-world
git checkout feature/hello-world
```

We can check the current branch running `git branch -a`: the starred one is the branch we are currently working on.

### 3.2 Add a Simple Python Program

1. Create a new Python file named `hello_world.py`. This can be done both with a GUI and using the following CLI:
   ```bash
   echo $null > hello_world.py
   ```

2. Open `hello_world.py` in a text editor.

3. Add the following Python code:
   ```python
   # hello_world.py
   print("Hello, World!")
   ```

### 3.3 Commit Changes in the New Branch

Exactly like with the previous README file, we are not done. We have to "save" the changes in the right Git area.

1. Stage the changes:
   ```bash
   git add hello_world.py
   ```

2. Commit the changes:
   ```bash
   git commit -m "Add hello_world.py"
   ```

### 3.4 Switch to the Master Branch and Merge

Here comes the first difference with the previous exercise. Now we do not push the updates directly to the master branch in order to avoid conflicts.

1. Switch back to the master branch:
   ```bash
   git checkout master
   ```

2. Merge the changes from the "feature/hello-world" branch into the master branch:
   ```bash
   git merge feature/hello-world
   ```

   If there are no conflicts, this will perform a fast-forward merge.

### 3.6 Delete the Feature Branch (Optional, but recommended)

1. If you want to clean up, you can delete the feature branch:
   ```bash
   git branch -d feature/hello-world
   ```

   The `-d` option stands for delete.

Congratulations! You've successfully created a new branch, added a simple Python program, committed the changes, and merged the branch into the master branch. This time a fast-forward merge should have been performed. How was the history edited? Can we see the "feature/hello-world" branch on the central repository? Where do we find tracks of its existence?

### 3.7 Oops! Something went wrong (?)
Did you check on GitHub for your last update on the repository? Where did the last update go?

Maybe we forgot to **push** them to the repo?